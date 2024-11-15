#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <inttypes.h>

#define HTTP_METHOD_LENGTH 8
#define HTTP_STATUS_NAME_LENGTH 100
#define HTTP_FIRST_LINE_LENGTH 1136 /* 8 (method) + 1 + 1025 (path) + 1 + 100 (method name) + 1 */
#define QUERY_PARAM_NAME_LENGTH 50
#define QUERY_PARAM_VALUE_LENGTH 513
#define HTTP_HEADER_NAME_LENGTH 100
#define HTTP_HEADER_VALUE_LENGTH 100
#define MAX_REQUEST_DATA 16384
#define MAX_QUERY_PARAMS 100

typedef uint8_t boolean;

extern const char HTTP_VERSION[9];

typedef struct HttpStatus {
  int status;
  char name[HTTP_STATUS_NAME_LENGTH];
} HttpStatus;

typedef struct HttpHeader {
  char name[HTTP_HEADER_NAME_LENGTH];
  char value[HTTP_HEADER_VALUE_LENGTH];
} HttpHeader;

typedef struct QueryParam {
  char name[QUERY_PARAM_NAME_LENGTH];
  char value[QUERY_PARAM_VALUE_LENGTH];
} QueryParam;

typedef struct HttpHeaderList {
  int count;
  HttpHeader * list;
} HttpHeaderList;

typedef struct QueryParamList {
  int count;
  QueryParam * list;
} QueryParamList;

/* More HTTP status can be added when necessary */

/* 2XX status */
extern const HttpStatus HTTP_OK;
extern const HttpStatus HTTP_CREATED;
extern const HttpStatus HTTP_NO_CONTENT;

/* 4XX status */
extern const HttpStatus HTTP_BAD_REQUEST;
extern const HttpStatus HTTP_UNAUTHORIZED;
extern const HttpStatus HTTP_FORBIDDEN;
extern const HttpStatus HTTP_NOT_FOUND;
extern const HttpStatus HTTP_CONFLICT;
extern const HttpStatus HTTP_IAM_TEAPOT;
extern const HttpStatus HTTP_UNPROCESSABLE_ENTITY;

/* 5XX status */
extern const HttpStatus HTTP_INTERNAL_SERVER_ERROR;

char * mount_initial_line(const HttpStatus * status);

char * mount_content_length(size_t content_size);

char * get_content(char * request);

boolean req_has_body(char method[HTTP_METHOD_LENGTH]);

// QueryParam * get_query_params(char * path, char * route_path, int query_param_num);

// this int represents the state type
int get_query_params(char * path, int * query_param_num, QueryParam ** params);

#endif
