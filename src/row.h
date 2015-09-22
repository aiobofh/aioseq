#ifndef _ROW_H_
#define _ROW_H_

#include "config.h"

#include "types.h"

#include "track_row.h"

typedef struct __attribute__((__packed__))
{
  track_row_t track_row[MAX_TRACK_ROWS];
  relative_tempo_t tempo_relative_to_pattern;
} row_t;

void project_get_pattern_row(char* buf, pattern_idx_t pattern_idx,
                             row_idx_t row_idx);
void row_append(char* buf, pattern_idx_t pattern_idx, row_t* row,
                track_idx_t tracks);
void row_file_format(file_format_args(row_t),
                     const pattern_idx_t pattern_idx,
                     const track_idx_t tracks);

#endif
