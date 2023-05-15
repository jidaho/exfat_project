#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "getFileType.h"

// A function that takes a file directory entry and returns a string indicating the file type
char** getFileType()
{
    fileDirectoryEntry* entries = (fileDirectoryEntry*) malloc(sizeof(fileDirectoryEntry));
    //fileDirectoryEntry entries[2];
    entries[0].EntryType = 0;
    entries[0].SecondaryCount = 0;
    entries[0].SetCheckSum = 0;
    entries[0].FileAttributes = 20;
    entries[0].Reserved1 = 0;
    entries[0].MSC[0] = 0;

    entries[1].EntryType = 0;
    entries[1].SecondaryCount = 0;
    entries[1].SetCheckSum = 0;
    entries[1].FileAttributes = 10;
    entries[1].Reserved1 = 0;
    entries[1].MSC[0] = 0;
    static char *stringArray[2];
    for (int i = 0; i < 2; i++)
    {
        if (entries->FileAttributes == 20)
        {
            
            stringArray[i]="File";
        }
        else if (entries->FileAttributes == 10)
        {
            stringArray[i]="Directory";            
        }
    }
    return stringArray;
}