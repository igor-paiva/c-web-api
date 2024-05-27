#ifndef PERSON_VIEW
#define PERSON_VIEW

#include <stdio.h>
#include "view.h"
#include "../states.h"
#include "../models/person_model.h"

state get_person_view(void ** args, char * response);

state list_person_view(void ** args, char * response);

#endif