#The program shall terminate if any flag besides those previously listed are inputted
echo test 1
./extfat -f

if [ $? -ne 0 ] ; then
result=0;
else 
result=1;
fi
exit $result