#!/usr/bin/env python3
"""Qualify FAT32.so using Defragmenter's unmodified production FAT child host.

Uses independent FAT32 images and read-only validators from the consumer
checkout. Never opens or formats a physical block device.
"""
import argparse
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


def run(*args, **kwargs):
    return subprocess.run([str(arg) for arg in args], check=True, **kwargs)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--module", type=Path, required=True)
    parser.add_argument("--defragmenter-source", type=Path, required=True)
    args = parser.parse_args()
    module = args.module.resolve()
    source = args.defragmenter_source.resolve() / "defragger"
    root = Path(__file__).resolve().parents[1]
    assert (root / "native/filesystems/fat/defrag/fat_defrag_abi.h").read_bytes() == (
        source / "native/fat_defrag_abi.h").read_bytes(), "FAT defrag ABI drift"
    assert (source / "shared/infiltratr-common/VERSION").is_file(), "Common submodule missing"
    sha = lambda p: hashlib.sha256(Path(p).read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory(prefix="fat32-real-host-") as tmp:
        work = Path(tmp)
        install = work / "Filesystems"
        install.mkdir()
        shutil.copyfile(module, install / "FAT32.so")
        assert sha(module) == sha(install / "FAT32.so")
        build = work / "buildhost"
        build.mkdir()
        core = source / "src/core"
        version = (source / "VERSION").read_text().strip()
        (build / "version.h").write_text((core / "version.h.in").read_text().replace(
            "@DEFRAGMENTER_PACKAGE_VERSION@", version))
        (build / "CMakeLists.txt").write_text(f'''cmake_minimum_required(VERSION 3.20)
project(FAT32RealConsumer C)
set(INFILTRATR_COMMON_BUILD_TESTS OFF)
set(INFILTRATR_COMMON_BUILD_SHARED OFF)
add_subdirectory("{source}/shared/infiltratr-common" common)
add_executable(fat-worker "{source}/native/fat_module_loader.c"
 "{core}/ld_runtime.c" "{core}/ld_io.c" "{core}/ld_device.c"
 "{core}/ld_stop.c" "{core}/ld_path.c" "{core}/ld_protocol.c")
target_include_directories(fat-worker PRIVATE "{core}" "{source}/native" "{build}")
target_compile_definitions(fat-worker PRIVATE _GNU_SOURCE _FILE_OFFSET_BITS=64 IFS_FAT_TESTING)
target_compile_options(fat-worker PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(fat-worker PRIVATE InfiltratrCommon::Common dl pthread)
''')
        run("cmake", "-S", build, "-B", build / "cmake")
        run("cmake", "--build", build / "cmake", "-j", "4")
        worker = build / "cmake/fat-worker"
        environment = dict(os.environ, IFS_TEST_MODULE_PARENT=str(work))
        fixture = source / "tests/make_growth_defrag_image.py"
        verify = source / "tests/verify_growth_defrag.py"

        def create(name):
            image = work / (name + ".img")
            run("python3", fixture, image)
            return image

        def command(action, image, journal, more=None, allowed=(0,)):
            env = dict(environment, **(more or {}))
            x = subprocess.run([str(worker), action, str(image), "--write",
                                "--confirm", str(image), "--journal", str(journal)],
                               env=env, capture_output=True, text=True)
            assert x.returncode in allowed, (action, x.returncode, x.stdout, x.stderr)
            return x.returncode

        # Verify a separate dosfstools-created clean volume through both
        # the system checker and this exact module before testing relocation.
        clean = work / "clean-fat32.img"
        with clean.open("wb") as stream:
            stream.truncate(128 * 1024 * 1024)
        run("mkfs.fat", "-F", "32", "-I", "-s", "1", clean)
        run("fsck.fat", "-n", clean)
        command("defrag", clean, work / "clean.journal")
        run("fsck.fat", "-n", clean)

        # The historical Growth fixture predates its fsck-clean metadata:
        # boot label has no root label entry, and FSINFO free count is unknown.
        # Verify these known baseline warnings without changing its root
        # entry offsets, which the independent Growth oracle depends upon.
        def check_growth_fixture(image):
            result = subprocess.run(["fsck.fat", "-n", str(image)],
                                    capture_output=True, text=True)
            assert result.returncode in (0, 1), result.stdout + result.stderr
            assert "Label in boot sector is 'GROWTEST'" in result.stdout
            assert "Free cluster summary uninitialized" in result.stdout
            assert "FATs differ" not in result.stdout

        image = create("growth")
        response = subprocess.check_output([str(worker), "identify", str(image)],
                                           env=environment, text=True)
        assert '"bits":32' in response, response
        command("growth-defrag", image, work / "growth.journal")
        run("python3", verify, image)
        check_growth_fixture(image)
        for stage in (1, 2, 3, 4):
            image = create("interrupt-" + str(stage))
            journal = work / ("interrupt-" + str(stage) + ".journal")
            command("growth-defrag", image, journal,
                    {"IFS_TEST_CRASH_AFTER_JOURNAL":str(stage)}, (86,))
            assert journal.is_file()
            wrong = work / "wrong.img"
            shutil.copyfile(image, wrong)
            before = sha(wrong)
            command("recover", wrong, journal, allowed=(1,))
            assert sha(wrong) == before and journal.is_file()
            command("recover", image, journal)
            assert not journal.exists()
            # Recover guarantees original content/consistency; interrupted
            # source may still be in a noncanonical placement. Finish it.
            command("growth-defrag", image, journal)
            run("python3", verify, image)
            check_growth_fixture(image)
        foreign = work / "foreign.img"
        foreign.write_bytes(bytes(16 * 1024 * 1024))
        run("mkfs.fat", "-F", "16", "-I", foreign)
        before = sha(foreign)
        command("defrag", foreign, work / "foreign.journal", allowed=(1,))
        assert sha(foreign) == before
        assert sha(module) == sha(install / "FAT32.so")
    print("FAT32-SHARED-SO-REAL-DEFRAGMENTER-RECOVERY-PASS", sha(module))


if __name__ == "__main__":
    main()
