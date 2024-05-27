#ifndef DATABASE_CONFIG
#define DATABASE_CONFIG

#include <malloc.h>
#include <pthread.h>
#include <sys/types.h>
#include "db_types.h"

#define MAX_TABLES 50
#define MAX_COLUMNS 50
#define TABLE_NAME_LENGTH 50
#define COLUMN_NAME_LENGTH 50

typedef void * (*FieldGetter)(void * data);
typedef void (*FieldSetter)(void * data, void * value);

typedef struct ColumnDefinition {
  char name[COLUMN_NAME_LENGTH];
  column_type type;
  FieldGetter getter;
  FieldSetter setter;
  boolean required;
  unsigned long length;
  struct DBReference {
    char table_name[TABLE_NAME_LENGTH];
    struct ColumnDefinition * field;
  } reference;
} ColumnDefinition;

typedef struct DBReference DBReference;

typedef struct TableDefinition {
  const char name[TABLE_NAME_LENGTH];
  const ColumnDefinition * columns[MAX_COLUMNS];
  pthread_mutex_t mutex;
  const ColumnDefinition * pk;
} TableDefinition;

const ColumnDefinition * find_column(const ColumnDefinition * columns[MAX_COLUMNS], char * column_name);

void load_database_info(TableDefinition ** tables_config, int * tables_count);

void init_mutexes(TableDefinition ** tables_config, int * tables_count);

void destroy_mutexes(TableDefinition ** tables_config, int * tables_count);

#endif
