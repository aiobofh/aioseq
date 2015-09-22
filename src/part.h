#ifndef _PART_H_
#define _PART_H_

#include "config.h"
#include "types.h"

#include "part_pattern.h"

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_song;
  part_pattern_idx_t part_patterns;
  part_pattern_t part_pattern[MAX_PART_PATTERNS];

  part_pattern_idx_t part_pattern_idx;
} part_t;

void part_file_format(file_format_args(part_t));

#endif
