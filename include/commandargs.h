#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct commandopts
{
    char *inputfile;
    char *outputfile;
    bool copy;
    bool verifyints;
} commandopts;

struct commandopts commandargs(int argc, char *argv[]);