#ifndef GENERIC_MODEL
#define GENERIC_MODEL

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

#include "../db.h"
#include "../states.h"
#include "../db_config.h"
#include "../linked_list.h"

void * alloc_struct(size_t size);

state model_create(void * data, size_t size, TableDefinition * tb_config);

state model_find(void * key, void * data, size_t size, TableDefinition * tb_config);

state model_list_all(LinkedList * list, size_t size, TableDefinition * tb_config);

state model_update(void * key, void * data, size_t size, TableDefinition * tb_config);

state model_destroy(void * key, size_t size, TableDefinition * tb_config);

#endif
