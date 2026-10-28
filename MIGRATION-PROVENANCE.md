# FAT32 source and licence provenance

- Authoritative original source: https://github.com/Infiltrator-Projects/Filesystems
- Pinned Filesystems revision: `0c30a80da199dd552a9f80ff98dea6fb3380b70d`
- 50 original editable Linux FAT/VFAT and TianoCore EDK2 EFI source files copied under `native/filesystems/fat/linux/` and `native/filesystems/fat/efi/`; verified with exact matching Git object hashes.
- Existing independently compiled FAT32 V1 reader and portable ChaN FatFs engine retained from the Filesystems code that also appeared in Filesystem-FAT16's working source baseline.
- Original licenses, copyright and attribution are preserved in copied source files, including `linux/COPYING` and `efi/EDK2-LICENSE.txt`.
- No local copy of Infiltratr Common was introduced. The FAT32 `.so` build does not need Common headers.

**Release scope:** the compiled `FAT32.so` is independently qualified. Linux `.ko`, firmware `.efi`, physical media and complete Defragmenter integration are separate, unqualified milestones. Source presence is not executable driver qualification.
