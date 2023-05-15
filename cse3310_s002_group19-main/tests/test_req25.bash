./extfat -i test.image -o out.txt -x test.txt

if [ $? -ne 0 ] ; then
result=0
else 
result=1;
fi
echo $result

if [ $result == 0 ] ; then
./extfat -i test.image -D test.txt
result=$?
echo $result
    if [ $result -ne 0 ] ; then
    result=0
    else 
    result=1;
    fi
fi



echo $result
exit $result