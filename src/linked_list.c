#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

LinkedList * init_list() {
  LinkedList * list;

  list = (LinkedList *) malloc(sizeof(LinkedList));

  if (list == NULL) return NULL;

  list->head = NULL;
  list->tail = NULL;

  return list;
}

void remove_list(LinkedList * list) {
  if (list == NULL) return;
  
  ListElement * current = list->head;

  while (current) {
    ListElement * tmp = current;

    current = current->next;

    free(tmp->data);
    free(tmp);
  }

  free(list);
}

state push_list(LinkedList * list, void * data, size_t size) {
  void * data_copy = malloc(size);
  ListElement * el = (ListElement *) malloc (sizeof(ListElement));

  if (el == NULL || data_copy == NULL) return ALLOC_FAIL;

  memcpy(data_copy, data, size);

  el->next = NULL;
  el->data = data_copy;

  if (list->head) {
    list->tail->next = el;

    list->tail = el;
  } else {
    list->head = el;
    list->tail = el;
  }

  return SUCCESS;
}
