#include <stdio.h>
#include "states.h"

boolean is_error(state value) {
  return value < 0;
}

boolean is_success(state value) {
  return value > 0;
}
