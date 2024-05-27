#include <stdio.h>
#include <string.h>
#include "controllers/controller.h"

boolean set_row_attr(void * data, const ColumnDefinition * column, cJSON * item) {
  void * json_value = NULL;
  boolean valid_type = TRUE;

  if (column->required == TRUE)
    valid_type = compare_json_type(item, column->type);

  if (valid_type) {
    json_value = get_json_value(item, column->type, column->length);

    column->setter(data, json_value);
  }

  free(json_value);
  return valid_type;
}

boolean set_row_data(char * json_string, void * data, TableDefinition  * tb_info) {
  cJSON * current;
  boolean valid = TRUE;
  
  current = cJSON_Parse(json_string);

  while (current) {
    if (current->type == cJSON_Object) {
      current = current->child;
    } else {
      const ColumnDefinition * col_type = find_column(tb_info->columns, current->string);

      boolean ret = set_row_attr(data, col_type, current);

      if (valid) valid = ret;

      current = current->next;
    }
  }

  cJSON_Delete(current);
  return valid;
}

boolean check_query_params(
  QueryParamList * params,
  const char required[][TABLE_NAME_LENGTH],
  int required_count,
  boolean * found_params
) {
  int param_i, required_i;
  boolean all_present = TRUE;

  for (required_i = 0; required_i < required_count; required_i++) {
    for (param_i = 0; param_i < params->count; param_i++) {
      int cmp = strncmp(params->list[param_i].name, required[required_i], QUERY_PARAM_NAME_LENGTH);

      if (cmp == 0) {
        found_params[param_i] = TRUE;
      } else {
        all_present = FALSE;
        found_params[param_i] = FALSE;
      }
    }
  }

  return all_present;
}

cJSON * get_attr(char * json_string, char * key) {
  cJSON * current;
  
  current = cJSON_Parse(json_string);

  while (current) {
    if (current->type == cJSON_Object) {
      current = current->child;
    } else {
      if (strcmp(current->string, key) == 0) return current;

      current = current->next;
    }
  }

  cJSON_Delete(current);

  return NULL;
}
