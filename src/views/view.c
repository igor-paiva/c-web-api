#include <stdio.h>
#include <string.h>
#include "views/view.h"

cJSON * create_entity_object(void * data, TableDefinition  * tb_info) {
  int i;
  cJSON * object;

  object = cJSON_CreateObject();

  if (object == NULL) return NULL;

  for (i = 0; i < MAX_COLUMNS; i++) {
    cJSON * item = NULL;
    const ColumnDefinition * column = tb_info->columns[i];

    if (!column) break;

    item = create_json_object(column->getter(data), column->type, NULL);

    if (item == NULL || !cJSON_AddItemToObject(object, column->name, item)) {
      cJSON_Delete(item);
      cJSON_Delete(object);
      return NULL;
    }
  }

  return object;
}

cJSON * create_entity_list_object(LinkedList * person_list, TableDefinition  * tb_info) {
  cJSON * list = NULL;
  ListElement * current_el = person_list->head;

  list = cJSON_CreateArray();

  if (list == NULL) return NULL;

  while (current_el) {
    cJSON * object = NULL;

    object = create_entity_object(current_el->data, tb_info);

    if (object == NULL || !cJSON_AddItemToArray(list, object)) {
      cJSON_Delete(object);
      cJSON_Delete(list);
      return NULL; 
    }

    current_el = current_el->next;
  }

  return list;
}
