#The extfat utility shall accept the -o option for the output file. 
#It is optional. If not specified, the output file is the same as the input file.

./extfat -i test.image -o out.image

if [ $? == 0 ] ; then
result=0;
else 
result=1;
fi
exit $result