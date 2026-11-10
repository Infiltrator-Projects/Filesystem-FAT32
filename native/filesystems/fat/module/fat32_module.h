/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Freestanding, read-only FAT32 drop-in: no libc, kernel or EFI imports. */
#ifndef INFILTRATOR_FAT32_MODULE_H
#define INFILTRATOR_FAT32_MODULE_H
#include <stddef.h>
#include <stdint.h>
typedef int (*IfsFat32ReadAt)(void *context, uint64_t offset,
                             void *buffer, uint32_t bytes);
typedef struct {
    IfsFat32ReadAt read_at;
    void *context;
    uint64_t device_bytes;
} IfsFat32Io;
typedef struct {
    uint64_t volume_bytes;
    uint64_t fat_offset;
    uint64_t data_offset;
    uint32_t cluster_bytes;
    uint32_t cluster_count;
    uint32_t sectors_per_fat;
    uint32_t root_cluster;
    uint32_t total_sectors;
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint8_t fat_copies;
} IfsFat32Volume;
typedef struct {
    char name[13];
    uint32_t first_cluster;
    uint32_t size_bytes;
    uint8_t attributes;
} IfsFat32File;
/* Internal implementation procedures: no independently exported ABI table. */
int ifs_fat32_probe(const IfsFat32Io *, IfsFat32Volume *);
int ifs_fat32_root_file(const IfsFat32Io *, const IfsFat32Volume *,
                        uint32_t nth, IfsFat32File *);
int ifs_fat32_read_file(const IfsFat32Io *, const IfsFat32Volume *,
                        const IfsFat32File *, uint64_t, void *, uint32_t);
int ifs_fat32_root_file_fragments(const IfsFat32Io *, const IfsFat32Volume *,
                                  const IfsFat32File *, uint32_t *);
#endif
