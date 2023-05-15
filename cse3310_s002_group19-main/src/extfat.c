/*

   This is an example of using mmap to read an extFAT image file.

   To make the sample file:

    % # create the file system image

    % dd if=/dev/zero of=test.image count=1 bs=1M

    % sudo losetup /dev/loop2 test.image
    % sudo /usr/sbin/mkexfatfs /dev/loop2

    % # put something in the file system image
    % mkdir /tmp/d
    % sudo mount /dev/loop2 /tmp/d
    % cp examples/mmap.c /tmp/d

    % # clean up
    % sudo umount /tmp/d
    % sudo losetup -d /dev/loop2
    % rm -rf /tmp/d
    % rm test.image

//xxd test.image | less

   Written by Bud Davis, jimmie.davis@uta.edu
   (c) 2023, All Rights Reserved
   Provided to students of CSE3310, UTA. Any use
   other than this course is prohibited.

*/

#include <stdio.h>
#include "extfat.h"
#include "commandargs.h"
#include "isEmpty.h"
#include "fileCopy.h"
#include "DiffCheck.h"
#include "routines.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
   // Allocate memory for the fileInfo structure
   fileInfo *INFILE = malloc(sizeof(*INFILE));
   fileInfo *OFILE = malloc(sizeof(*OFILE));

   const int error = -1;
   // Task 1 - commandargs.c
   // struct commandopts *S = malloc(sizeof(commandopts));
   assert(argc != 0);
   assert(argv[1] != NULL);
   struct commandopts S = commandargs(argc, argv);
   char *inFileName = S.inputfile;
   char *outFileName = S.outputfile;

   // open the file system image file
   // this was changed on merge
   assert(inFileName != NULL);
   int fd = open(inFileName, O_RDWR);
   if (fd == error)
   {
      perror("file open: ");
      exit(1);
   }

   // gets size of file
   struct stat st;
   if (fstat(fd, &st) == -1)
   {
      perror("fstat");
      exit(1);
   }

   // Take the pointer returned from mmap() and turn it into
   // a structure that understands the layout of the data
   /*
    void *t    = (void*)(Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot) + 4096, // worse case misallignment is 4096
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     0); // note the offset
                                     */
   off_t size = 0;
   struct stat statbuf;
   assert(fd != error);
   if (fstat(fd, &statbuf))
   {
      perror("stat of file:");
      exit(0);
   }
   size = statbuf.st_size;


   //For DiffCheck start
       void *v    = (void*)(Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot) + 4096, // worse case misallignment is 4096
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     0); // note the offset
   Main_Boot *MB=v;

   if (MB == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(0);
   }
   v+=(512*12);
   Main_Boot *BB = (Main_Boot*) v;


   if (BB == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(0);
   }
   //For DiffCheck end

   // mmap the entire file into memory
   // every data item we reference, will be relative to fp...
   void *t = (void *)mmap(NULL,
                          size,
                          PROT_READ,
                          MAP_PRIVATE,
                          fd,
                          0); // note the offset
   Main_Boot *MBR = t;

   if (MBR == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(1);
   }
   /*
   t+=(512*12);
   Main_Boot *BB = (Main_Boot*) t;
   */
   int bytesPerSector = 2 << (MBR->BytesPerSectorShift - 1);
   // next, the Backup Boot record
   Main_Boot *BBR = (Main_Boot *)(t + 12 * bytesPerSector);

   if (BBR == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(1);
   }

   // storing information in struct
   INFILE->M_Boot = MB;
   INFILE->B_Boot = BB;
   INFILE->FAT = t + (512 * 12);
   INFILE->Data = t + (512 * 12);
   INFILE->fileName = inFileName;
   INFILE->fd = fd;
   INFILE->SectorSize = sizeof(Main_Boot);
   INFILE->FileLength = st.st_size;

   // storing information in struct
   OFILE->M_Boot = MB;
   OFILE->B_Boot = BB;
   OFILE->FAT = t + (512 * 12);  // to extracted from image. maybe should be null?
   OFILE->Data = t + (512 * 12); // to extracted from image. maybe should be null?
   OFILE->fileName = outFileName;
   OFILE->fd = fd;
   OFILE->SectorSize = sizeof(Main_Boot);
   OFILE->FileLength = st.st_size;

   // Task 2 - fileCopy.c
   // names for testing, will add more in next iteration.
   //  char* inFileName = "test.image";
   //  char* outFileName = "outFile.image";
   assert(inFileName != NULL);
   assert(outFileName != NULL);
   if ((strcmp(inFileName, outFileName) == 0) && (S.copy) == 1)
   {
      printf("Please do not enter in two same file names.\n");
      exit(1);
   }

   if (S.copy == 1)
   {
      assert(INFILE->fileName != NULL);
      assert(OFILE->fileName != NULL);
      // old: infileName, outFileName
      // passing in filenames
      fileCopy(INFILE->fileName, OFILE->fileName);
   }

   // Task 4 - DiffCheck.c
   // check if received option -v Task 4
   if (S.verifyints == 1)
   {
      // DiffCheck(MB,BB);
      printf("BackUpBootRecord %p\n", BBR);
      printf("FileSystemName %s\n", BBR->FileSystemName); // warning, not required to be terminated

      // Checksum
      assert(MBR != (Main_Boot *)-1);
      assert(BBR != (Main_Boot *)-1);
      uint32_t mbrChecksum = BootChecksum((uint8_t *)MBR, (short)bytesPerSector);
      uint32_t bbrChecksum = BootChecksum((uint8_t *)BBR, (short)bytesPerSector);
      printf("Checksum  (MB) %x (BBR) %x\n", mbrChecksum, bbrChecksum);
      DiffCheck(INFILE->M_Boot, OFILE->B_Boot);
      if (mbrChecksum != bbrChecksum)
      {
         printf("Main boot does not equal backup boot. error\n");
         exit(1);
      }
   }

   // End Process
   // unmap the file Main
   assert(MBR != NULL);
   if (munmap(MBR, sizeof(Main_Boot)) == error)
   {
      perror("error from unmap :");
      exit(1);
   }

   // close the file
   assert(fd != error);
   if (close(fd))
   {
      perror("closeStat:");
   }
   fd = 0;
   assert(S.inputfile != NULL);
   free(S.inputfile);
   assert(S.outputfile != NULL);
   free(S.outputfile);
   S.inputfile = NULL;
   S.outputfile = NULL;
   assert(INFILE != NULL);
   assert(OFILE != NULL);
   free(INFILE);
   free(OFILE);
   INFILE = NULL;
   OFILE = NULL;

   return 0;
}