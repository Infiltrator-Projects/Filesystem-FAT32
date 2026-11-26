# Filesystem-FAT32

## 1. Purpose and format scope

Microsoft FAT32, with a chain-based root directory and 28 significant allocation-link bits. This repository owns the format-specific implementation of `FAT32.so` for InfiltratorOS.

## 2. On-disk architecture and invariants

Validate BPB geometry, root cluster, FAT boundaries and active/mirrored FAT policy. Classify FAT32 from the actual cluster population. Preserve the upper reserved nibble when changing FAT entries; treat FSInfo free-count and next-free values as advisory. Ordinary directory file length is 32-bit. Directory rename must preserve ancestry and avoid cycles.

## 3. Common V2 implementation and capabilities

Common V2 provides probe, mount, file/directory operations, sync and format through the FAT32-selected engine. The hosted FatFs engine supports one mounted volume per loaded image. Native namespace and capacity errors are preserved. BOOT consumes the read-only profile; the dedicated relocation and recovery interface remains separately qualified. The canonical entry point is `infiltrator_filesystem_api_v2`. Consumers must negotiate the actual profile and operation masks and use bounded, partition-relative host I/O. A source-level callback does not by itself establish that every format variant or host profile is qualified.

## 4. Integrity, mutation, allocation and recovery

A backup boot sector or mirrored FAT is not a journal. Preserve all allocation references during relocation and verify payload and target identity before recovery. The legacy validation record includes OVMF handoff and dedicated Defragmenter recovery tests; physical-media crash safety and generic kernel-side SO mounting remain separate. For destructive operations, require the applicable exclusive writable host, durability contract and format-specific validation. Never silently substitute a kernel or FUSE driver or treat a rejected or unavailable operation as success. Preserve original disk-format semantics; relocation and recovery are separate from ordinary file I/O.

## 5. Build, installation and release

The target installation is `/System/Filesystems/FAT32.so` and `/System/Resources/Documentation/Filesystem-FAT32.pdf`. The project release policy is a portable amd64 DEB and a CPU-native source-first RUN, with GitHub's automatic source archive; do not treat this policy statement as verification of the current installer payload. The release workflow is the authoritative executable build and package specification. Historical adapters and test artifacts are not additional production filesystem authorities.

## 6. Qualification and limitations

[Recorded qualification workflow](https://github.com/Infiltrator-Projects/Filesystem-FAT32/actions/runs/45519961986) · [Current workflows](https://github.com/Infiltrator-Projects/Filesystem-FAT32/actions?query=branch%3Amain). A successful workflow establishes the tested scope only, not complete original-format parity, physical-media crash safety, or production BOOT/kernel integration. Use disposable images for destructive qualification.

## 7. Technical references and provenance

- [Provider implementation](native/filesystems/fat/portable/fatfs_module.c)
- [Native design or source provenance](docs/VALIDATION.md)
- [Migration and source history](MIGRATION-PROVENANCE.md)
- [Original-format reference source](docs/FAT32.pdfstreams) and [PDF](docs/FAT32.pdf)
- [Field-level original-format specification supplement](docs/FORMAT_SPECIFICATION.md)
- [Project philosophy](docs/PHILOSOPHY.md)
- [Common ABI](https://github.com/Infiltrator-Projects/Libraries)
- [One-module architecture](docs/ONE_MODULE_END_STATE.md)

The original-format reference and historical migration notes describe different evidence from the current production provider. When descriptions disagree, the current exported descriptor, implementation, executable tests and exact-revision release evidence take precedence.

The [engineering reference](docs/FAT32-TECHNICAL-REFERENCE.md) integrates the original-format specification and worked exercises; earlier illustrated material is preserved in the PDF.
