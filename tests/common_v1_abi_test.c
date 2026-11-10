/* SPDX-License-Identifier: GPL-3.0-or-later */
#include <infiltratr/filesystem_module_v2.h>
#include <dlfcn.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    void *library;
    IfsFsModuleApiEntryV1 entry;
    IfsFsModuleApiEntryV2 entry2=NULL;
    const IfsFsModuleApiV2 *v2;
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
    *(void **)(&entry2)=dlsym(library,IFS_FS_COMMON_EXPORT_V2);
    v2=entry2?entry2():NULL;
    if(!ifs_fs_api_v2_valid(v2,"fat32") ||
       v2->base_v1 != api ||
       ifs_fs_group_v2(v2,IFS_FS_GROUP_VOLUME)==NULL ||
       ifs_fs_group_v2(v2,IFS_FS_GROUP_HANDLES)!=NULL ||
       (v2->runtime_profiles & IFS_FS_PROFILE_KERNEL)) {
        fputs("Common V2 contract failed\\n",stderr);
        dlclose(library);return 1;
    }
    puts("FAT32 Common V1/V2 contract passed");
    dlclose(library);
    return 0;
}
