#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 100

int main(void){
  char input[NUM] = {"1.2a312"};
  char buff1[NUM];
  char format[40], buff2[5];
  double result;

  sprintf(buff2, "%d", NUM-1);
  sprintf(format, "%s%d%s", "%", NUM-1, "[0123456789.\n]");
  printf("format : %s\n", format);

  sscanf(input, format, buff1);
  printf("input : %s\n", input);
  printf("buff1 : %s\n", buff1);

  if(strlen(input)!=strlen(buff1)){
    printf("Invalid data!!\n");
    return -2;
  }

  if(strchr(buff1, '.')!=strrchr(buff1, '.')){
    printf("Error!!\n");
    return 1;
  }

  if(!(int)(input - strchr(input, '.'))){
    printf("Invalid format!!\n");
    return -1;
  }

  sscanf(buff1, "%lf", &result);

  printf("Data : %f\n", result);

  return 0;
}

