#ifndef PERSON_CONTROLLER
#define PERSON_CONTROLLER

#include <stdio.h>
#include <string.h>

#include "controller.h"
#include "../db.h"
#include "../http.h"
#include "../states.h"
#include "../db_config.h"
#include "../linked_list.h"
#include "../views/person_view.h"
#include "../models/person_model.h"

const HttpStatus * list_person(ControllerActionParam * param);

const HttpStatus * get_person(ControllerActionParam * param);

const HttpStatus * create_person(ControllerActionParam * param);

const HttpStatus * update_person(ControllerActionParam * param);

const HttpStatus * delete_person(ControllerActionParam * param);

#endif
