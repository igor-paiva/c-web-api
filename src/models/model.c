#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include "models/model.h"

void * alloc_struct(size_t size) {
  void * entity;

  entity = malloc(size);

  return entity;
}

state model_create(void * data, size_t size, TableDefinition * tb_config) {
  char * file_name;
  state op_state = CREATED;
  
  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = register_row(file_name, data, size);

  pthread_mutex_unlock(&tb_config->mutex);

  free(file_name);
  return op_state;
}

state model_find(
  void * key,
  void * data,
  size_t size,
  TableDefinition * tb_config
) {
  char * file_name;
  state op_state = SUCCESS;
  
  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = get_row(file_name, tb_config->pk, key, data, size);

  pthread_mutex_unlock(&tb_config->mutex);

  free(file_name);
  return op_state;
}

state model_list_all(LinkedList * list, size_t size, TableDefinition * tb_config) {
  char * file_name;
  state op_state = SUCCESS;
  
  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = get_all(file_name, list, size);

  pthread_mutex_unlock(&tb_config->mutex);

  free(file_name);
  return op_state;
}

state model_update(
  void * key,
  void * data,
  size_t size,
  TableDefinition * tb_config
) {
  char * file_name;
  state op_state = UPDATED;
  
  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = edit_row(file_name, tb_config->pk, key, data, size);

  pthread_mutex_unlock(&tb_config->mutex);

  free(file_name);
  return op_state;
}

state model_destroy(
  void * key,
  size_t size,
  TableDefinition * tb_config
) {
  char * file_name;
  state op_state = DELETED;
  
  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = remove_row(file_name, tb_config->pk, key, size);

  pthread_mutex_unlock(&tb_config->mutex);

  free(file_name);
  return op_state;
}

// state model_query_table(
//   LinkedList * list,
//   size_t size,
//   CmpFunc cmp_attr,
//   Attr * attrs,
//   TableDefinition * tb_config
// ) {
//   char * file_name;
//   state op_state = SUCCESS;
  
//   file_name = get_file_name(tb_config->name);

//   if (file_name == NULL) return ALLOC_FAIL;

//   pthread_mutex_lock(&tb_config->mutex);

//   op_state = query_table(file_name, cmp_attr, attrs, list, size);

//   pthread_mutex_unlock(&tb_config->mutex);

//   free(file_name);
//   return op_state;
// }
