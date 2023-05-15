#./extfat -i test.image -o out.image -c
./extfat -i test.image -D /testDir

echo $?

if [ $? -ne 0 ] ; then
result=0;
else 
result=1;
fi

echo $result

#rm out.image
exit $result