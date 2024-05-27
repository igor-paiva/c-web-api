#ifndef GENERIC_VIEW
#define GENERIC_VIEW

#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "db_config.h"
#include "linked_list.h"

cJSON * create_entity_object(void * data, TableDefinition  * tb_info);

cJSON * create_entity_list_object(LinkedList * person_list, TableDefinition  * tb_info);

#endif
