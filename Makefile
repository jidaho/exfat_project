# for this project, there is only one makefile
#
# this makefile is expected to be run from the project 
# root.
#
# all executables are expected to be ran from the project 
# root.
#
# this makefile will clean up when asked politely
#
# No intermediate .o files or libraries are created

#CPPFLAGS=-Wall -Wextra -O0 -std=gnu17 -g3 -fsanitize=address
CPPFLAGS=-Wall -Wextra -O0 -std=gnu17 -g3 -fsanitize=bounds-strict
# note address sanitizer "-fsanitize=address" is new. it can be
# removed from the makefile if it causes problems.

CPPINCS=-I./include

all:mmap fread unit_tests extfat crc_example

# the utility that is a focus of this project
extfat:src/extfat.c common/routines.c src/DiffCheck.c src/commandargs.c src/fileCopy.c src/isEmpty.c src/listFiles.c src/extract.c src/fileDelete.c src/parser.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^


# unit tests
unit_tests: munit_example cluster

# this test needs to be deleted once we get some real tests
# for the problem at hand
munit_example:unit_tests/munit/example.c 
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^
cluster:unit_tests/cluster.c common/cluster.c common/routines.c src/isEmpty.c src/fileCopy.c src/commandargs.c src/DiffCheck.c src/listFiles.c src/getFileType.c src/getFileNameDirectory.c src/extract.c src/fileDelete.c src/parser.c
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^
# requirements tests

system_tests: extfat
	bash tests/system_tests.bash

grading_tests: extfat
	bash tests/grading_tests.bash

# example code
mmap:examples/mmap.c  
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

fread:examples/fread.c  
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

crc_example:examples/crc_example.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^ -lz

# run tests
tests: run_unit_tests run_tests

run_unit_tests: munit_example
	./munit_example

run_tests:
	echo "here i would be running the requirements tests"


clean:
	-rm -f mmap fread munit_example extfat  cluster
