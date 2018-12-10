#include <stdio.h>
#include <stdlib.h>
#include "QUEUE.h"

int main(void){
  Queue int_que;
  Queue *queue;
  int i;

  queue = &int_que;
  initQueue(queue);

  printf("Enqueue :");
  for(i = 0; i < 20; i++)
    printf(" %d", enqueue(queue, i));
  putchar('\n');

  printf("Dequeue :");
  for(i = 0; i < 10; i++)
    printf(" %d", dequeue(queue));
  putchar('\n');

  printf("Enqueue :");
  for(i = 0; i < 4; i++)
    printf(" %d", enqueue(queue, i));
  putchar('\n');

  printf("Get  : %d\n", get(queue));
  printf("Size : %d\n", size(queue));

  termQueue(queue);

  return 0;
}
