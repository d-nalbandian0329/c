#include <stdio.h>

typedef union{
  unsigned char x;
  struct {
    unsigned int b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;
  }b;
}bit;


int main(void){
  bit obj;
  obj.x = 255U;

  printf("x      : %x\n", (obj.x));

  printf("\nx[bit] : ");
  printf("%u", obj.b.b7);
  printf("%u", obj.b.b6);
  printf("%u", obj.b.b5);
  printf("%u", obj.b.b4);
  printf("%u", obj.b.b3);
  printf("%u", obj.b.b2);
  printf("%u", obj.b.b1);
  printf("%u", obj.b.b0);
  putchar('\n');

  return 0;
}

