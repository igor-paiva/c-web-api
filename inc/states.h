#ifndef STATES_HANDLING
#define STATES_HANDLING

#include <stdio.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

typedef int boolean;

/* general status, could be either a success_t or an error_t */
typedef int state;

/* those will always be positive */
typedef enum {
  SUCCESS = 1,
  CREATED,
  UPDATED,
  DELETED
} success_t;

/* those will always be negative */
typedef enum {
 NOT_FOUND = INT_MIN,
 ALLOC_FAIL,
 UNABLE_OPEN_FILE,
 UNABLE_READ_FILE,
 UNABLE_WRITE_FILE,
 UNABLE_CREATE_FILE,
} error_t;

boolean is_error(state value);

boolean is_success(state value);

#endif
