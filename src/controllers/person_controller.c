#include <stdio.h>
#include <string.h>
#include "controllers/person_controller.h"

extern TableDefinition tb_person_def;

const HttpStatus * list_person(ControllerActionParam * param) {
  state retrieve_state;
  LinkedList * person_list = NULL;

  person_list = init_list();

  if (person_list == NULL) return &HTTP_INTERNAL_SERVER_ERROR;

  retrieve_state = list_all(person_list);

  if (is_error(retrieve_state)) {
    remove_list(person_list);
    return error_to_http(retrieve_state);
  }

  list_person_view((void **) &person_list, param->request_response);

  remove_list(person_list);

  return &HTTP_OK;
}

const HttpStatus * get_person(ControllerActionParam * param) {
  boolean found_param;
  state retrieve_state;
  Person * person = NULL;
  const char req_params[][COLUMN_NAME_LENGTH] = {"name"};

  person = (Person *) alloc_struct(sizeof(Person));

  if (person == NULL) return &HTTP_INTERNAL_SERVER_ERROR;

  check_query_params(param->params_list, req_params, 1, &found_param);

  if (!found_param) {
    free(person);
    return &HTTP_BAD_REQUEST;
  }

  retrieve_state = find(param->params_list->list[0].value, person);

  if (is_error(retrieve_state)) {
    free(person);
    return error_to_http(retrieve_state);
  }

  get_person_view((void **) &person, param->request_response);

  return &HTTP_OK;
}

const HttpStatus * create_person(ControllerActionParam * param) {
  Person * person;
  boolean data_valid;
  state register_state;

  person = (Person *) alloc_struct(sizeof(Person));

  if (person == NULL) return &HTTP_INTERNAL_SERVER_ERROR;

  data_valid = set_row_data(param->request_data, (void *) person, &tb_person_def);

  if (!data_valid) {
    free(person);
    return &HTTP_UNPROCESSABLE_ENTITY;
  }

  register_state = create(person);

  free(person);

  return state_to_http(register_state);
}

const HttpStatus * update_person(ControllerActionParam * param) {
  Person * person;
  boolean data_valid;
  state update_state;
  boolean found_param;
  const char req_params[][COLUMN_NAME_LENGTH] = {"name"};

  person = (Person *) alloc_struct(sizeof(Person));

  if (person == NULL) return &HTTP_INTERNAL_SERVER_ERROR;

  check_query_params(param->params_list, req_params, 1, &found_param);

  if (!found_param) {
    free(person);
    return &HTTP_BAD_REQUEST;
  }

  data_valid = set_row_data(param->request_data, (void *) person, &tb_person_def);

  if (!data_valid) {
    free(person);
    return &HTTP_UNPROCESSABLE_ENTITY;
  }

  update_state = update(param->params_list->list[0].value, person);

  if (is_error(update_state)) {
    free(person);
    return error_to_http(update_state);
  }
  
  free(person);

  return &HTTP_OK;
}

const HttpStatus * delete_person(ControllerActionParam * param) {
  state delete_state;
  boolean found_param;
  const char req_params[][COLUMN_NAME_LENGTH] = {"name"};

  check_query_params(param->params_list, req_params, 1, &found_param);

  if (!found_param) return &HTTP_BAD_REQUEST;

  delete_state = destroy(param->params_list->list[0].value);

  if (is_error(delete_state)) return error_to_http(delete_state);

  return &HTTP_OK;
}
