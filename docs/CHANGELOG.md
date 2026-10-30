# Filesystem FAT32 changelog

## 0.1.3 — 2026-10-30 (recover and qualify original engine)

- Recovered all 25 canonical defragmentation, relocation, journaling, allocation and
  recovery sources previously developed in Filesystems/FAT16.
- Added versioned Defragmenter ABI and the tested production worker consumer.
- Added the generic read-only Boot SO interface with actual OVMF Linux handoff
  and malformed-module/no-firmware-fallback acceptance tests.
- Preserved correct native PGO/LTO freestanding intrinsics and bundled ABI
  and Common licence source with the native RUN.
- Separate kernel-specific and EDK2 EFI compatibility drivers remain VM-qualified;
  the universal kernel SO bridge and physical hardware tests remain open.

## 0.1.2 — 2026-10-28 (independent FAT32 SO plus native adapters)

- Independent FAT32 read/write/format SO, native EFI and pinned-kernel KO
  qualification and release.
