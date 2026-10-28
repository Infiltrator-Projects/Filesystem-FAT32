#!/bin/sh
set -eu
fail() { echo "FAT32-KO-RUNTIME-FAIL: $*" > /dev/ttyS0; poweroff -f; exit 1; }
mount -t proc proc /proc || fail proc
mount -t sysfs sysfs /sys || fail sys
mount -t devtmpfs devtmpfs /dev || fail devtmpfs
insmod /fat32.ko || fail insmod
grep -q '[[:space:]]fat32$' /proc/filesystems || fail registration
for target in /dev/vdb /dev/vdc; do
    before=$(sha256sum "$target")
    if mount -t fat32 "$target" /mnt/test; then
        umount /mnt/test || :
        fail "foreign format mounted"
    fi
    test "$(sha256sum "$target")" = "$before" || fail "foreign media modified"
done
mount -t fat32 /dev/vda /mnt/test || fail mount
mkdir /mnt/test/tree || fail mkdir
echo 'fat32 kernel driver survives remount' > /mnt/test/tree/original.txt || fail write
mv /mnt/test/tree/original.txt '/mnt/test/tree/Renamed Long Name.txt' || fail rename
sync || fail sync
umount /mnt/test || fail umount
mount -t fat32 /dev/vda /mnt/test || fail remount
test "$(cat '/mnt/test/tree/Renamed Long Name.txt')" = 'fat32 kernel driver survives remount' || fail readback
umount /mnt/test || fail final
echo 'FAT32-KO-RUNTIME-PASS' > /dev/ttyS0
poweroff -f
