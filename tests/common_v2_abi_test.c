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
    if(dlsym(library,"infiltrator_filesystem_defrag_v1") ||
       dlsym(library,"infiltrator_filesystem_api_v1") ||
       dlsym(library,"infiltrator_filesystem_module_v1"))
        return 3;
    void *symbol=dlsym(library,IFS_FS_COMMON_EXPORT_V2);
    IfsFsModuleApiEntryV2 resolve=NULL;
    if(sizeof(symbol)!=sizeof(resolve))return 4;
    memcpy(&resolve,&symbol,sizeof(resolve));
    const IfsFsModuleApiV2 *api=resolve?resolve():NULL;
    if(!ifs_fs_api_v2_valid(api,"fat32"))return 5;
    /* This provider is independently freestanding and has no runtime
     * imports, so firmware hosts must discover it by the V2 profile bit. */
    if ((api->runtime_profiles & (IFS_FS_PROFILE_HOSTED |
         IFS_FS_PROFILE_FREESTANDING | IFS_FS_PROFILE_FIRMWARE)) !=
         (IFS_FS_PROFILE_HOSTED | IFS_FS_PROFILE_FREESTANDING |
          IFS_FS_PROFILE_FIRMWARE)) return 14;
    if(api->group_mask!=(IFS_FS_GROUP_BIT(IFS_FS_GROUP_CORE)|
                         IFS_FS_GROUP_BIT(IFS_FS_GROUP_VOLUME)|
                         IFS_FS_GROUP_BIT(IFS_FS_GROUP_DEFRAG)|
         IFS_FS_GROUP_BIT(IFS_FS_GROUP_ALLOCATION)|
                         IFS_FS_GROUP_BIT(IFS_FS_GROUP_FORMAT_PROFILE)) ||
       (api->runtime_profiles & IFS_FS_PROFILE_KERNEL))
        return 6;
    const IfsFsFormatProfileOpsV2 *profile=(const IfsFsFormatProfileOpsV2 *)
        ifs_fs_group_v2(api,IFS_FS_GROUP_FORMAT_PROFILE);
    IfsFsFormatProfileV2 defaults={.structure_bytes=sizeof(defaults)};
    if(!profile || !profile->get_default ||
       profile->get_default(NULL)!=IFS_FS_INVALID ||
       profile->get_default(&defaults)!=IFS_FS_OK ||
       !ifs_fs_format_profile_v2_valid(&defaults) ||
       defaults.format_variant!=32u || defaults.probe_variant!=32u ||
       defaults.layout!=IFS_FS_LAYOUT_MULTI_EXTENT ||
       defaults.recommended_bytes!=2048ull*1024u*1024u ||
       defaults.flags!=IFS_FS_FORMAT_REQUIRE_CHECK)return 6;
    const IfsFsCoreOpsV2 *core=
        (const IfsFsCoreOpsV2 *)ifs_fs_group_v2(api,IFS_FS_GROUP_CORE);
    const IfsFsVolumeOpsV2 *volume_ops=
        (const IfsFsVolumeOpsV2 *)ifs_fs_group_v2(api,IFS_FS_GROUP_VOLUME);
    const IfsFsDefragOpsV2 *defrag_ops=
        (const IfsFsDefragOpsV2 *)ifs_fs_group_v2(api,IFS_FS_GROUP_DEFRAG);
    const IfsFsAllocationOpsV2 *allocation=(const void*)ifs_fs_group_v2(api,IFS_FS_GROUP_ALLOCATION);
    if(!allocation || allocation->header.capabilities!=IFS_FS_ALLOCATION_MAP || !allocation->map)return 6;
    if(!core || !volume_ops || !core->probe || !core->mount || !core->unmount ||
       !core->stat || !core->directory_entry || !core->read || !core->write ||
       !core->create || !core->remove || !core->rename || !core->truncate ||
       !volume_ops->query || !volume_ops->sync || !volume_ops->format)
        return 7;
    if((volume_ops->header.capabilities &
        (IFS_FS_VOLUME_QUERY|IFS_FS_VOLUME_SYNC|IFS_FS_VOLUME_FORMAT)) !=
        (IFS_FS_VOLUME_QUERY|IFS_FS_VOLUME_SYNC|IFS_FS_VOLUME_FORMAT))
        return 8;
    /* CHECK is now native and must be exported; REPAIR and relocation
     * remain unadvertised until real implementations qualify. */
    if(!(volume_ops->header.capabilities & IFS_FS_VOLUME_CHECK) || !volume_ops->check ||
       (volume_ops->header.capabilities & IFS_FS_VOLUME_REPAIR) ||
       !defrag_ops || defrag_ops->header.capabilities!=IFS_FS_DEFRAG_ALL ||
       !defrag_ops->map || !defrag_ops->plan || !defrag_ops->execute ||
       !defrag_ops->verify || !defrag_ops->recover || !defrag_ops->release_plan)
        return 9;
    if(dlclose(library)!=0)return 10;
    puts("FAT32-STRICT-COMMON-V2-ABI-PASS");
    return 0;
}
