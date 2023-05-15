#8 The extfat utility shall accept two char* values.
echo test 5
./extfat -i test.image -o out.image

if [ $? == 0 ] ; then
result=0;
else
result=1;
fi

exit $result
