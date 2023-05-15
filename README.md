extfat utils is a utility to manipulate extfat images.

# Command Options:
-i    (required) Input File - the next command line argument must be the desired input file, if not included the program will close.

-o    (optional) Output File - the next command line argument must be the desired output file, if not included the file name that was used for\n
                 the input file will be copied over to the output file.  If used but no valid file is included, the input file will be copied to the 
                 output file.
                 
-h    (optional) Help - display the help text.  If used as a command line argument, the program will display the help and close.

-c    (optional) Copy - the input file will be copied to the output file without change.

-v    (optional) Verify - the main and backup boot sectors will be checked, if they are not the same, an error message will be printed.

-d    (optional) Display File System - Displays the current contents of the file system, including all directories and files

-x    (optional) Extract - Extracts the contents of a file in the file system onto a designated output file.

-D    (optional) Delete - Deletes a specified file from the file system.




# Example Invocations:
./extfat -i test.image -o test2.image -c
        The content in test.image will be copied to test2.image
./extfat -i test.image -o -c
        The content in test.image will be copied into test.image as no valid output file was provided.
./extfat -v -i test.image -o test2.image -c
        The main and back up boot sectors will be verified, and if they are the same, test.image will be copied into test2.image.
        If not the same, and error message will be printed and the program will close.
./extfat -i test.image -o -c -h
        The help menu will be displayed on the screen and the program will exit before executing the desired commands.
./extfat -h
        The help menu will be displayed and the program will exit.


# Internals

## Build Process
```bash
% make clean
% make

To run the tests
% make tests

```
## Unit Test
% ./cluster

## Examples
There is a directory of examples.  They are compiled by default using the makefile
Before you can run the example programs, you need to have an file image.

```bash
% bash /examples/create_image.bash
% ./examples/fread
% ./examples/mmap
```






















## Misc notes, eventually delete.
```
dd if=/dev/zero of=zzz count=1 bs=1G
yum install -y exfat-utils fuse fuse-exfat
losetup /dev/loop0 diskimage 
mount /dev/loop0 /mnt
umount /mnt
```
## References


https://pawitp.medium.com/notes-on-exfat-and-reliability-d2f194d394c2


https://learn.microsoft.com/en-gb/windows/win32/fileio/exfat-specification


https://uta.service-now.com/selfservice?id=ss_kb_article&sys_id=KB0011414


https://nemequ.github.io/munit/#download


https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html


https://www.freecodecamp.org/news/how-to-use-git-and-github-in-a-team-like-a-pro/


https://en.wikipedia.org/wiki/ExFAT


https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
