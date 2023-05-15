#include "fileCopy.h"
#include "extfat.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

// void fileCopy(char* inFileName, char* outFileName)
// {
//     FILE* Handler = fopen(inFileName,"r");
//     if (Handler == NULL)
//     {

//     }
//     Handler = fopen(outFileName,"w");
//     fclose(Handler);
// }
void fileCopy(char *inFileName, char *outFileName)
{
   const int error = -1;
   // open the file system image file
   int fd = open(inFileName, O_RDWR);
   assert(fd != error);
   if (fd == error)
   {
      perror("file open: ");
      exit(0);
   }

   // Take the pointer returned from mmap() and turn it into
   // a structure that understands the layout of the data
   Main_Boot *MB = (Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot),
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     0); // note the offset

   if (MB == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(0);
   }

   // shift the bytes and sectors to proper places
   int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
   // int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1 );

   // open file and write from mmap
   assert(outFileName != NULL);
   FILE *outFile = fopen(outFileName, "w");
   char b = 'a';
   // value acquired by dividing total size of Volume length by bytes per sector cluster, initialize the outFile
   long desiredSize = MB->VolumeLength * bytesPerSector;
   assert(desiredSize != 0);
   fseek(outFile, desiredSize - 1, SEEK_END);
   fwrite(&b, 1, sizeof(char), outFile);
   fclose(outFile);

   int fdOut = open(outFileName, O_RDWR);
   if (fdOut == error)
   {
      perror("file open: ");
      exit(0);
   }

   // write image in char to save size
   char *outImage = (char *)mmap(NULL,
                                 desiredSize,
                                 PROT_READ | PROT_WRITE, MAP_SHARED,
                                 fdOut,
                                 0); // note the offset
   if (outImage == NULL)
   {
      perror("error from mmap:");
      exit(0);
   }

   // unmap the file
   assert(MB != NULL);
   if (munmap(MB, sizeof(Main_Boot)) == error)
   {
      perror("error from unmap:");
      exit(0);
   }

   // close the file
   assert(fd != error);
   if (close(fd))
   {
      perror("closeStat:");
   }
   fd = 0;

   // open the file system image file
   assert(inFileName != NULL);
   fd = open(inFileName, O_RDWR);
   if (fd == error)
   {
      perror("file open: ");
      exit(0);
   }

   // open a new image in the size we want
   char *MBchar = (char *)mmap(NULL,
                               desiredSize,
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE,
                               fd,
                               0); // note the offset

   if (MBchar == NULL)
   {
      perror("error from mmap:");
      exit(0);
   }

   // copy over every piece of data in the original file to outImage
   for (int index = 0; index < desiredSize; index++)
   {
      outImage[index] = MBchar[index];
   }

   assert(outImage != NULL);
   if (msync(outImage, 10, MS_SYNC) == error)
   {
      perror("Could not sync the file to disk");
   }

   // unmap the file
   assert(outImage != NULL);
   assert(desiredSize != 0);
   if (munmap(outImage, desiredSize) == error)
   {
      perror("error from unmap:");
      exit(0);
   }

   // close the file
   if (close(fdOut))
   {
      perror("closeStat:");
   }
   fdOut = 0;

   assert(MBchar != NULL);
   assert(desiredSize != 0);
   if (munmap(MBchar, desiredSize) == error)
   {
      perror("error from unmap:");
      exit(0);
   }

   // close the file
   assert(fd != error);
   if (close(fd))
   {
      perror("closeStat:");
   }
   fd = 0;
}