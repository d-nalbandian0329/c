#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ERROR_MESSAGE "\nOutOfMemoryException!!\n"
#define INDEX_ERROR_MESSAGE  "\nOutOfIndexException!!\n"
#define EMPTY_ERROR_MESSAGE  "\nEmptyException!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define retEXE(str) do{ printf(str); return 0; }while(0)

#define def_calloc(ptr,cast,num,size) \
  do{\
    if((ptr=(cast)calloc(num,size))==NULL)\
      errorEXE(MEMORY_ERROR_MESSAGE);\
  }while(0)

#define index_check(index,LOW,HIGH)\
  do{\
    if(index<LOW||index>HIGH)\
      retEXE(INDEX_ERROR_MESSAGE);\
  }while(0)

#define size_check(size,LOW)\
  do{\
    if(size<LOW)\
      retEXE(EMPTY_ERROR_MESSAGE);\
  }while(0)

// Test.
typedef int Data;


// Node type.
typedef struct node{
  Data data;         // Data part.
  struct node *next; // Pointer part.
}Node;

// List type.
typedef struct list{
  Node dummy;   // Dummy head.
  int  size;    // Size of list.
}List;

// Initialized the list.
void initList(List *list);

// Terminate the list.
void termList(List *list);

// Add new node to the list.
// @para1 : A pointer to this list.
// @para2 : New data to be stored.
// @para3 : Index of new data location. [0,size)
Data addNode(List *list, Data data, int index);

// Remove specified node from the list.
Data removeNode(List *list, int index);

// Get specified node from the list.
Data getData(List *list, int index);

// Get size of the list.
int size(List *list);


void test(List *list){
  Node *nptr;
  int count, i;

  printf("1\n");
  for(i = 0; i < 20; i++){
    printf("i : %d\n", i);
    addNode(list, i, 0);
  }
  printf("2\n");

  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');


  count = 0;
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("count[%d] : %d\n", ++count, nptr->data);
    nptr = nptr->next;
  }
  printf("3\n");

  printf("remove [%2d] ", 3);
  removeNode(list, 3);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", 11);
  removeNode(list, 11);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", 3);
  removeNode(list, 3);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", 0);
  removeNode(list, 0);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", list->size);
  removeNode(list, list->size);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", list->size-1);
  removeNode(list, list->size-1);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("remove [%2d] ", 1);
  removeNode(list, 1);
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("%d,", nptr->data);
    nptr = nptr->next;
  }
  putchar('\n');

  printf("Get => ");
  count = 0;
  while(count < list->size){
    printf("%d,", getData(list, count));
    count++;
  }
  putchar('\n');




  printf("4\n");
  count = 0;
  nptr = list->dummy.next;
  while(nptr != NULL){
    printf("count[%d] : %d\n", ++count, nptr->data);
    nptr = nptr->next;
  }

}


int main(void){
  List int_list;

  initList(&int_list);

  test(&int_list);

  termList(&int_list);

  return 0;
}

// Initialized the list.
void initList(List *list){
  list->dummy.next = NULL;
  list->size = 0;
}

// Terminate the list.
void termList(List *list){
  Node *pre_node, *post_node;

  pre_node = list->dummy.next;
  while(pre_node != NULL){
    post_node = pre_node->next;
    free(pre_node);
    pre_node = post_node;
  }
}

// Add new node to this list.
// @para1 : A pointer to this list.
// @para2 : New data to be stored.
// @para3 : Index of new data location. [0,size)
Data addNode(List *list, Data data, int index){
  Node *new_node, *nptr;
  int count, ret;

  ret = data;

  // error : index < 0 || index > list->size
  index_check(index, 0, list->size);

  def_calloc(new_node, Node *, 1, sizeof(Node));
  new_node->data = data;
  new_node->next = NULL;

  count = 0;
  nptr = &(list->dummy);

  while(count<index && nptr->next!=NULL){
    nptr = nptr->next;
    ++count;
  }

  new_node->next = nptr->next;

  nptr->next = new_node;

  ++(list->size);

  return data;
}

// Remove specified node from this list.
Data removeNode(List *list, int index){
  Node *nptr, *old_nptr;
  int  count, ret;

  index_check(index, 0, list->size-1);
  size_check(list->size, 1);

  count = 0;
  nptr = &(list->dummy);
  while(count<index && nptr->next!=NULL){
    nptr = nptr->next;
    ++count;
  }

  old_nptr = nptr->next;
  ret = old_nptr->data;
  nptr->next = nptr->next->next;

  free(old_nptr);

  --(list->size);

  return ret;
}


// Get specified node from this list.
Data getData(List *list, int index){
  Node *nptr;
  int  count;

  index_check(index, 0, list->size);
  size_check(list->size, 1);

  count = 0;
  nptr = list->dummy.next;
  while(count<index && nptr!=NULL){
    nptr = nptr->next;
    ++count;
  }

  return nptr->data;
}

// Get size of this list.
int size(List *list){
  return list->size;
}

