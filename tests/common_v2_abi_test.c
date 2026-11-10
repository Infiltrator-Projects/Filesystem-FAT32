/* SPDX-License-Identifier: GPL-3.0-or-later
 * Verify one strict Common V2 table, no obsolete Common V1 entry point.
 */
#include <infiltratr/filesystem_module_v2.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
    if(argc!=2)return 2;
    void *library=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);
    if(!library){fprintf(stderr,"%s\n",dlerror());return 1;}
    if(dlsym(library,"infiltrator_filesystem_api_v1") ||
       dlsym(library,"infiltrator_filesystem_module_v1"))
        return 3;
    void *symbol=dlsym(library,IFS_FS_COMMON_EXPORT_V2);
    IfsFsModuleApiEntryV2 resolve=NULL;
    if(sizeof(symbol)!=sizeof(resolve))return 4;
    memcpy(&resolve,&symbol,sizeof(resolve));
    const IfsFsModuleApiV2 *api=resolve?resolve():NULL;
    if(!ifs_fs_api_v2_valid(api,"fat32"))return 5;
    if(api->group_mask!=(IFS_FS_GROUP_BIT(IFS_FS_GROUP_CORE)|
                         IFS_FS_GROUP_BIT(IFS_FS_GROUP_VOLUME)) ||
       (api->runtime_profiles & (IFS_FS_PROFILE_KERNEL|IFS_FS_PROFILE_FIRMWARE)))
        return 6;
    const IfsFsCoreOpsV2 *core=
        (const IfsFsCoreOpsV2 *)ifs_fs_group_v2(api,IFS_FS_GROUP_CORE);
    const IfsFsVolumeOpsV2 *volume_ops=
        (const IfsFsVolumeOpsV2 *)ifs_fs_group_v2(api,IFS_FS_GROUP_VOLUME);
    if(!core || !volume_ops || !core->probe || !core->mount || !core->unmount ||
       !core->stat || !core->directory_entry || !core->read || !core->write ||
       !core->create || !core->remove || !core->rename || !core->truncate ||
       !volume_ops->query || !volume_ops->sync || !volume_ops->format)
        return 7;
    if((volume_ops->header.capabilities &
        (IFS_FS_VOLUME_QUERY|IFS_FS_VOLUME_SYNC|IFS_FS_VOLUME_FORMAT)) !=
        (IFS_FS_VOLUME_QUERY|IFS_FS_VOLUME_SYNC|IFS_FS_VOLUME_FORMAT))
        return 8;
    if(volume_ops->header.capabilities & (IFS_FS_VOLUME_CHECK|IFS_FS_VOLUME_REPAIR) ||
       ifs_fs_group_v2(api,IFS_FS_GROUP_DEFRAG))
        return 9;
    if(dlclose(library)!=0)return 10;
    puts("FAT32-STRICT-COMMON-V2-ABI-PASS");
    return 0;
}
