#ifndef _NOTE_H_
#define _NOTE_H_

#include "types.h"

typedef struct __attribute__((__packed__))
{
  key_t key;
  velocity_t velocity;
} note_t;

#endif
