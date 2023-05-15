//this function parses through an input string and splits it into two pieces

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLENGTH 100

char* parser(char* filePath)
{
    //create three variables to handle the dissection
    //fileName is not a pointer, but is a local variable so that 
    //when the char* pointer is returned, it can be independent from the input
    char* ch = filePath;
    char fileName[strlen(filePath) + 1];
    char* remainingPath[strlen(filePath) + 1];

    //int flag = 0;

    // Ignore an opening '/' character if used
    //Returns a null character pointer that wont be used, this segment of code
    //should only be called in commandargs.c in the event that there is a leading '/'
    //on the file path, it removes that dash and modifies the inputted file path to have no
    //leading '/'
    if (ch[0] == '/') 
    {
        char* modifiedPath[strlen(filePath)+1];
        ch++;
        int j = 0;
        while(*ch)
        {
            modifiedPath[j] = ch;
            ch++;
            j++;
            
        }
        modifiedPath[j] = '\0';
        strcpy(filePath, *modifiedPath);
        return strdup(fileName);
    }
    int i = 0;


    //add characters to fileName until a /
    while (*ch && *ch != '/') 
    {
        fileName[i] = *ch;
        ch++;
        i++;
    }
    fileName[i] = '\0';

    i = 0;
    //add the rest to the remaining path, skipping only the first '/'
    while (*ch) 
    {

        if (ch[0] == '/') 
        {
        ch++;
        }
        remainingPath[i] = ch;
        ch++;
        i++;
    }
    //if i==0, then remainingPath is pointing to NULL and shouldn't be copied, so this exits parser early
    if (i == 0)
    {
        //printf("filePath: %s  fileName: %s \n", filePath, fileName);
        strcpy(filePath, fileName);
        return strdup(fileName);
    }

    //modify filePath by copying remainingPath over to it, so that the input filePath will be modified
    //when the program closes
    remainingPath[i] = '\0';
    strcpy(filePath, *remainingPath); 
    
    //strdup returns a char* pointer to a mallocated string, which can now be returned and is independent
    //of the input pointer

    return strdup(fileName);
}