#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 11

#define MEMORY_ERROR_MESSAGE "OutOfMemoryException!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size)\
  do{\
    if((ptr = (cast)calloc(num,size)) == NULL)\
      errorEXE(MEMORY_ERROR_MESSAGE);\
  }while(0)


void get2Darray(char ***p, int num){
  int i;

  def_calloc(*p, char **, num, sizeof(char *));
  for(i = 0; i < num; i++)
    def_calloc((*p)[i], char *, NUM, sizeof(char));
}

void free2Darray(char **p, int num){
  int i;

  for(i = 0; i < num; i++)
    free(p[i]);

  free(p);
}

void doAllProcess(void){
  char **p, buff[5];
  int  n, i;

  fgets(buff, sizeof(buff), stdin);
  sscanf(buff, "%d", &n);

  get2Darray(&p, n);

  i = 0;
  while(i < n){
    char hoge[NUM], hoge2[NUM];

    fgets(hoge, NUM, stdin);

    sscanf(hoge, "%10[^0123456789]", hoge2);

    if((strchr(hoge, '\n')-hoge)==(strchr(hoge2, '\n')-hoge2)){
      sscanf(hoge2, "%10[^\n]", p[i]);
      ++i;
    }
  }

  printf("%s", p[0]);

  for(i = 1; i < n; i++)
    printf("_%s", p[i]);

  free2Darray(p, n);
}

int main(void){

  doAllProcess();

  return 0;
}

