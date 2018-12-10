#include <stdio.h>

#define swap(type,x,y) {type t = x; x = y; y = t;}

int main(void){
  int a = 1, b = 2, c = 3;

  printf("%d, %d, %d\n", a, b, c);

  swap(int, a, b);

  {
  printf("%d, %d, %d\n", a, b, c);
  }

  return 0;
}
