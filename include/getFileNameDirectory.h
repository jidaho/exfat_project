#include <stdint.h>
typedef struct fileNameDirectory {
    uint8_t EntryType : 8;
    uint8_t GeneralSecondaryFlags : 8;
    uint8_t FileName[30];
} fileNameDirectory;
#include <stdio.h>
#include <string.h>

fileNameDirectory* getFileNameDirectory();
