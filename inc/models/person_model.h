#ifndef PERSON_MODEL
#define PERSON_MODEL

#include <stdio.h>

#include "../db.h"
#include "model.h"
#include "../db_config.h"
#include "../states.h"

#define PERSON_NAME_LENGTH 100

typedef struct Person {
  int age;
  char name[PERSON_NAME_LENGTH];
} Person;

const ColumnDefinition PERSON_NAME_COLUMN;
const ColumnDefinition PERSON_AGE_COLUMN;

state find(char * key, Person * person);

state list_all(LinkedList * list);

state create(Person * person);

state update(char * key, Person * person);

state destroy(void * key);

#endif
