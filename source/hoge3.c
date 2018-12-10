#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_ERROR_MESSAGE "OutOfMemoryException!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size) \
  do{\
    if((ptr=(cast)calloc(num,size))==NULL)\
      errorEXE(MEMORY_ERROR_MESSAGE);\
  }while(0)

void get2D_ary(int ***p, int num){
  int i;

  def_calloc(*p, int **, num, sizeof(int *));
  for(i = 0; i < num; i++)
    def_calloc((*p)[i], int *, num, sizeof(int));
}

void free2D_ary(int **p, int num){
  int i;

  for(i = 0; i < num; i++)
    free(p[i]);

  free(p);
}


int prep(int ***p){
  char buff[50];
  int  num;

  fgets(buff, sizeof(buff), stdin);

  sscanf(buff, "%d", &num);

  get2D_ary(p, num);

  return num;
}

int matchPattern(int *point, int **np, int N, int **mp, int M){
  int i, j, flag;

  flag = 0;
  for(i = 0; i < M; i++)
    for(j = 0; j < M; j++)
      flag |= (np[i+point[0]][j+point[1]] ^ mp[i][j]);

  return !flag;
}

void doAllProcess(void){
  int  **np, **mp;
  char buff[1000], *bp;
  int  point[2];
  int  N, M, i, j, flag;

  N = prep(&np);
  for(i = 0; i < N; i++){
    fgets(buff, sizeof(buff), stdin);

    bp = strtok(buff, " ");
    np[i][0] = *bp-'0';
    for(j = 1; j < N; j++){
      bp = strtok(NULL, " ");
      np[i][j] = *bp-'0';
    }
  }

  M = prep(&mp);
  for(i = 0; i < M; i++){
    fgets(buff, sizeof(buff), stdin);

    bp = strtok(buff, " ");
    mp[i][0] = *bp-'0';
    for(j = 1; j < M; j++){
      bp = strtok(NULL, " ");
      mp[i][j] = *bp-'0';
    }
  }

  for(i = 0; i <= N-M; i++){
    for(j = 0; j <= N-M; j++){
      point[0] = i;
      point[1] = j;

      flag = matchPattern(point, np, N, mp, M);

      if(flag){
        i = N;
        break;
      }
    }
  }

  printf("\nans : (%d %d)\n", point[0], point[1]);

  free2D_ary(np, N);
  free2D_ary(mp, M);
}

int main(void){

  doAllProcess();

  return 0;
}

