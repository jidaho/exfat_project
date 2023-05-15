#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include "extfat.h"
#include "listFiles.h"

#define cluster(N) ((fp + MB->ClusterHeapOffset * bytesPerSector) + ((N - 2) * bytesPerSector * sectorsPerCluster))

// struct for holding the different entry types for each directory or file
// I will probably move these structs into a different header file for the next iteration if they are needed
// for extracting the data in the files.
typedef struct
{
   union
   {
      uint8_t EntryType;
      struct
      {
         uint8_t TypeCode : 5;
         uint8_t TypeImportance : 1;
         uint8_t TypeCategory : 1;
         uint8_t InUse : 1;
      };
   };
   uint8_t CustomDefined[19];
   uint32_t FirstCluster;
   uint64_t DataLength;
} GenericDirectoryStruct;

// this is the first struct for a file entry, struct holds the file attributes which
// identify the file as either a directory or a file
typedef struct fileDirectoryEntry
{
   uint8_t EntryType : 8;
   uint8_t SecondaryCount : 8;
   uint16_t SetCheckSum : 16;
   uint16_t FileAttributes : 16; // will be 20 if file, and 10 if directory
   uint16_t Reserved1 : 16;
   uint8_t MSC[24];
} fileDirectoryEntry;

// the immediately following entry is the file stream extension which contains the location of the data stored in
// a directory under the 'First Cluster' attribute
typedef struct fileStreamExtension
{
   uint8_t EntryType : 8;
   uint8_t MSC[19];
   uint32_t FirstCluster : 32;
   uint64_t DataLength : 64;
} fileStreamExtension;

// contains the name of the file
typedef struct fileNameDirectory
{
   uint8_t EntryType : 8;
   uint8_t GeneralSecondaryFlags : 8;
   uint8_t FileName[30];
} fileNameDirectory;

// this function recursively traverses through the file system, finding all directories and files starting from the
// root directory.
// dir dive takes the cluster number, the file handle and MB as well as the bytes Per Sector and sectors per cluster that are
// defined in Main
// The dive depth variable is simply a formatting variable that will tab out based on the depth of the recursion, so
// the directory hierarchy will be visually easy to see.
int dirDive(int clusterNum, void *fp, Main_Boot *MB, int bytesPerSector, int sectorsPerCluster, int divedepth)
{
   GenericDirectoryStruct *GDS = cluster(clusterNum);
   int i = 0;

   while (GDS[i].EntryType)
   {
      // A file/directory have 3 required entries: a file directory entry, a file stream extension, and a file name directory
      // the file directory entry starts with 0x85
      if (GDS[i].InUse && GDS[i].EntryType == 0x85)
      {
         // we are at a file directory entry and need to check if it is a normal file or a directory:
         struct fileDirectoryEntry *FDE = (struct fileDirectoryEntry *)&GDS[i];
         // a directory's file attributes will be 0x10
         if (FDE->FileAttributes == 0x10)
         {
            // we need the file stream extension of a directory to find the next cluster
            i++;
            struct fileStreamExtension *FSE = (struct fileStreamExtension *)&GDS[i];
            // we need the name of the directory
            i++;
            struct fileNameDirectory *FND = (struct fileNameDirectory *)&GDS[i];
            // formatting loop
            for (int d = 0; d < divedepth; d++)
            {
               printf("\t");
            }
            printf("Directory:  ");
            if (FND->EntryType == 0xc1)
            {
               char *ch = (char *)FND->FileName;
               while (*ch)
               {
                  printf("%c", *ch);
                  ch++;
                  ch++;
               }
               printf("\n");
            }
            if (FSE->EntryType == 0xc0)
            {

               dirDive(FSE->FirstCluster, fp, MB, bytesPerSector, sectorsPerCluster, divedepth + 1);
            }
         }
         // this is a normal file, and we just need the name of it
         else if (FDE->FileAttributes == 0x20)
         {

            i++; // skip the file stream extension
            i++; // i is now pointing to the file name directory
            struct fileNameDirectory *FND = (struct fileNameDirectory *)&GDS[i];
            for (int d = 0; d < divedepth; d++)
            {
               printf("\t");
            }
            printf("File:       ");
            if (FND->EntryType == 0xc1)
            {
               char *ch = (char *)FND->FileName;

               while (*ch)
               {
                  printf("%c", *ch);
                  ch++;
                  ch++;
               }
               printf("\n");
            }
         }
         else
         {
            printf("invalid value\n");
         }
      }
      i++;
   }
   return 0;
}

int listFiles(char *inputFile)
{
   // open the file system image file
   int fd = open(inputFile, O_RDWR);
   if (fd == -1)
   {
      perror("file open: ");
      exit(0);
   }

   off_t size = 0;
   struct stat statbuf;
   if (fstat(fd, &statbuf))
   {
      perror("stat of file:");
      exit(0);
   }
   size = statbuf.st_size;

   // mmap the entire file into memory
   // every data item we reference, will be relative to fp...
   void *fp = (void *)mmap(NULL,
                           size,
                           PROT_READ,
                           MAP_PRIVATE,
                           fd,
                           0); // note the offset

   if (fp == (void *)-1)
   {
      perror("mmap:");
      exit(0);
   }

   Main_Boot *MB = (Main_Boot *)fp;

   int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
   int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1);

   // calls the directory dive function written above
   dirDive(MB->FirstClusterOfRootDirectory, fp, MB, bytesPerSector, sectorsPerCluster, 0);

   // unmap the file
   if (munmap(fp, size))
   {
      perror("error from unmap:");
      exit(0);
   }

   // close the file
   if (close(fd))
   {
      perror("close:");
   }
   fd = 0;

   return 0;
}
