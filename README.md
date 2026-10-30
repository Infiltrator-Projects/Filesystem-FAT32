# Filesystem-FAT32

Independent FAT32 filesystem engine for InfiltratorOS. Builds `FAT32.so` directly from source, with a portable DEB and a checksum-verified source-embedded native `.run` installer.

The original FAT32 V1 reader has authentic FAT32 BPB, root-cluster, fragmented-read and width checks. The integrated portable ChaN FatFs implementation adds V3 file/directory operations and V4 volume formatting, attributes, timestamps and free-space reporting. Directory renames use the source-owned FAT32 ancestry guard; broader independent ancestry and malformed-directory qualification remains open. All writing requires exclusive media ownership and host-supplied read/write/flush callbacks.

```sh
make check
make release
```

Aggressive builds use `-O3 -march=native -mtune=native -flto=auto` and real two-pass GCC PGO trained on disposable regular-file images; release CI must confirm ABI exports, native build records and module SHA-256.

**Version 0.1.3 scope:** One authoritative `FAT32.so` supplies V1/V3/V4 read/write/format, generic Boot V1 and the production Defragmenter relocation/recovery interface. The unchanged Boot ELF/UEFI adapter loads the same module and hands off a real Linux kernel/initramfs in OVMF. Defragmenter exercises that very SO through its production host, including Growth Defrag, interrupted recovery, wrong-target rejection and independently checked FAT images. Native portable and source-compiled PGO packages are separately qualified.

The independent `FAT32.efi` and `fat32-linux-6.12.107.ko` are also VM-tested and published as transitional compatibility drivers; the KO is pinned to that kernel ABI. The final generic kernel-side SO bridge, original firmware/physical media and power-loss durability remain separate integration gates.

Sources originated in [Filesystems](https://github.com/Infiltrator-Projects/Filesystems) and the existing FAT12/FAT16 independent module work. See [provenance](MIGRATION-PROVENANCE.md) and [validation](docs/VALIDATION.md).

## Preserved Linux and EFI source (0.1.1)

The complete original Linux FAT/VFAT source under `native/filesystems/fat/linux/` and TianoCore EDK2 EFI FAT source under `native/filesystems/fat/efi/` have been imported as **real, editable source files**, not references. All 50 imported blobs were checked byte-for-byte against the pinned Filesystems source revision. Independent FAT32-specific transitional KO and EFI binaries are VM-qualified for their pinned environments, but the DEB and RUN still install only `FAT32.so`.

The source archive and native RUN contain these sources so future FAT32-specific `.ko` and `.efi` work can proceed without returning to a scattered implementation. The corresponding kernel and firmware release artifacts have their own qualification workflows; they are not substitutes for the eventual generic kernel SO loader.
