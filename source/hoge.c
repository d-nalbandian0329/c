#include <stdio.h>
#include <stdlib.h>

#define exError(str) printf(str),exit(EXIT_FAILURE)

int main(void){
  if(1)
    exError("Error macro!!\n");

  return 0;
}
