./extfat -i test.image -D /testDir/testDir2/test.txt
./extfat -i test.image -d | grep 'test.txt'

if [ $? != 0 ]; then
    result=0
else
    result=1
fi

exit $result