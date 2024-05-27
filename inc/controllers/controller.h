#ifndef GENERIC_CONTROLLER
#define GENERIC_CONTROLLER

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <inttypes.h>
#include "cJSON.h"
#include "http.h"
#include "states.h"
#include "db_config.h"

typedef struct ControllerActionParam {
  char * request_data;
  char * request_response;
  QueryParamList * params_list;
} ControllerActionParam;

boolean set_row_attr(void * data, const ColumnDefinition * column, cJSON * item);

boolean set_row_data(char * json_string, void * data, TableDefinition * tb_info);

cJSON * get_attr(char * json_string, char * key);

boolean check_query_params(
  QueryParamList * params,
  const char required[][TABLE_NAME_LENGTH],
  int required_count,
  boolean * found_params
);

#endif
