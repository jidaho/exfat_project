#include "isEmpty.h"

// function to use to check if the input or output file are empty
int isEmpty(char *array, int size)
{
   // checks if the first character of the array is another flag
   if (array[0] == 45)
   {
      return 1;
   }
   for (int i = 0; i < size; i++)
   {
      if (array[i] != 0)
      {
         return 0;
      }
   }

   return 1;
}
