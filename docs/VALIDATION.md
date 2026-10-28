# FAT32 independent qualification

- FAT32 width and V1 ABI: authentic 28-bit entries, chained root, fragmented file reads, corrupt/mismatched/truncated media rejected.
- V3/V4: FatFs backing; full directory/file creation, read/write/truncate, LFN, attributes, timestamps, explicit FAT32 formatting with a preview refusing incorrect variants.
- No destructive test ever targets physical drives: all formatting uses disposable regular-file images.
- Final `.so` must have V1/V3/V4, matching `.ifsmeta`, no ELF runtime dependency, and a package that installs FAT32 only.
- Aggressive build requires nonempty training profiles and two-pass PGO; no speedup can be claimed without benchmarks.
- Unqualified: directory rename, Defragmenter bridge, physical-media interruption/durability, kernel .ko, UEFI .efi, root support, full independent FAT32 conformance.

## Native Linux and EFI source baseline

50 original Linux FAT/VFAT and EDK2 FAT source blobs were imported unchanged from Filesystems commit `0c30a80da199dd552a9f80ff98dea6fb3380b70d` and independently matched against the exact Git blob IDs. Their presence establishes source availability only. The build and publication workflow still validates **only** `FAT32.so`; it does not produce, install, or claim separate `.ko` or `.efi` artifacts.
