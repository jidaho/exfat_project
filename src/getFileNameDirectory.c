#include <stdlib.h>
#include "getFileNameDirectory.h"
fileNameDirectory *getFileNameDirectory()
{
    fileNameDirectory *fnd = (fileNameDirectory *)malloc(sizeof(fileNameDirectory));

    fnd->EntryType = 0x85;
    fnd->GeneralSecondaryFlags = 1;
    char *filename = (char *)fnd->FileName;
    strncpy(filename, "test", 30);
    return fnd;
}
