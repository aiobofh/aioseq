#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "types.h"

typedef struct __attribute__((__packed__))
{
  command_t command;
  parameter_t parameter;
} effect_t;

#endif
