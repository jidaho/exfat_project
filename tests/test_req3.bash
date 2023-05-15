#The extfat utility shall accept the -h option. 
#The program will then print the help text on how to execute the program and the allowed options, and then exit.
echo test 2
./extfat -h > /dev/null


if [ $? == 0 ] ; then
result=0;
else 
result=1;
fi
exit $result