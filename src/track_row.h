#ifndef _TRACK_ROW_H_
#define _TRACK_ROW_H_

#include "constants.h"

#include "note.h"
#include "effect.h"

typedef struct __attribute__((__packed__))
{
  note_t note[MAX_NOTES];
  effect_t effect[MAX_EFFECTS];
} track_row_t;

#endif
