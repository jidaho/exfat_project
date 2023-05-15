/*
This function recursively parses through the file system and deletes the file specified.  Its requirements are:
1) -D command will delete the proceeding file listed by labeling it in the exfat system as not in use
2) -D command will not delete the contents of that file, but will only mark them as available for use
3) -D command will only accept a file to be deleted, if a directory is entered an error message will display and the program will close
4) -D command will accept a file in a subdirectory if entered in the notation /directory1/directory2/directory3/filename
5) -D and -x command will accept a file in a subdirectory if entered in the notation directory1/directory2/directory3/filename
6) -All other notations of entering a file in a subdirectory will result in an error message and the program will close.

Requirements 6 and 3 have not yet been fully implemented, -D won't delete a directory but no error handling has been 
put in yet.

I'm sure this code has some bugs as well, probably the same bug as extract.c (can't handle a single file that starts with a /)
One thing that I'm not sure is a bug or not?
   After the file has been deleted (labeled as not in use) it still exists in /tmp/d and therefore another file of that name cannot
   be added to the extfat program

Things that could be done if there is time:
move the structs into their own header file
I think there is a good argument that -D should be able to remove directories as well, and that code could be implemented with some
work

*/


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
#include "extract.h"
#include <string.h>
#include "fileCopy.h"
#include "fileDelete.h"
#include "parser.h"


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

//
//
//
//
//
//
//
//
//
//


void dirdiveRemove(int clusterNum, void *fp, Main_Boot *MB, int bytesPerSector, 
                                          int sectorsPerCluster, char* removeFile)
{
   GenericDirectoryStruct *GDS = cluster(clusterNum);
   int i = 0;
   int j = 0;
   //char fileName[15];   
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
            //malloc a variable for the name of the current GDE so that it can be compared
            char* filesss = malloc(16*sizeof(char));
            memset(filesss, 0, 15 * sizeof(char));
            if (FND->EntryType == 0xc1)
            {
               char *ch = (char *)FND->FileName;

               while (*ch)
               {

                  strcat(filesss, ch);
                  ch++;
                  ch++;
               }


            }

            //for the same reason as with extract.c, its important to separately malloc a variable to hold
            //a temporary file path so that if the current GDE is not the directory in the filepath, parser doesn't modify
            //the filepath and render it unusable 
            char* tempFilePath = malloc(strlen(removeFile)+1);
            strcpy(tempFilePath, removeFile);
            char* next = parser(tempFilePath);

            //strcmp, if it equals zero then we are at the directory listed in the filepath, so update the file path and then 
            //move into the directory
            //If this compare fails, nothing more needs to be done.
            if (strcmp(filesss, next) == 0)
            {
               removeFile = tempFilePath;
               dirdiveRemove(FSE->FirstCluster, fp, MB, bytesPerSector, sectorsPerCluster, removeFile);
            }
            free(filesss);
            free(tempFilePath);

         }
      //this is a normal file, and we need to check if it is the file specified by -D
      else if (FDE->FileAttributes == 0x20)
      {
         j=i;

         i++; //skip the file stream extension as it is not needed 
         i++; // i is now pointing to the file name directory
         struct fileNameDirectory *FND = (struct fileNameDirectory *)&GDS[i];
         if (FND->EntryType == 0xc1)
         {
            //same logic as written in the directory segment above
            char *ch = (char *)FND->FileName;
            char* filesss = malloc(strlen(removeFile));
            memset(filesss, 0, 15 * sizeof(char));
            while (*ch)
            {
               strcat(filesss, ch);
               ch++;
               ch++;
            }
            //if the string matches, remove the file and close the program
            if (strcmp(filesss, removeFile) == 0)
            {

               GDS[j].InUse = (uint8_t) 0;
               printf("File has been successfully removed\nProgram will now close.\n");
               exit(0);

            }
            free(filesss);
         }
      }
      else
         {
            printf("No file of the name %s found.\n", removeFile);
            exit(1);
         }
      }
   i++;
   

 
   }

   
   printf("The file designated for deletion does not exist in the system.\n");
   exit(1);
   

}





int fileDelete(char* imageName, char *deleteFile)
{
   // open the file system image file
   int fd = open(imageName, O_RDWR);
   if (fd == -1)
   {
      perror("file open: ");
      exit(1);
   }

   off_t size = 0;
   struct stat statbuf;
   if (fstat(fd, &statbuf))
   {
      perror("stat of file:");
      exit(1);
   }
   size = statbuf.st_size;

   // mmap the entire file into memory
   // every data item we reference, will be relative to fp...
   // it is very important to have both PROT_WRITE and MAP_SHARED here
   //PROT_WRITE allows the program to view and modify the file system
   //MAP_SHARED will ensure that any changes made by the program will be saved to the actual file system
   //MAP_PRIVATE will just reset any changes made when the program closes
   void *fp = (void *)mmap(NULL,
                           size,
                           PROT_WRITE,
                           MAP_SHARED,
                           fd,
                           0); // note the offset

   if (fp == (void *)-1)
   {
      perror("mmap:");
      exit(1);
   }

   Main_Boot *MB = (Main_Boot *)fp;

   int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
   int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1);

   //run the function
   dirdiveRemove(MB->FirstClusterOfRootDirectory, fp, MB, bytesPerSector, sectorsPerCluster, deleteFile);
   

   
   if (munmap(fp, size))
   {
      perror("error from unmap:");
      exit(1);
   }

   // close the file
   if (close(fd))
   {
      perror("close:");
   }
   fd = 0;

   return 0;
}