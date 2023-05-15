#The extfat utility shall accept the -i option for the input file. This file will be used for input. It is mandatory.
#req 1
./extfat -i test.image

if [ $? == 0 ] ; then
result=0;
else 
result=1;
fi

exit $result