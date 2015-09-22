#ifndef _SONG_H_
#define _SONG_H_

#include "types.h"
#include "constants.h"

#include "config.h"

#include "song_part.h"

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_project;
  song_part_idx_t song_parts;
  song_part_t song_part[MAX_SONG_PARTS];

  song_part_idx_t song_part_idx;
} song_t;

void song_file_format(file_format_args(song_t));

#endif
