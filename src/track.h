#ifndef _TRACK_H_
#define _TRACK_H_

#include "config.h"
#include "types.h"
#include "constants.h"

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH];
  device_idx_t device_idx;

  key_t note_on[MAX_NOTES];
} track_t;

void track_file_format(file_format_args(track_t));

#endif
