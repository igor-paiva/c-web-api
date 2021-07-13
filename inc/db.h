#ifndef DATABASE_MANAGEMENT
#define DATABASE_MANAGEMENT

#include <stdio.h>

#include "states.h"
#include "linked_list.h"

#define MAX_FILE_NAME_LENGTH 100

typedef boolean (* CmpFunc)(void * person_data, void * attrs);

state create_file_ifn_exists(const char * file_name);

state register_row(const char * file_name, void * data, size_t size);

state get_all(const char * file_name, LinkedList * list, size_t size);

state find_row(
  const char * file_name,
  size_t size,
  CmpFunc cmp_key,
  void * key,
  size_t * offset
);

state get_row(
  const char * file_name,
  CmpFunc cmp_key,
  void * key,
  void * data,
  size_t data_size
);

state edit_row(
  const char * file_name,
  CmpFunc cmp_key,
  void * key,
  void * new_data,
  size_t data_size
);

state remove_row(
  const char * file_name,
  CmpFunc cmp_key,
  void * key,
  size_t size
);

#endif
