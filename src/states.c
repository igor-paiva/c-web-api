#include <stdio.h>
#include "states.h"

boolean is_error(state value) {
  return value < 0;
}

boolean is_success(state value) {
  return value > 0;
}

const HttpStatus * success_to_http(state value) {
  if (value < 0) return NULL;

  switch (value) {
    case CREATED:
      return &HTTP_CREATED;
    default:
      return &HTTP_OK;
  }
}

const HttpStatus * error_to_http(state value) {
  if (value > 0) return NULL;

  switch (value) {
    case NOT_FOUND:
      return &HTTP_NOT_FOUND;
    case INVALID_ROW_DATA:
      return &HTTP_UNPROCESSABLE_ENTITY;
    case INVALID_ROUTE_PATH:
      return &HTTP_BAD_REQUEST;
    default:
      return &HTTP_INTERNAL_SERVER_ERROR;
  }
}

const HttpStatus * state_to_http(state value) {
  const HttpStatus * success_status = success_to_http(value);

  if (success_status) return success_status;

  return error_to_http(value);
}
