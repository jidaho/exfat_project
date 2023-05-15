#The extfat utility shall accept the -c option. 
#This option denotes copy, where the input file is copied to the output file without change.
echo test 1,2,4
./extfat -i test.image -o out.image -c
input=$(/usr/bin/md5sum test.image)
output=$(/usr/bin/md5sum out.image)
input="${input%% *}" #should remove spaces and filename from md5sum
output="${output%% *}"
if [ "$input" == "$output" ] ; then
result=0;
else 
result=1;
fi
exit $result