sudo -v
mkdir -p fs_mount
losetup /dev/loop0 disk.img
mount /dev/loop0 fs_mount/
