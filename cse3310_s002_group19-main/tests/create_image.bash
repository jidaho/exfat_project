echo creating test.image
dd if=/dev/zero of=test.image count=1 bs=1M
sudo losetup /dev/loop2 test.image
sudo /usr/sbin/mkexfatfs /dev/loop2

# put something in the file system image
mkdir /tmp/d
sudo mount /dev/loop2 /tmp/d
cp examples/mmap.c /tmp/d
mkdir -p /tmp/d/testDir/testDir2
echo "testing" > /tmp/d/testDir/testDir2/test.txt