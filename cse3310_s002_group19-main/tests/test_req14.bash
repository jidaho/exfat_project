#8 The extfat utility shall terminate at any point if files fail to open
# 
echo test 14

./extfat -i test.image -o out.image -c

input="test.image"

if [ ! -e "$input" ]; then
    echo "Error: File '$input' is not opwn"
    exit 1
fi

echo "File '$input' is open"
exit 0
