#include <stdio.h>
#include <string.h>

int main(void){
  char str[1000] = "5\n3\n1 3 4\n3\n2 3 5";
  char *p[5];
  int  i, M2, n;

  p[0] = strtok(str, "\n");
  for(i = 1; i < 5; i++)
    p[i] = strtok(NULL, "\n");

  for(i = 0; i < 5; i++)
    printf("p[%d] : %s\n", i, p[i]);

  sscanf(p[3], "%d", &M2);
  printf("M2 : %d\n", M2);

  printf("p[%d] : %s\n", 2, p[2]);
  printf("%c\n", *(p[4]+4));

  if(strchr(p[2], *(p[4]+4)))
    printf("OK\n");

  n = 0;
  for(i = 0; i < M2; i++)
    if(strchr(p[2], *(p[4]+i*2)))
      ++n;

  printf("n : %d\n", n);

  return 0;
}

