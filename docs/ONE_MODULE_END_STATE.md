# FAT32: one authoritative filesystem engine

`/System/Filesystems/FAT32.so` supplies the full FAT32 filesystem engine
and versioned read/write/format, Defragmenter and generic Boot interfaces.

Boot loads and executes its restricted freestanding read-only interface through
the already-released generic ELF/UEFI bridge, tested with real Linux handoff.
Defragmenter loads the same installed module; its generic host does not own
FAT32 placement, allocation, journal or recovery. Test Media must likewise
call the module V4/V3 to format/populate rather than constructing FAT32 entries.

The Linux kernel currently uses its independently qualified, kernel-pinned
transitional `fat32.ko`, and a separate native `FAT32.efi` remains available.
They are not a permanent three-driver architecture. The final generic kernel
SO bridge, including safe early-root mount, signed-module policy, host ABI
validation, hardware and physical media are still separate deliverables.
