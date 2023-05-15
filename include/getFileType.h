#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef struct fileDirectoryEntry {
    uint8_t EntryType : 8;
    uint8_t SecondaryCount : 8;
    uint16_t SetCheckSum : 16;
    uint16_t FileAttributes : 16; 
    uint16_t Reserved1 : 16;
    uint8_t MSC[24];
} fileDirectoryEntry;

char** getFileType();