/*
Recursively parses through directories until it finds the file, and then it extracts it to a mandatory output file: 
its requirements are:

1) -D and -x command will accept a file in a subdirectory if entered in the notation directory1/directory2/directory3/filename
2) All other notations of entering a file in a subdirectory will result in an error message and the program will close.
3) -x command will accept only a file as its argument
4) -x command will extract all of the contents in that file and place them in the argument given by -o
5) -x command must have a valid -o argument to execute, otherwise an error message will display and the program will close
6) if the -x command is used when the arguments for -i and -o are the same, an error message will display and the program will close



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


// this function recursively traverses through the file system, finding all directories and files starting from the
// root directory.
// dir dive takes the cluster number, the file handle and MB as well as the bytes Per Sector and sectors per cluster that are
// defined in Main
// The dive depth variable is simply a formatting variable that will tab out based on the depth of the recursion, so
// the directory hierarchy will be visually easy to see.
struct fileStreamExtension* dirdive(int clusterNum, void *fp, Main_Boot *MB, 
                                    int bytesPerSector, int sectorsPerCluster, char* extractFile)
{
GenericDirectoryStruct *GDS = cluster(clusterNum);
int i = 0;
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
            
            //this char* will store the name of the file once the FND is parsed, it needs to be outside
            //so that it is in the same scope as the recursive part of the function
            char* filesss = malloc(15*sizeof(char));
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
            //if the name of the extractFile is the same as the file found in filename, then return that file
            //stream extension, so that it can be extracted
               
            //since parser modifies the input char*, create a temporary variable to hold it in case the strcmp fails
            //newFilePath must be mallocated so that it can be altered without affecting extract file
            //If extract file is parsed, but the GDE is at the wrong directory, there is no way to go back

            char* newFilePath = malloc(strlen(extractFile)+1);
            strcpy(newFilePath, extractFile);

            //The parser function will split the file path into two pieces, one piece should be a directory name
            //the other piece should be the remaining file path

            char* next = parser(newFilePath);

            //if the name of the parsed file matches, then at the right directory
            if ((strcmp(filesss, next) == 0) && (FSE->EntryType == 0xc0))
            {

               //if the strings match, change extractFile so that it holds the remaining file path
               extractFile = newFilePath;
               return dirdive(FSE->FirstCluster, fp, MB, bytesPerSector, sectorsPerCluster, newFilePath);
                  
            }

            //this piece of code could be deleted, but left it as a reminder that if the directory does not match
            //no recursion should be done
            else if (FSE->EntryType == 0xc0)
            {

               //return dirdive(FSE->FirstCluster, fp, MB, bytesPerSector, sectorsPerCluster, extractFile);
            }
               free(filesss);
               free(newFilePath);
         }
         
         // this is a normal file, and we just need the name of it
         else if (FDE->FileAttributes == 0x20)
         {

            //Needs the file Stream Extension in order to get to the file contents
            i++; 
            struct fileStreamExtension *FSE = (struct fileStreamExtension *)&GDS[i];
            i++; // i is now pointing to the file name directory
            struct fileNameDirectory *FND = (struct fileNameDirectory *)&GDS[i];
            if (FND->EntryType == 0xc1)
            {
               char *ch = (char *)FND->FileName;
               char* filesss = malloc(15*sizeof(char));
               //set the allocated memory to 0 to remove any junk that may or may not be present
               memset(filesss, 0, 15 * sizeof(char));
               while (*ch)
               {
                  strcat(filesss, ch);
                  ch++;
                  ch++;

               }

               //if the name of the extractFile is the same as the file found in filename, then return that file
               //stream extension, so that it can be extracted
               //parser(extractFile);

               if (strcmp(filesss, extractFile) == 0)
               { 
                  return FSE;
               }

            }
         }
         else
         {
            printf("Invalid value, neither a file or directory\n");
         }
      }
      i++;
   }

   //this exit should only occur if no file is found
   printf("No file of that name was found, program will now exit.\n"); 
   exit(1);

}




int extract(char* imageName, char *extractFile, char* destinationFile)
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
   // Note the flags - mmap must be writable (PROT_WRITE) or segfault
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

   // calls the directory dive function written above to find the file that should be deleted

   struct fileStreamExtension* FSE = dirdive(MB->FirstClusterOfRootDirectory, fp, MB, bytesPerSector, sectorsPerCluster, extractFile);

   //the code for extracting the file to another file
   FILE* outputFile;
   char* outputFileName = destinationFile;


   outputFile = fopen(outputFileName, "wb");
   if (fp == NULL) 
   {
      printf("Error: could not open file\n");
      return 1;
   }

   if (outputFile == NULL) 
   {
      printf("Error: could not open output file\n");
      return 1;
   }

   //find the offset and file size of the cluster of the desired file
   long int offset = cluster(FSE->FirstCluster) - fp;
   int filesize = FSE->DataLength;

   //use fwrite to write (file size) number of bytes from (fp+offset) the start of the file into the output file
   fwrite(fp+offset, 1, filesize, outputFile);
   printf("File has been successfully extracted.\n");


   fclose(outputFile);
   // unmap the file
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