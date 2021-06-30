#ifndef DATABASE_MANAGEMENT
#define DATABASE_MANAGEMENT

#include <stdio.h>
#include "states.h"

state create_file_ifn_exists(const char * file_name);

state register_row(const char * file_name, void * data, size_t size);

#endif
