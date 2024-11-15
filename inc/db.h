#ifndef DATABASE_MANAGEMENT
#define DATABASE_MANAGEMENT

#include <stdio.h>

#include "states.h"
#include "db_types.h"
#include "db_config.h"
#include "linked_list.h"

#define MAX_FILE_NAME_LENGTH 50
#define DATA_FILE_EXTENSION ".bin"

extern const char files_folder[9];

char * get_file_name(const char * tb_name);

state create_file_ifn_exists(const char * file_name);

state register_row(const char * file_name, void * data, size_t size);

state get_all(const char * file_name, LinkedList * list, size_t size);

state find_row(
  const char * file_name,
  size_t size,
  const ColumnDefinition * pk,
  void * key,
  size_t * offset
);

state get_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  void * data,
  size_t data_size
);

state edit_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  void * new_data,
  size_t data_size
);

state remove_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  size_t size
);

#endif
