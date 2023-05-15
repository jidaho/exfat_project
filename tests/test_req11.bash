#11 The extfat utility shall terminate at any point if the utility fails to
# read the original given file.

echo test 11

input="test.image"

if ! [ -r "$input" ]; then
  echo "Error: The input file '$input' cannot be read"
  exit 1
fi

./extfat -i test.image -o out.image -c
status=$?

if [ $status -ne 0 ]; then
  echo "Error: Failed to read original file"
  exit 1
fi

echo "Test 11 passed successfully"
exit 0


