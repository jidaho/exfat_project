#The extfat utility shall accept the -d option. 
#This option will print all files and directories
echo test 17

sync
./extfat -i test.image -d > /dev/null


if [ $? == 0 ] ; then
echo "PASS"
result=0;
else 
result=1;
fi
exit $result
