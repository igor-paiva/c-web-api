#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

state create_file_ifn_exists(const char * file_name) {
  FILE * file;

  file = fopen(file_name, "rb");

  if (file == NULL) {
    file = fopen(file_name, "wb");

    if (!file) return UNABLE_OPEN_FILE;
  }

  fclose(file);

  return SUCCESS;
}

state register_row(const char * file_name, void * data, size_t size) {
  size_t ret;
  FILE * file;

  file = fopen(file_name, "ab");

  if (!file) return UNABLE_OPEN_FILE;

  ret = fwrite(data, size, 1, file);

  /* it should write only one member */
  if (ret != 1) return UNABLE_WRITE_FILE;

  fclose(file);

  return CREATED;
}

state get_all(const char * file_name, LinkedList * list, size_t size) {
  FILE * file;
  void * current;

  current = malloc(size);

  if (current == NULL) return ALLOC_FAIL;

  file = fopen(file_name, "rb");

  if (!file) return UNABLE_OPEN_FILE;

  while(fread(current, size, 1, file) > 0) {
    state push_state = push_list(list, current, size);

    if (is_error(push_state)) {
      fclose(file);
      return push_state;
    }
  };

  fclose(file);

  return SUCCESS;
}

state find_row(
  const char * file_name,
  size_t size,
  CmpFunc cmp_key,
  void * key,
  size_t * offset
) {
  FILE * file;
  int count = 0;
  void * current;
  boolean found = FALSE;

  file = fopen(file_name, "rb");

  if (!file) return UNABLE_OPEN_FILE;

  while(fread(current, size, 1, file) > 0) {
    if ((*cmp_key)(current, key)) {
      found = TRUE;
      break;
    }

    count++;
  }

  fclose(file);

  if (found == FALSE) return NOT_FOUND;

  *offset = count * size;

  return SUCCESS;
}

state get_row(
  const char * file_name,
  CmpFunc cmp_key,
  void * key,
  void * data,
  size_t data_size
) {
  FILE * file;
  size_t offset;
  void * row_data;
  state find_state;

  find_state = find_row(file_name, data_size, cmp_key, key, &offset);

  if (is_error(find_state)) return find_state;

  file = fopen(file_name, "rb");

  if (!file) return UNABLE_OPEN_FILE;

  fseek(file, offset, SEEK_SET);

  if (fread(row_data, data_size, 1, file) != 1) {
    fclose(file);
    return UNABLE_READ_FILE;
  }

  fclose(file);

  memcpy(data, row_data, data_size);

  return SUCCESS;
}
