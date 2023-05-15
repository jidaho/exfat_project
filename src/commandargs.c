#include "commandargs.h"
#include "isEmpty.h"
#include <getopt.h>
#include <assert.h>
#include "listFiles.h"
#include "extract.h"
#include "fileDelete.h"
#include "parser.h"

// All tasks with functions in here proves their work (Not necessarily mine)

struct commandopts commandargs(int argc, char *argv[])
{
   int check;
   // find the largest argc entry, to malloc the input and output file sizes with
   int biggest_file_size = 0;
   for (int i = 0; i < argc; i++)
   {
      if ((int)strlen(argv[i]) > biggest_file_size)
      {
         biggest_file_size = (int)strlen(argv[i]);
      }
   }
   // the struct header is added in commandargs.h
   struct commandopts args;

   //allocation for all the file names that could potentiall be needed is done here
   char* extractFile = (char *)malloc(biggest_file_size*sizeof(char)+1);
   char* deleteFile = (char *)malloc(biggest_file_size*sizeof(char)+1);
   args.inputfile = (char *)malloc(biggest_file_size * sizeof(char) + 1);
   args.outputfile = (char *)malloc(biggest_file_size * sizeof(char) + 1);
   // set them to 0 for checking in the future
   assert(args.inputfile != NULL);
   assert(args.outputfile != NULL);

   //set them to 0 for the isEmpty check
   //might need to do the same for extractFile and deleteFile
   memset(args.inputfile, 0, biggest_file_size * sizeof(char) + 1);
   memset(args.outputfile, 0, biggest_file_size * sizeof(char) + 1);


   //declaration of all flags as 0
   args.copy = 0;
   args.verifyints = 0;
   int directory = 0;
   int extractflag = 0;
   int deleteflag = 0;

   assert(argc >= 2);
   assert(argv[1] != NULL);
   while ((check = getopt(argc, argv, "-iovhcdxD")) != -1)
   {
      switch (check)
      {

      case 'i':
         for (int i = 0; i < argc; i++)
         {
            // checks if a -i is inputted, and if so, that it has a following argument
            assert(i < argc);
            if ((strcmp(argv[i], "-i") == 0) && (i + 1 < argc))
            {
               // printf("argv[%d]:%s",i,argv[i]);
               // assert(i+1<argc);
               assert(args.inputfile != NULL);
               assert(argv[i + 1] != NULL);
               strcpy(args.inputfile, argv[i + 1]);

            }
         }
         break;
      case 'o':
         for (int i = 0; i < argc; i++)
         {
            assert(argv[i] != NULL);
            if ((strcmp(argv[i], "-o") == 0) && (i + 1 < argc))
            {
               // assert(i+1<argc);
               assert(args.outputfile != NULL);
               assert(argv[i + 1] != NULL);
               strcpy(args.outputfile, argv[i + 1]);
            }
         }

         break;
      case 'D':
         deleteflag = 1;
         for (int i = 0; i < argc; i++)
         {
            // checks if a -D is inputted, and if so, that it has a following argument
            assert(i < argc);
            if ((strcmp(argv[i], "-D") == 0) && (i + 1 < argc))
            {
               assert(args.inputfile != NULL);
               assert(argv[i + 1] != NULL);
               strcpy(deleteFile, argv[i + 1]);
            }
         }
         break;

      case 'x':
         extractflag = 1;
         for (int i = 0; i < argc; i++)
         {
            // checks if a -x is inputted, and if so, that it has a following argument
            assert(i < argc);
            if ((strcmp(argv[i], "-x") == 0) && (i + 1 < argc))
            {

               assert(args.inputfile != NULL);
               assert(argv[i + 1] != NULL);
               strcpy(extractFile, argv[i + 1]);
            }

         }

         break;
      case 'v':
         args.verifyints = 1;

         break;
      
      //update with -D and -x
      case 'h':
         printf("This program accepts the following flags: \n\n");
         printf("(required) -i <input file>\n");
         printf("Provides an input file image\n\n");
         printf("(optional)  -o <output file>\n");
         printf("Provides an output file image\n");
         printf("if not provided, the input file image will be used as the output file image\n\n");
         printf("(optional) -c \n");
         printf("Copy - The input file will be copied to the output file without change\n\n");
         printf("(optional) -v\n");
         printf("Verify - the main and backup boot sectors will be checked, if they are not the same, an error message will be printed\n\n");
         printf("(optional) -d\n");
         printf("Display - The contents of the file system will be displayed on the terminal, in an indented format\n\n");
         printf("(optional) -D\n");
         printf("Display - The specified file will be deleted from the extfat file system.\n\n");
         printf("(optional) -x\n");
         printf("Display - The specified file will be extracted into a designated output file.\n\n");
         assert(args.inputfile != NULL);
         assert(args.outputfile != NULL);
         free(args.inputfile);
         free(args.outputfile);
         args.inputfile = NULL;
         args.outputfile = NULL;
         exit(0);

         break;
      case 'c':
         args.copy = 1;

         break;
      case 'd':
         directory = 1;
         break;
      case '?':
         perror("Invalid command entered, program closing, please use -h for a list of accepted commands\n");
         // printf("Invalid command entered, program closing, please use -h for a list of accepted commands\n");
         free(args.inputfile);
         free(args.outputfile);
         free(extractFile);
         free(deleteFile);
         exit(1);
         break;
      }
   }
   if (directory == 1)
   {
      listFiles(args.inputfile);
   }

   if (deleteflag == 1)
   {
      if (deleteFile[0] == '/')
      {
         parser(deleteFile);
      }
      fileDelete(args.inputfile, deleteFile);
   }


   // close the program with a helpful prompt if no inputfile is included
   if (isEmpty(args.inputfile, biggest_file_size) == 1)
   {
      printf("Invalid input - an input file must be entered\n");
      printf("input file: %s\n", args.inputfile);
      assert(args.inputfile != NULL);
      assert(args.outputfile != NULL);
      free(args.inputfile);
      free(args.outputfile);
      free(extractFile);
      free(deleteFile);
      args.inputfile = NULL;
      args.outputfile = NULL;
      exit(0);
   }

   // copy inputfile to output file if no output file
   if (isEmpty(args.outputfile, biggest_file_size))
   {
      // printf("No output file given, copying input file to output file\n");
      assert(args.inputfile != NULL);
      assert(args.outputfile != NULL);
      strcpy(args.outputfile, args.inputfile);
   }

   if (extractflag == 1)
   {
      if (extractFile[0] == '/')
      {
         parser(extractFile);
      }
      if (strcmp(args.inputfile, args.outputfile) == 0)
      {
         printf("A valid output file must be entered using -o\nProgram will now close\n");
         exit(0);
      }
      extract(args.inputfile, extractFile, args.outputfile);
   }

   free(deleteFile);
   free(extractFile);
   return args;
}
