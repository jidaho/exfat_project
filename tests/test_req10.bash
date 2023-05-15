#The extfat utility shall terminate if the copy flag was used and the same file was entered for both the input and output file
echo test 4
#output=$(./extfat -i test.image -o test.image -c)

#echo  "$output"
./extfat -i test.image -o test.image -c > /dev/null

if [ $? -ne 0 ] ; then
result=0;
else 
result=1;
fi
exit $result