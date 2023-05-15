#The extfat utility shall accept the -v option. 
#This means verify. An input file is required. 
#The main and backup boot sectors will be read and checked to see if they are the same. 
#Output the both datas by using this format, "Checksum  (MB) %x (BBR) %x".

./extfat -i test.image -v

if [ $? == 0 ] ; then
result=0; 
echo success in test 6
else 
echo error in test 6
result=1;
fi
exit $result