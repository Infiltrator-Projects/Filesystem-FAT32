# Filesystem-FAT32

Independent FAT32 filesystem engine for InfiltratorOS. Builds `FAT32.so` directly from source, with a portable DEB and a checksum-verified source-embedded native `.run` installer.

The original FAT32 V1 reader has authentic FAT32 BPB, root-cluster, fragmented-read and width checks. The integrated portable ChaN FatFs implementation adds V3 file/directory operations and V4 volume formatting, attributes, timestamps and free-space reporting. Directory renames are deliberately disabled until FAT32 ancestry checks are qualified. All writing requires exclusive media ownership and host-supplied read/write/flush callbacks.

```sh
make check
make release
```

Aggressive builds use `-O3 -march=native -mtune=native -flto=auto` and real two-pass GCC PGO trained on disposable regular-file images; release CI must confirm ABI exports, native build records and module SHA-256.

**Scope:** the initial release includes only `/System/Filesystems/FAT32.so`. Native Linux `.ko`, EFI `.efi`, complete Defragmenter integration, root mounting and physical-device safety are not yet qualified or published.

Sources originated in [Filesystems](https://github.com/Infiltrator-Projects/Filesystems) and the existing FAT12/FAT16 independent module work. See [provenance](MIGRATION-PROVENANCE.md) and [validation](docs/VALIDATION.md).
