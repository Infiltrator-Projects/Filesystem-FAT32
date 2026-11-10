/* SPDX-License-Identifier: GPL-3.0-or-later */
#include <infiltratr/filesystem_module.h>
#include <dlfcn.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    void *library;
    IfsFsModuleApiEntryV1 entry;
    const IfsFsModuleApiV1 *api;
    if (argc != 2) return 2;
    library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!library) { fprintf(stderr, "%s\n", dlerror()); return 1; }
    *(void **)(&entry) = dlsym(library, IFS_FS_COMMON_EXPORT_V1);
    if (!entry || !(api = entry()) || !ifs_fs_common_api_valid(api, "fat32") ||
        !(api->capabilities & IFS_FS_CAP_PROBE) ||
        !(api->capabilities & IFS_FS_CAP_MOUNT) ||
        !(api->capabilities & IFS_FS_CAP_FORMAT) ||
        (api->capabilities & (IFS_FS_CAP_CHECK | IFS_FS_CAP_RECOVER)) ||
        (api->capabilities & IFS_FS_CAP_DEFRAG_MOVE)) {
        fprintf(stderr, "FAT32 Common ABI contract failed\n");
        dlclose(library);
        return 1;
    }
    puts("FAT32 Common ABI contract passed");
    dlclose(library);
    return 0;
}
