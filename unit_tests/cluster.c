#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include "fileCopy.h"
#include "DiffCheck.h"
#include "isEmpty.h"
#include "munit.h"
#include "cluster.h"
#include "fileCopy.h"
#include "routines.h"
#include "commandargs.h"
#include "listFiles.h"
#include "getFileType.h"
#include "getFileNameDirectory.h"
#include "extract.h"
#include "fileDelete.h"
#include "parser.h"

static MunitResult
GetFileNameDirectoryTest(const MunitParameter params[], void *data)
{
   // silence compiler warnings
  (void)params;
  (void)data; 
  fileNameDirectory* fnd = (fileNameDirectory*) malloc(sizeof(fileNameDirectory));
  fnd=getFileNameDirectory();
  munit_assert_int(fnd->EntryType,==,0x85);
  munit_assert_int(fnd->GeneralSecondaryFlags,==,1);
  munit_assert_string_equal((char *)fnd->FileName, (const char *)"test");
  free(fnd);
  return MUNIT_OK;
}
static MunitResult
GetFileTypeTest(const MunitParameter params[], void *data)
{
   // silence compiler warnings
  (void)params;
  (void)data; 
  char **array = getFileType();
  munit_assert_string_equal(array[0], "File");
  munit_assert_string_equal(array[1], "File");
  return MUNIT_OK;
}

static MunitResult
CommandArgsTest(const MunitParameter params[], void *data)
{
  // silence compiler warnings
  (void)params;
  (void)data;
  int argc = 5;

  char *argv[] = {"./cluster", "-i", "FileName", "-v", "-c"};
  struct commandopts S = commandargs(argc, argv);
  char *FileName = "FileName";
  munit_assert_string_equal(S.inputfile, FileName);
  munit_assert_string_equal(S.outputfile, FileName);
  munit_assert_int(S.copy, ==, true);
  munit_assert_int(S.verifyints, ==, true);

  return MUNIT_OK;
}

static MunitResult
DiffCheckTest(const MunitParameter params[], void *data)
{
  // silence compiler warnings
  (void)params;
  (void)data;
  Main_Boot MB;
  MB.BytesPerSectorShift = 9;
  MB.SectorsPerClusterShift = 3;
  MB.ClusterHeapOffset = 328;
  Main_Boot BB;
  BB.BytesPerSectorShift = 9;
  BB.SectorsPerClusterShift = 3;
  BB.ClusterHeapOffset = 328;
  // if (MB->BytesPerSectorShift != BB->BytesPerSectorShift)
  munit_assert_int(MB.BytesPerSectorShift, ==, BB.BytesPerSectorShift);
  // if (MB->SectorsPerClusterShift != BB->SectorsPerClusterShift)
  munit_assert_int(MB.SectorsPerClusterShift, ==, BB.SectorsPerClusterShift);
  // if (MB->ClusterHeapOffset != BB->ClusterHeapOffset)
  munit_assert_int(MB.ClusterHeapOffset, ==, BB.ClusterHeapOffset);

  return MUNIT_OK;
}

static MunitResult
IsEmptyTest(const MunitParameter params[], void *data)
{
  // silence compiler warnings
  (void)params;
  (void)data;
  char array[] = "-";
  char array2[] = "1";
  int size = sizeof(array) / sizeof(char);
  // printf("isEmpty(array, size): %d\n", isEmpty(array, size));
  // printf("isEmpty(array, size): %d\n", isEmpty(array2, size));
  munit_assert_int(isEmpty(array, size), ==, true);
  munit_assert_int(isEmpty(array2, size), ==, false);

  return MUNIT_OK;
}
static MunitResult
clusterTest(const MunitParameter params[], void *data)
{
  // silence compiler warnings
  (void)params;
  (void)data;

  // set up input data
  Main_Boot MB;
  MB.BytesPerSectorShift = 9;
  MB.SectorsPerClusterShift = 3;
  MB.ClusterHeapOffset = 328;

  // check for an expected NULL
  void *result = cluster(&MB, 0);
  munit_assert_ptr_equal(result, NULL);

  result = cluster(&MB, 1);
  munit_assert_ptr_equal(result, NULL);

  result = cluster(NULL, 0);
  munit_assert_ptr_equal(result, NULL);

  // some valid values
  result = cluster(&MB, 2);
  munit_assert_ptr_equal((void *)&MB + 0x29000, result);

  /*
  cluster  2 heap starts at 0x7f294d629000 relative 29000
  cluster  3 heap starts at 0x7f294d62a000 relative 2a000
  cluster  4 heap starts at 0x7f294d62b000 relative 2b000
  cluster  5 heap starts at 0x7f294d62c000 relative 2c000
  cluster  6 heap starts at 0x7f294d62d000 relative 2d000
  cluster  7 heap starts at 0x7f294d62e000 relative 2e000
  */

  return MUNIT_OK;
}
/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
    {(char *)"cluster", clusterTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},    
    {(char *)"DiffCheck", DiffCheckTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"IsEmpty", IsEmptyTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"CommandArgs", CommandArgsTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},    
    {(char *)"GetFileTypeTest", GetFileTypeTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"GetFileNameDirectory", GetFileNameDirectoryTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite = {
    (char *)"",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
  return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}