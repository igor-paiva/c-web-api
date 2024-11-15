#include <stdio.h>
#include "db_config.h"
#include "models/person_model.h"

extern TableDefinition tb_person_def;

const ColumnDefinition * find_column(const ColumnDefinition * columns[MAX_COLUMNS], char * column_name) {
  int i;

  for (i = 0; i < MAX_COLUMNS; i++) {
    if (!columns[i]) break;

    if (strncmp(columns[i]->name, column_name, COLUMN_NAME_LENGTH) == 0)
      return columns[i];
  }

  return NULL;
}

state load_database_info(TableDefinition ** tables_config, int * tables_count) {
  state table_file = SUCCESS;

  tables_config[(*tables_count)++] = &tb_person_def;

  for (int i = 0; i < *tables_count; i++) {
    table_file = create_file_ifn_exists(tables_config[i]->name);

    if (is_error(table_file)) break;
  }

  return table_file;
}

void init_mutexes(TableDefinition ** tables_config, int * tables_count) {
  int i;
  for (i = 0; i < *tables_count; i++) {
    pthread_mutex_init(&tables_config[i]->mutex, NULL);
  }
}

void destroy_mutexes(TableDefinition ** tables_config, int * tables_count) {
  int i;
  for (i = 0; i < *tables_count; i++) {
    pthread_mutex_destroy(&tables_config[i]->mutex);
  }
}
