# FAT32 qualification contract

The [original-format reference](FORMAT_SPECIFICATION.md) defines media invariants independently of current implementation scope. [Common V2 architecture](ONE_MODULE_END_STATE.md) governs all current consumers. [Historical 0.1.3 validation evidence](https://github.com/Infiltrator-Projects/Filesystem-FAT32/blob/d1e919033f2713bff3df8d9ca20e1f8aa575f85e/docs/VALIDATION.md) is preserved at the prior revision, not a claim that its legacy interfaces or drivers are still part of the product.

## Required current evidence

- Validate exact packaged FAT32.so identity, exclusive Common V2 export, supported operation masks, native and portable profiles, installed PDF and source-downloading native RUN. Confirm no independent per-format KO/EFI payload or legacy interfaces.
- Verify BPB/FSInfo geometry, masked high nibble, FAT mirroring/active selection, root and directory lifecycle, file bytes, corruption rejection, short I/O and media unchanged under read-only access. Use independent dosfstools/mtools images and checks as oracles.
- Defragmenter and Test Media invoke the actual V2 operations on disposable images; prove allocation, preserved bytes, relocation, 10% Growth Defrag if supported, interruption and target-bound recovery separately. Unsupported operations must refuse before writing.
- BOOT and kernel must qualify exact hosted/freestanding module bytes through their generic loaders, with real kernel handoff and mount/read/write/root tests as applicable. Prior OVMF and separate KO tests cannot establish current integration or physical-device durability.

The [release workflow](../.github/workflows/release.yml) defines executable gates. No new CI or media qualification is asserted by this documentation correction. Historic tags are immutable.
