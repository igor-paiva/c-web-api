#ifndef APPLICATION_ROUTES
#define APPLICATION_ROUTES

#include <stdio.h>
#include <regex.h>
#include "http.h"
#include "controllers/person_controller.h"

#define MAX_ROUTES 255
#define ROUTE_PATH_LENGTH 1025
#define ROUTE_REGEX_LENGTH 513

typedef const HttpStatus * (* OpHandlerFunc)(ControllerActionParam * param);

typedef struct Route {
  char regex[ROUTE_REGEX_LENGTH];
  OpHandlerFunc handler;
  char path[ROUTE_PATH_LENGTH];
  char method[HTTP_METHOD_LENGTH];
} Route;

Route * routes[MAX_ROUTES];

#endif
