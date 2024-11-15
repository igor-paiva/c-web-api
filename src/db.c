#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

const char files_folder[] = "app_data";

state create_file_ifn_exists(const char * file_name) {
  FILE * file;

  file = fopen(file_name, "rb");

  if (file == NULL) {
    file = fopen(file_name, "wb");

    if (file == NULL) return UNABLE_CREATE_FILE;
  }

  fclose(file);

  return SUCCESS;
}

char * get_file_name(const char * tb_name) {
  int sprintf_ret;
  char * file_name;

  file_name = (char *) malloc (MAX_FILE_NAME_LENGTH * sizeof(char));

  if (file_name == NULL) return NULL;

  sprintf_ret = sprintf(file_name, "%s/%s%s", files_folder, tb_name, DATA_FILE_EXTENSION);

  if (sprintf_ret <= 0) return NULL;

  return file_name;
}

state register_row(const char * file_name, void * data, size_t size) {
  size_t ret;
  FILE * file;
  state op_state = CREATED;

  file = fopen(file_name, "ab");

  if (file == NULL) return UNABLE_OPEN_FILE;

  ret = fwrite(data, size, 1, file);

  /* it should write only one member */
  if (ret != 1) op_state = UNABLE_WRITE_FILE;

  fclose(file);

  return op_state;
}

state get_all(const char * file_name, LinkedList * list, size_t size) {
  FILE * file;
  void * current;
  state op_state = SUCCESS;

  current = malloc(size);

  if (current == NULL) return ALLOC_FAIL;

  file = fopen(file_name, "rb");

  if (file == NULL) {
    free(current);
    return UNABLE_OPEN_FILE;
  }

  /* it should read only one member */
  while(fread(current, size, 1, file) == 1) {
    state push_state = push_list(list, current, size);

    if (is_error(push_state)) {
      op_state = push_state;
      break;
    }
  };

  fclose(file);
  free(current);

  return op_state;
}

state find_row(
  const char * file_name,
  size_t size,
  const ColumnDefinition * pk,
  void * key,
  size_t * offset
) {
  FILE * file;
  int count = 0;
  void * current;
  boolean found = FALSE;

  current = malloc(size);

  if (current == NULL) return ALLOC_FAIL;

  file = fopen(file_name, "rb");

  if (file == NULL) {
    free(current);
    return UNABLE_OPEN_FILE;
  }

  /* it should read only one member */
  while(fread(current, size, 1, file) == 1) {
    if (compare((*pk->getter)(current), key, pk->type, pk->length)) {
      found = TRUE;
      break;
    }

    count++;
  }

  fclose(file);
  free(current);

  if (found == FALSE) return NOT_FOUND;

  *offset = count * size;

  return SUCCESS;
}

state get_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  void * data,
  size_t data_size
) {
  FILE * file;
  state find_state;
  void * row_data = NULL;
  size_t offset, read_ret;
  state op_state = SUCCESS;

  find_state = find_row(file_name, data_size, pk, key, &offset);

  if (is_error(find_state)) return find_state;

  row_data = malloc(data_size);

  if (row_data == NULL) return ALLOC_FAIL;

  file = fopen(file_name, "rb");

  if (file == NULL) {
    free(row_data);
    return UNABLE_OPEN_FILE;
  }

  fseek(file, offset, SEEK_SET);

  read_ret = fread(row_data, data_size, 1, file);

  fclose(file);

  /* it should read only one member */
  if (read_ret == 1) {
    memcpy(data, row_data, data_size);
  } else {
    op_state = UNABLE_READ_FILE;
  }

  free(row_data);

  return op_state;
}

state edit_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  void * new_data,
  size_t data_size
) {
  FILE * file;
  state find_state;
  size_t offset, ret;

  find_state = find_row(file_name, data_size, pk, key, &offset);

  if (is_error(find_state)) return find_state;

  file = fopen(file_name, "rb+");

  if (file == NULL) return UNABLE_OPEN_FILE;

  fseek(file, offset, SEEK_SET);

  ret = fwrite(new_data, data_size, 1, file);

  fclose(file);

  if (ret != 1) return UNABLE_WRITE_FILE;

  return UPDATED;
}

state remove_row(
  const char * file_name,
  const ColumnDefinition * pk,
  void * key,
  size_t size
) {
  void * current = NULL;
  char * data_file_name;
  state op_state = DELETED;
  FILE * db_file, * tmp_file;
  char cpy_file_name[MAX_FILE_NAME_LENGTH];
  char tmp_file_name[MAX_FILE_NAME_LENGTH];

  current = malloc(size);

  if (current == NULL) return ALLOC_FAIL;

  /* cannot modify the file name, so its being copied */
  strncpy(cpy_file_name, file_name, MAX_FILE_NAME_LENGTH);

  data_file_name = (
    /* + 1 to include the "/" */
    strtok(cpy_file_name, "/") + ((strlen(files_folder) + 1) * sizeof(char))
  );

  sprintf(tmp_file_name, "tmp/%s", data_file_name);

  tmp_file = fopen(tmp_file_name, "wb");

  if (tmp_file == NULL) {
    free(current);
    return UNABLE_CREATE_FILE;
  }

  db_file = fopen(file_name, "rb");

  if (db_file == NULL) {
    op_state = UNABLE_OPEN_FILE;
    goto end;
  }

  /* it should read only one member */
  while(fread(current, size, 1, db_file) == 1) {
    boolean cmp = compare((*pk->getter)(current), key, pk->type, pk->length);

    if (!cmp) {
      size_t ret = fwrite(current, size, 1, tmp_file);

      if (ret != 1) {
        fclose(db_file);
        op_state = UNABLE_WRITE_FILE;
        goto end;
      }
    }
  }

end:
  free(current);
  fclose(tmp_file);

  if (is_success(op_state)) {
    /* move the temporary file to override app_data file */
    if (rename(tmp_file_name, file_name) != 0)
      op_state = UNABLE_MOVE_FILE;
  }

  return op_state;
}
