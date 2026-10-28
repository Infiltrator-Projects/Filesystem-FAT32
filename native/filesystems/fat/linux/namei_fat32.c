// SPDX-License-Identifier: GPL-2.0-only
// Compile the original Linux VFAT namespace logic as a FAT32-only module.
#define IFS_FAT_ONLY 32
#include "namei_vfat.c"
