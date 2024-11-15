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

// state query_table(
//   const char * file_name,
//   CmpFunc cmp_attrs,
//   Attr * attrs,
//   LinkedList * list,
//   size_t size
// );

// state copy_to_list(
//   const char * file_name,
//   CmpFunc cmp_key,
//   void * key,
//   size_t size,
//   LinkedList * list
// );

// #define ATTR_LENGTH 50

// typedef enum {
//   OR = 1,
//   AND
// } operator;

// typedef struct Attr {
//   char attr[ATTR_LENGTH];
//   /*
//     if the mult_values is TRUE this we should use this as a pointer array
//     if it's FALSE then we should only access a single value
//     (*value for pointers types and **value for others)
//   */
//   void ** values;
//   boolean regex;
//   boolean mult_values;
//   /* AND, OR and etc with the multiple received fields */
//   operator field_operator;
// } Attr;

#endif
