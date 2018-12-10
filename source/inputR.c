#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 100

double *checker(double *ret, const char *prompt){
  char value[NUM], buff[NUM];
  char format[40];

  printf("%s", prompt);
  fgets(value, (int)sizeof(value), stdin);

  sprintf(format, "%s%d%s", "%", NUM-1, "[0123456789.\n]");
  
  sscanf(value, format, buff);

  if(strlen(value)==strlen(buff)){
    if(strchr(value, '.')==strrchr(value, '.')){
      int diff = (int)(strchr(value, '.')-value);

      if(diff && ((int)strlen(value)-diff-2)){
        sscanf(value, "%lf", ret);
        return ret;
      }
    }
  }

  return NULL;
}


double input(){
  double ret;

  while(checker(&ret, "data : ")==NULL);

  return ret;
}


int main(void){
  printf("\nInput data : %lf\n", input());

  return 0;
}

