#ifndef DATABASE_TYPES
#define DATABASE_TYPES

#include <stdio.h>
#include <inttypes.h>
#include "states.h"
#include "cJSON.h"

typedef enum {
  INT8b = 1,
  INT16b,
  INT32b,
  INT64b,
  UNSIGNED_INT8b,
  UNSIGNED_INT16b,
  UNSIGNED_INT32b,
  UNSIGNED_INT64b,
  FLOAT,
  DOUBLE,
  STRING,
  CHAR,
  BOOLEAN
} column_type;

boolean compare(void * v1, void * v2, column_type type, const size_t size);

boolean compare_json_type(cJSON * item, column_type type);

size_t type_size(column_type type, const unsigned long length);

void * get_json_value(cJSON * item, column_type type, const unsigned long length);

cJSON * create_json_object(void * value, column_type type, boolean * string_ref);

// DATE,
// TIME,
// DATETIME

// char * create_date(uint8_t day, uint8_t month, unsigned int year);

// char * create_datetime(
//   uint8_t day,
//   uint8_t month,
//   unsigned int year,
//   uint8_t hours,
//   uint8_t minutes,
//   uint8_t seconds
// );

// time_t create_time(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif
