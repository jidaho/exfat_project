# clean up
echo clean up
sudo umount /tmp/d
sudo losetup -d /dev/loop2
rm -rf /tmp/d
rm test.image
rm out.image