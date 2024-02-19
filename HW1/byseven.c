#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[]){
   if (argc != 2){
      return EXIT_FAILURE;
      /* two arguments, filename and int*/
   }
   int n = atoi(argv[1]); //turns str command into number  
   if (n <= 0) {
      return EXIT_FAILURE;
   }
   for (int i = 6; n > 0; i++) {
      if (i%7 ==0) {
         printf("%d\n", i);
         n--;
      }
   }
   return EXIT_SUCCESS;
}
