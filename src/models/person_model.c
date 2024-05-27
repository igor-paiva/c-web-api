#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include "models/person_model.h"

void * get_person_name(void * data);
void * get_person_age(void * data);

void set_person_name(void * data, void * value);
void set_person_age(void * data, void * value);

const ColumnDefinition PERSON_AGE_COLUMN = {
  .name = "age",
  .type = INT32b,
  .getter = &get_person_age,
  .setter = &set_person_age
};

const ColumnDefinition PERSON_NAME_COLUMN = {
  .name = "name",
  .type = STRING,
  .length = PERSON_NAME_LENGTH,
  .getter = &get_person_name,
  .setter = &set_person_name
};

TableDefinition tb_person_def = {
  .name = "person",
  .columns = { &PERSON_NAME_COLUMN, &PERSON_AGE_COLUMN },
  .pk = &PERSON_NAME_COLUMN
};

void * get_person_name(void * data) {
  return ((Person *) data)->name;
}

void set_person_name(void * data, void * value) {
  memccpy(((Person *) data)->name, value, '\0', PERSON_NAME_LENGTH * sizeof(char));
}

void * get_person_age(void * data) {
  return &(((Person *) data)->age);
}

void set_person_age(void * data, void * value) {
  ((Person *) data)->age = *((int *) value);
}

state create(Person * person) {
  return model_create((void *) person, sizeof(Person), &tb_person_def);
}

state find(char * key, Person * person) {
  return model_find(
    (void *) key,
    (void *) person,
    sizeof(Person),
    &tb_person_def
  );
}

state list_all(LinkedList * list) {
  return model_list_all(list, sizeof(Person), &tb_person_def);
}

state update(char * key, Person * person) {
  return model_update(
    (void *) key,
    (void *) person,
    sizeof(Person),
    &tb_person_def
  );
}

state destroy(void * key) {
  return model_destroy(
    (void *) key,
    sizeof(Person),
    &tb_person_def
  );
}
