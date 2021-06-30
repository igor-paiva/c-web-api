#ifndef STATES_HANDLING
#define STATES_HANDLING

#include <stdio.h>
#include <limits.h>

/* general status, could be either a success_t or an error_t */
typedef int state;

/* those will always be positive */
typedef enum {
  SUCCESS = 1,
  CREATED
} success_t;

/* those will always be negative */
typedef enum {
 UNABLE_OPEN_FILE = INT_MIN,
 UNABLE_WRITE_FILE,
} error_t;

#endif
