# FAT32 shared-object qualification (0.1.3)

The production `FAT32.so` owns all FAT32-specific semantics. BOOT and
Defragmenter consume that exact module through reviewed versioned interfaces.
The standalone `.ko` and `.efi` are preserved transitional adapters; the
eventual kernel-side shared-object loader is not yet implemented.

## Proven source and functions

- Original FAT32 V1 uses authentic FAT32 BPB, 28-bit entries, cluster-chain root,
  fragmented-file reads and strict foreign-width/malformed-image rejection.
- FatFs V3/V4 owns format, directory/file create/read/write/truncate, long names,
  attributes, timestamps, volume space and explicit flush.
- The original FAT12/FAT16/FAT32 native Defragmenter engine is compiled directly
  into FAT32.so. It owns FAT32 relocation, exact 10% Growth Defrag, journals and
  recovery. The production worker provides only generic raw I/O, target safety
  and durable journal host services.
- Generic Boot V1 uses the same FatFs state, read-only partition-relative I/O,
  a checked `IFSBOOT1` manifest, relative relocations and no runtime imports.
  Boot's own released ELF/UEFI implementation is not forked.
- Source-embedded native RUN includes ABI headers, the defragmenter engine and
  required source/LICENSE data. Native GCC PGO/LTO must preserve zero runtime imports.

## Mandatory release jobs

1. `qualify`: portable and native module exports, `.ifsmeta`, native installer
   exact hashes, format/file lifecycle, dosfstools and mtools independent readback.
2. `defrag-consumer`: compile the exact pinned Defragmenter production child host,
   call the standalone FAT32.so, verify real FAT32 Growth Defrag and staged
   interruption recovery including wrong-target rejection and no media corruption.
   Check independently formatted clean FAT32 media using fsck and use the
   independent Growth fixture's exact allocation/content oracle.
3. `boot-consumer`: both portable/native FAT32.so through unchanged Boot ELF/UEFI
   bridge and real Linux kernel/initramfs under OVMF, with malformed-module
   rejection proving no fallback to firmware's native FAT driver.
4. `native-efi` and `native-kernel`: independent EFI writable OVMF qualification
   and Linux 6.12.107-specific KO read/write/remount QEMU qualification.
5. Only publish an immutable new tag when these jobs pass at the exact main
   commit. No source qualification action touches a physical partition or ESP.

The historical `make_growth_defrag_image.py` fixture intentionally predates
clean FAT32 label/FSINFO metadata. Its independent allocation and file-data
checks run alongside a separately formatted, fsck-clean FAT32 image. Known
preexisting label/FSINFO warnings are not reclassified as data corruption.

## Not yet certified

Generic kernel-side SO loading/mount/root, arbitrary host kernel ABIs, Secure
Boot signing, real removable media and original firmware interoperability,
power-loss atomicity, full FAT32 fuzzing, all directory-rename ancestry edge
cases and performance claims need independent qualification. A passing OVMF or
QEMU run does not certify every physical device.
