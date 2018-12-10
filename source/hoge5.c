#include <stdio.h>


int main(void){
  int  i, j;
  long ret, N;
  char buff[10];

  fgets(buff, sizeof(buff), stdin);
  sscanf(buff, "%ld", &N);

  ret = 1L;
  for(i = 1; i <= N; i++){
    i %= 1000000000L;

    ret = calc(ret, i);

    while()

  }

  printf("%ld", ret);

  return 0;
}

