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

state check_primary_key_constraint(void * key, size_t size, TableDefinition * tb_config) {
  state exists_state = model_record_exists(key, size, tb_config);

  if (exists_state == FOUND) return ALREADY_EXISTS;

  if (is_error(exists_state) && exists_state != NOT_FOUND) return exists_state;

  return SUCCESS;
}

state model_create(void * data, size_t size, TableDefinition * tb_config, void * key) {
  char * file_name;
  state op_state = CREATED;

  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  op_state = check_primary_key_constraint(key, size, tb_config);

  if (is_error(op_state)) goto end;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = register_row(file_name, data, size);

  pthread_mutex_unlock(&tb_config->mutex);

end:
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

state model_record_exists(
  void * key,
  size_t size,
  TableDefinition * tb_config
) {
  char * file_name;
  state op_state = SUCCESS;

  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  pthread_mutex_lock(&tb_config->mutex);

  op_state = check_row_exists(file_name, tb_config->pk, key, size);

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
  boolean key_cmp;
  char * file_name;
  state op_state = UPDATED;

  file_name = get_file_name(tb_config->name);

  if (file_name == NULL) return ALLOC_FAIL;

  key_cmp = compare(
    (*tb_config->pk->getter)(data), key, tb_config->pk->type, tb_config->pk->length
  );

  /* If search primary key is different than the model replace value */
  if (!key_cmp) {
    op_state = check_primary_key_constraint(key, size, tb_config);

    /* If no record was found should return not found instead */
    if (op_state == SUCCESS) {
      op_state = NOT_FOUND;
      goto end;
    }

    op_state = check_primary_key_constraint((*tb_config->pk->getter)(data), size, tb_config);

    if (is_error(op_state)) goto end;
  }

  pthread_mutex_lock(&tb_config->mutex);

  op_state = edit_row(file_name, tb_config->pk, key, data, size);

  pthread_mutex_unlock(&tb_config->mutex);

end:
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
