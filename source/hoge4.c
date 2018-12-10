#include <stdio.h>

#define NUM 101

void add(int *array, int data){
  int i, j, dest;

  i = 0;
  while(array[i]>data) i++;

  dest = i;
  while(array[dest]) dest++;

  for(j = dest; j > i; j--)
    array[j] = array[j-1];

  array[i] = data;
}

int minimum(int *array){
  int min, i;

  min = array[0] - array[1];
  for(i = 1; i < NUM-1 && array[i]; i++){
    if(min > array[i]-array[i+1])
      min = array[i] - array[i+1];
  }

  return min;
}

void doAllProcess(void){
  char buff[50];
  int  Z;
  int  x_edge[NUM] = {0};
  int  y_edge[NUM] = {0};
  int  n, i, flag, hoge;

  fgets(buff, sizeof(buff), stdin);
  sscanf(buff, "%d %d %d %d", &x_edge[0], &y_edge[0], &Z, &n);

  for(i = 0; i < n; i++){
    fgets(buff, sizeof(buff), stdin);
    sscanf(buff, "%d %d", &flag, &hoge);

    add((flag ? y_edge : x_edge), hoge);
  }

  printf("%d", minimum(x_edge)*minimum(y_edge)*Z);
}

int main(void){

  doAllProcess();

  return 0;
}

