#include <stdio.h>
#include <string.h>
#include "views/person_view.h"

extern TableDefinition tb_person_def;

state list_person_view(void ** args, char * response) {
  state op_state = SUCCESS;
  char * json_string = NULL;
  cJSON * json = NULL, * list = NULL;

  json = cJSON_CreateObject();

  if (json == NULL) return JSON_ITEM_CREATE_FAIL;

  list = create_entity_list_object((LinkedList *) args[0], &tb_person_def);

  if (list == NULL) {
    op_state = JSON_ITEM_CREATE_FAIL;
    goto end;
  }

  if (!cJSON_AddItemToObject(json, "data", list)) {
    cJSON_Delete(list);
    op_state = JSON_ITEM_ADD_FAIL;
    goto end;
  }

  json_string = cJSON_PrintUnformatted(json);

  if (json_string == NULL) {
    op_state = JSON_ITEM_PRINT_FAIL;
    goto end;
  }

  memccpy(response, json_string, '\0', MAX_REQUEST_DATA * sizeof(char));

end:
  free(json_string);
  cJSON_Delete(json);
  return op_state;
}

state get_person_view(void ** args, char * response) {
  cJSON * json = NULL;
  state op_state = SUCCESS;
  char * json_string = NULL;

  json = create_entity_object((Person *) args[0], &tb_person_def);

  if (json == NULL) return JSON_ITEM_CREATE_FAIL;

  json_string = cJSON_PrintUnformatted(json);

  if (json_string == NULL) {
    op_state = JSON_ITEM_PRINT_FAIL;
    goto end;
  }

  memccpy(response, json_string, '\0', MAX_REQUEST_DATA * sizeof(char));

end:
  free(json_string);
  cJSON_Delete(json);

  return op_state;
}
