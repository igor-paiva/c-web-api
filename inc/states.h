#ifndef STATES_HANDLING
#define STATES_HANDLING

#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include "http.h"

#define TRUE 1
#define FALSE 0

typedef uint8_t boolean;

/* general status, could be either a success_t or an error_t */
typedef int state;

/* those will always be positive */
typedef enum {
  SUCCESS = 1,
  CREATED,
  UPDATED,
  DELETED,
  NOT_APPLICABLE,
  FOUND
} success_t;

/* those will always be negative */
typedef enum {
  NOT_FOUND = INT_MIN,
  ALLOC_FAIL,
  UNABLE_OPEN_FILE,
  INVALID_ROW_DATA,
  UNABLE_READ_FILE,
  UNABLE_WRITE_FILE,
  UNABLE_CREATE_FILE,
  UNABLE_MOVE_FILE,
  NO_QUERY_PARAM,
  PRINT_FAIL,
  JSON_ITEM_ADD_FAIL,
  JSON_ITEM_PRINT_FAIL,
  JSON_ITEM_CREATE_FAIL,
  INVALID_ROUTE_PATH,
  ALREADY_EXISTS
} error_t;

boolean is_error(state value);

boolean is_success(state value);

const HttpStatus * success_to_http(state value);

const HttpStatus * error_to_http(state value);

const HttpStatus * state_to_http(state value);

#endif
