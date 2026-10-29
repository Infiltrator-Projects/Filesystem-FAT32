# FAT32 defragmentation source ownership

The source files in `native/filesystems/fat/defrag/` are original source-code imports from `Infiltrator-Projects/Filesystems` commit `7473534b1afbd1e9b14f8ab9ae0769fc6be2126e`, including FAT32-specific layout, allocation, recovery and format-specific defragmentation worker code.

They belong in **Filesystem-FAT32**, not only in Filesystems or Defragmenter. The `Common`/Libraries public headers remain an external build dependency pinned for qualification, not an embedded copy. This import initially qualifies full native linking and the `infiltrator_filesystem_defrag_v1` export; a successful link is not permission to replace the published FAT32.so until media mutation/recovery regression tests also pass.

The existing v0.1.2 FAT32.so release and its source-embedded installer remain unchanged. This branch changes no system, root, EFI, or physical device.
