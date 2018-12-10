#include <stdio.h>
#include <stdlib.h>

#define ERROR_MESSAGE "Error!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size) \
                      do{\
                        if((ptr=(cast)calloc(num,size))==NULL)\
                          errorEXE(ERROR_MESSAGE);\
                      }while(0)

typedef unsigned char Data;

// Allocate primitive type array.
void get_array_2d(Data ***p, int row, int col){
  int i;

  def_calloc(*p, Data **, row, sizeof(Data *));

  for(i = 0; i < row; i++)
    def_calloc((*p)[i], Data *, col, sizeof(Data));
}

// Free 2D array.
void free_array_2d(Data **p, int row){
  int i;

  for(i = 0; i < row; i++)
    free((p)[i]);
  free(p);
}

int main(void){
  Data **p, i, j;

  get_array_2d(&p, 5, 5);

  for(i = 0; i < 5; i++){
    for(j = 0; j < 5; j++){
      p[i][j] = (Data)10;
      printf("p[%d][%d] : %u\n", i, j, p[i][j]);
    }
  }

  free_array_2d(p, 5);

  return 0;
}
