#ifndef _PATTERN_H_
#define _PATTERN_H_

#include "config.h"
#include "types.h"
#include "constants.h"

#include "pattern_track.h"
#include "row.h"

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_part;
  row_idx_t rows;
  row_t row[MAX_ROWS];
  pattern_track_idx_t pattern_tracks;
  pattern_track_t pattern_track[MAX_TRACKS];
} pattern_t;

void pattern_file_format(file_format_args(pattern_t),
                         const pattern_idx_t pattern_idx,
                         const track_idx_t tracks);

#endif
