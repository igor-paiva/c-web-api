#include <stdio.h>
#include "routes.h"

Route person_read = {
  .handler = &get_person,
  .path = "/person",
  .method = "GET",
  .regex = "/person\\?name=[a-zA-Z0-9_]+"
};

Route person_list = {
  .handler = &list_person,
  .path = "/person",
  .method = "GET"
};

Route person_creation = {
  .handler = &create_person,
  .path = "/person",
  .method = "POST"
};

Route person_update = {
  .handler = &update_person,
  .path = "/person",
  .method = "PATCH",
  .regex = "/person\\?name=[a-zA-Z0-9_]+",
};

Route person_delete = {
  .handler = &delete_person,
  .path = "/person",
  .method = "DELETE",
  .regex = "/person\\?name=[a-zA-Z0-9_]+"
};

Route * routes[MAX_ROUTES] = {
  &person_list,
  &person_read,
  &person_creation,
  &person_update,
  &person_delete
};
