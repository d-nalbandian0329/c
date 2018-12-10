#include <stdio.h>
#include <stdarg.h>

#define NUM 10

double sum(int num, ...){
  double sum_d;
  int    i;

  va_list p;
  va_start(p, num);

  sum_d = 0.0;
  for(i = 0; i < num; i++)
    sum_d += va_arg(p, double);

  return sum_d;
}

int main(void){
  double array[NUM], sum_d;
  int i;

  for(i = 0; i < NUM; i++)
    array[i] = (double)i;

  sum_d = sum(3, array[0], array[1], array[2]);
  printf("0+1+2=%d\n", (int)sum_d);

  sum_d = sum(5, array[0], array[1], array[2], array[3], array[4]);
  printf("0+1+2+3+4=%d\n", (int)sum_d);

  return 0;
}

