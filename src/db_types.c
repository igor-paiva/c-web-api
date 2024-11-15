#include <stdio.h>
#include <malloc.h>
#include "db_types.h"

boolean compare(void * v1, void * v2, column_type type, const size_t size) {
  switch (type) {
    case STRING: {
      if (size <= 0)
        return strcmp((char *) v1, (char *) v2) == 0;

      return strncmp((char *) v1, (char *) v2, size) == 0;
    }
    case CHAR:
      return *((char *) v1) == *((char *) v2);
    case INT8b:
      return *((int8_t *) v1) == *((int8_t *) v2);
    case INT16b:
      return *((int16_t *) v1) == *((int16_t *) v2);
    case INT32b:
      return *((int32_t *) v1) == *((int32_t *) v2);
    case INT64b:
      return *((int64_t *) v1) == *((int64_t *) v2);
    case BOOLEAN:
    case UNSIGNED_INT8b:
      return *((uint8_t *) v1) == *((uint8_t *) v2);
    case UNSIGNED_INT16b:
      return *((uint16_t *) v1) == *((uint16_t *) v2);
    case UNSIGNED_INT32b:
      return *((uint32_t *) v1) == *((uint32_t *) v2);
    case UNSIGNED_INT64b:
      return *((uint64_t *) v1) == *((uint64_t *) v2);
    case FLOAT:
      return *((float *) v1) == *((float *) v2);
    case DOUBLE:
      return *((double *) v1) == *((double *) v2);
    default:
      return memcmp(v1, v2, size) == 0;
  }
}

boolean compare_json_type(cJSON * item, column_type type) {
  switch (type) {
    case STRING:
      return item->type == cJSON_String;
    default:
      return item->type == cJSON_Number;
  }
}

size_t type_size(column_type type, const unsigned long length) {
  switch (type) {
    case STRING:
      return length * sizeof(char);
    case CHAR:
      return sizeof(char);
    case BOOLEAN:
    case UNSIGNED_INT8b:
      return sizeof(uint8_t);
    case UNSIGNED_INT16b:
      return sizeof(uint16_t);
    case UNSIGNED_INT32b:
      return sizeof(uint32_t);
    case UNSIGNED_INT64b:
      return sizeof(uint64_t);
    case INT8b:
      return sizeof(int8_t);
    case INT16b:
      return sizeof(int16_t);
    case INT32b:
      return sizeof(int32_t);
    case INT64b:
      return sizeof(int64_t);
    case FLOAT:
      return sizeof(float);
    case DOUBLE:
      return sizeof(double);
    default:
      return 0;
  }
}

void * get_json_value(cJSON * item, column_type type, const unsigned long length) {
  void * value;
  size_t value_size = type_size(type, length);

  value = malloc(value_size);

  if (value == NULL) return NULL;

  switch (type) {
    case STRING: {
      memccpy(value, item->valuestring, '\0', length * sizeof(char));
      break;
    }
    case CHAR: {
      char casted_value = (char) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case INT8b: {
      int8_t casted_value = (int8_t) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case INT16b: {
      int16_t casted_value = (int16_t) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case INT32b: {
      int32_t casted_value = (int32_t) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case INT64b: {
      int64_t casted_value = (int64_t) item->valuedouble;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case BOOLEAN:
    case UNSIGNED_INT8b: {
      uint8_t casted_value = (uint8_t) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case UNSIGNED_INT16b: {
      uint16_t casted_value = (uint16_t) item->valueint;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case UNSIGNED_INT32b: {
      uint32_t casted_value = (uint32_t) item->valuedouble;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case UNSIGNED_INT64b: {
      uint64_t casted_value = (uint64_t) item->valuedouble;
      memcpy(value, &casted_value, value_size);
      break;
    }
    case FLOAT: {
      float casted_value = (float) item->valuedouble;
      memcpy(value, &casted_value, value_size);
      break;
    }
    /* DOUBLE and other types will be copied as a double */
    default:
      memcpy(value, &item->valuedouble, value_size);
      break;
  }

  return value;
}

cJSON * create_json_object(void * value, column_type type, boolean * string_ref) {
  switch (type) {
    case STRING:
      if (string_ref != NULL && *string_ref) return cJSON_CreateString((char *) value);

      return cJSON_CreateStringReference((char *) value);
    case CHAR:
      return cJSON_CreateNumber((double) *((char *) value));
    case BOOLEAN:
    case UNSIGNED_INT8b:
      return cJSON_CreateNumber((double) *((uint8_t *) value));
    case UNSIGNED_INT16b:
      return cJSON_CreateNumber((double) *((uint16_t *) value));
    case UNSIGNED_INT32b:
      return cJSON_CreateNumber((double) *((uint32_t *) value));
    case UNSIGNED_INT64b:
      return cJSON_CreateNumber((double) *((uint64_t *) value));
    case INT8b:
      return cJSON_CreateNumber((double) *((int8_t *) value));
    case INT16b:
      return cJSON_CreateNumber((double) *((int16_t *) value));
    case INT32b:
      return cJSON_CreateNumber((double) *((int32_t *) value));
    case INT64b:
      return cJSON_CreateNumber((double) *((int64_t *) value));
    case FLOAT:
      return cJSON_CreateNumber((double) *((float *) value));
    default:
      return cJSON_CreateNumber(*((double *) value));
  }
}
