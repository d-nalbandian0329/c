#include <stdio.h>
#include <stdlib.h>
#include "QUEUE.h"

void show(Queue *q){
  int limit = getSize(&q->list);
  int i;

  printf("List :");
  for(i = 0; i < limit; i++)
    printf(" %d", (int)getData(&q->list, i));
  putchar('\n');
}

int main(void){
  Queue queA, queB;
  int   i;

  initQueue(&queA);
  initQueue(&queB);

  for(i = 0; i < 10; i++){
    enqueue(&queA, i);
  }
  putchar('A');  show(&queA);
  putchar('B');  show(&queB);

  for(i = 0; i < 5; i++)
    enqueue(&queB, dequeue(&queA));
  putchar('A');  show(&queA);
  putchar('B');  show(&queB);

  for(i = 0; i < 3; i++)
    enqueue(&queA, dequeue(&queB));
  putchar('A');  show(&queA);
  putchar('B');  show(&queB);



  termQueue(&queA);
  termQueue(&queB);

  return 0;
}
