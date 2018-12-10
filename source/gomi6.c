#include <stdio.h>
#include <stdlib.h>

#define CN 10

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

int main(void){
  int **p;
  int i, j;

  if((p = (int **)calloc(CN, sizeof(int *))) == NULL)
    errorEXE("MemoryOverflow!!\n");

  for(i = 0; i < CN; i++){
    if((p[i] = (int *)calloc(2, sizeof(int))) == NULL)
      errorEXE("MemoryOverflow!!\n");
  }

  for(i = 0; i < CN; i++)
    for(j = 0; j < 2; j++)
      printf("a[%d][%d] : %d\n", i, j, p[i][j]);

  for(i = 0; i < CN; i++)
    free(p[i]);

  free(p);

  return 0;
}
