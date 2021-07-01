#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdio.h>
#include "states.h"

typedef struct ListElement {
  void * data;
  struct ListElement * next;
} ListElement;

typedef struct LinkedList {
  ListElement * head;
  ListElement * tail;
} LinkedList;

LinkedList * init_list();

void remove_list(LinkedList * list);

state push_list(LinkedList * list, void * data, size_t size);

#endif
