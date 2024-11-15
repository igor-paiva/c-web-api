#include <stdio.h>
#include "http.h"
#include "states.h"

#define TRUE 1
#define FALSE 0

const char HTTP_VERSION[] = "HTTP/1.1";

/* More HTTP status can be added when necessary */

/* 2XX status */
const HttpStatus HTTP_OK = { .status = 200, .name = "OK" };
const HttpStatus HTTP_CREATED = { .status = 201, .name = "Created" };
const HttpStatus HTTP_NO_CONTENT = { .status = 204, .name = "No Content" };

/* 4XX status */
const HttpStatus HTTP_BAD_REQUEST = { .status = 400, .name = "Bad Request" };
const HttpStatus HTTP_UNAUTHORIZED = { .status = 401, .name = "Unauthorized" };
const HttpStatus HTTP_FORBIDDEN = { .status = 403, .name = "Forbidden" };
const HttpStatus HTTP_NOT_FOUND = { .status = 404, .name = "Not Found" };
const HttpStatus HTTP_CONFLICT = { .status = 409, .name = "Conflict" };
const HttpStatus HTTP_IAM_TEAPOT = { .status = 418, .name = "I'm a teapot" };
const HttpStatus HTTP_UNPROCESSABLE_ENTITY = { .status = 422, .name = "Unprocessable Entity" };

/* 5XX status */
const HttpStatus HTTP_INTERNAL_SERVER_ERROR = { .status = 500, .name = "Internal Server Error" };

boolean req_has_body(char method[HTTP_METHOD_LENGTH]) {
  return (
    strncmp(method, "POST", HTTP_METHOD_LENGTH) == 0
    || strncmp(method, "PUT", HTTP_METHOD_LENGTH) == 0
    || strncmp(method, "PATCH", HTTP_METHOD_LENGTH) == 0
  );
}

char * mount_initial_line(const HttpStatus * status) {
  char * initial_line;

  initial_line = (char *) malloc(HTTP_FIRST_LINE_LENGTH * sizeof(char));

  sprintf(initial_line, "%s %d %s\n", HTTP_VERSION, status->status, status->name);

  return initial_line;
}

char * mount_content_length(size_t content_size) {
  char * content_line;

  content_line = (char *) malloc(50 * sizeof(char));

  sprintf(content_line, "Content-Length: %ld\n", content_size);

  return content_line;
}

char * get_content(char * request) {
  unsigned int i = 0, j = 0;
  char * content, line[150];

  content = (char *) malloc(MAX_REQUEST_DATA * sizeof(char));

  while (j < strlen(request)) {
    if (request[j] == '\n') {
      if (strcmp(line, "\r") == 0 || strcmp(line, "\n") == 0) {
        strcpy(content, &request[j]);
        break;
      }

      j++;
      i = 0;
      memset(line, 0, 150);
    } else {
      line[i++] = request[j++];
    }
  }

  return content;
}

state get_query_params(char * path, int * params_count, QueryParam ** params) {
  char * ptr;
  char * start_position = strchr(path, '?');
  char format[QUERY_PARAM_NAME_LENGTH + QUERY_PARAM_VALUE_LENGTH + 2];

  *params_count = 0;

  if (start_position == NULL || (start_position && start_position[1] == '='))
    return INVALID_ROUTE_PATH;

  *params = (QueryParam *) malloc (MAX_QUERY_PARAMS * sizeof(QueryParam));

  if (*params == NULL) return ALLOC_FAIL;

  ptr = start_position + sizeof(char);

  sprintf(format, "%c%d[^=]=%c%d[^& ;]", '%', QUERY_PARAM_NAME_LENGTH, '%', QUERY_PARAM_VALUE_LENGTH);

  sscanf(ptr, format, (*params)[*params_count].name, (*params)[*params_count].value);

  if (strlen((*params)[*params_count].value) <= 0) return INVALID_ROUTE_PATH;

  *params_count += 1;

  while (TRUE) {
    ptr = strchr(path, '&');

    if (ptr == NULL) {
      ptr = strchr(path, ';');

      if (ptr == NULL) break;
    }

    ptr += sizeof(char);

    sscanf(ptr, format, (*params)[*params_count].name, (*params)[*params_count].value);

    *params_count += 1;
  }

  if (*params_count < MAX_QUERY_PARAMS) {
    *params = (QueryParam *) realloc (*params, *params_count * sizeof(QueryParam));

    if (*params == NULL) return ALLOC_FAIL;
  }

  return SUCCESS;
}
