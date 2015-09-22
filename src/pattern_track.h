#ifndef _PATTERN_TRACK_H_
#define _PATTERN_TRACK_H_

#include "config.h"

#include "types.h"

typedef struct __attribute__((__packed__))
{
  instrument_idx_t instrument_idx;
  setting_idx_t setting_idx;
} pattern_track_t;

void pattern_track_file_format(file_format_args(pattern_track_t));

#endif
