# FAT32 source and licence provenance

- Authoritative original source: https://github.com/Infiltrator-Projects/Filesystems
- Pinned Filesystems revision: `0c30a80da199dd552a9f80ff98dea6fb3380b70d`
- 50 original editable Linux FAT/VFAT and TianoCore EDK2 EFI source files copied under `native/filesystems/fat/linux/` and `native/filesystems/fat/efi/`; verified with exact matching Git object hashes.
- Existing independently compiled FAT32 V1 reader and portable ChaN FatFs engine retained from the Filesystems code that also appeared in Filesystem-FAT16's working source baseline.
- Original licenses, copyright and attribution are preserved in copied source files, including `linux/COPYING` and `efi/EDK2-LICENSE.txt`.
- The new canonical defragmentation engine requires narrowly scoped Common headers; a pinned, source-owned snapshot under `src/infiltratr-common/include/` and its GPL-3 licence are retained for independent source-embedded native RUN reproducibility. This does not license divergent Common semantics.

**Release scope (0.1.3):** the compiled `FAT32.so` includes the recovered complete FAT32 defragmentation engine and generic Boot V1 bridge. Independent UEFI and pinned Linux KO runtime binaries have their own VM qualifications. Generic kernel SO loading and physical-media qualification remain open. Source presence alone is not executable-driver qualification.

The 25 recovered FAT relocation/defragmentation source files were compared against Filesystems commit `7473534b1afbd1e9b14f8ab9ae0769fc6be2126e`: 24 match byte-for-byte by Git blob SHA. The only newer file, `fat_volume.c`, matches the FAT16 canonical engine source at its current main revision.
