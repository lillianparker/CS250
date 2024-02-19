#include "stdio.h"
#include "stdlib.h"


int f(int n) {
    if (n == 0){
        return 2;
    }
    else {
        return 3 * (n - 1) + f(n - 1) + 1;
    }
    /* recursive call with base case of f(0) = 2*/
}

int main(int argc, char *argv[]){
   if (argc != 2){
        return EXIT_FAILURE;
      /* two arguments, filename and int*/
   }
   int n = atoi(argv[1]); //turns str commhand into number
   if (n < 0) {
        printf("N must be greater than 0\n");
        return EXIT_FAILURE;
   }

   int product = f(n); 
   printf("%d\n", product);
   return EXIT_SUCCESS;
}