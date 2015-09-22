#ifndef _PART_PATTERN_H_
#define _PART_PATTERN_H_

#include "config.h"

#include "types.h"

typedef struct __attribute__((__packed__))
{
  pattern_idx_t pattern_idx;
} part_pattern_t;

void part_pattern_file_format(file_format_args(part_pattern_t));

#endif
