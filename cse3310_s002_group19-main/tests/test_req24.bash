./extfat -i test.image -o out.txt -x /testDir/testDir2/test.txt



#input = $(/usr/bin/md5sum out.txt | cut -c -32)
#input2 = $(/usr/bin/md5sum out2.txt | cut -c -32)
input=$(/usr/bin/md5sum out.txt)
input="${input%% *}" #should remove spaces and filename from md5sum

./extfat -i test.image -o out2.txt -x /testDir/testDir2/test.txt
input2=$(/usr/bin/md5sum out2.txt)
input2="${input2%% *}"

echo $input
echo $input2

if [ "$input" == "$input2" ] ; then
result=0;
else 
result=1;
fi

rm out.txt
rm out2.txt

echo $result

exit $result