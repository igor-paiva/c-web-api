#include <stdio.h>
#include "routes.h"

Route person_read = {
  .handler = &get_person,
  .path = "/person",
  .method = "GET"
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

// Route person_query = {
// };

Route person_update = {
  .handler = &update_person,
  .path = "/person",
  .method = "PATCH"
};

Route person_delete = {
  .handler = &delete_person,
  .path = "/person",
  .method = "DELETE"
};

Route * routes[MAX_ROUTES] = {
  &person_list,
  &person_read,
  // &person_query,
  &person_creation,
  &person_update,
  &person_delete
};

// void save_routes(Route ** routes, int * routes_num) {
//   routes[(*routes_num)++] = &person_read;

//   routes[(*routes_num)++] = &person_list;

//   routes[(*routes_num)++] = &person_creation;

//   routes[(*routes_num)++] = &person_update;

//   routes[(*routes_num)++] = &person_delete;
// }
