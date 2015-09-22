#ifndef _SONG_PART_H_
#define _SONG_PART_H_

#include "config.h"

#include "types.h"

typedef struct __attribute__((__packed__))
{
  part_idx_t part_idx;
} song_part_t;

void song_part_file_format(file_format_args(song_part_t));

#endif
