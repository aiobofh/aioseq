#include <stdio.h>

#include "types.h"

#ifndef _ERROR_H_
#define _ERROR_H_

bool debug_enabled;

#define error(ID, ...)                                          \
  fprintf(stderr, "ERROR: " ID "\n", __VA_ARGS__)

#define warning(ID, ...)                                        \
  fprintf(stdout, "WARNING: " ID "\n", __VA_ARGS__)

#define debug(ID, ...)                                  \
  if (true == debug_enabled) {                          \
    fprintf(stdout, "DEBUG: " ID "\n", __VA_ARGS__);    \
  }

#endif
