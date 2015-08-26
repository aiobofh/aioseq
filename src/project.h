#ifndef _PROJECT_H_
#define _PROJECT_H_

#include "types.h"
#include "constants.h"

typedef struct
{
  key_t key;
  velocity_t velocity;
} note_t;

typedef struct
{
  command_t command;
  parameter_t parameter;
} effect_t;

typedef struct
{
  note_t note[MAX_NOTES];
  effect_t effect[MAX_EFFECTS];
} track_row_t;

typedef struct
{
  track_row_t track_row[MAX_TRACK_ROWS];
  relative_tempo_t tempo_relative_to_pattern;
} row_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_part;
  row_idx_t rows;
  row_t row[MAX_ROWS];

  row_idx_t row_idx;
} pattern_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  device_idx_t device;
} track_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_song;
  part_pattern_idx_t part_patterns;
  pattern_idx_t pattern_idx[MAX_PART_PATTERNS];

  part_pattern_idx_t part_pattern_idx;
} part_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_project;
  song_part_idx_t song_parts;
  part_idx_t part_idx[MAX_SONG_PARTS];

  song_part_idx_t song_part_idx;
} song_t;

typedef struct
{
  bool changed;
  char filename[MAX_FILE_NAME_LENGTH + 1];

  char name[MAX_NAME_LENGTH];
  tempo_t tempo;
  track_idx_t tracks;
  track_t track[MAX_TRACKS];
  pattern_idx_t patterns;
  pattern_t pattern[MAX_PATTERNS];
  part_idx_t parts;
  part_t part[MAX_PARTS];
  song_idx_t songs;
  song_t song[MAX_SONGS];

  song_idx_t song_idx;
} project_t;

void project_init();
bool project_load(const char* filename);
bool project_save(const char* filename,
                  char* (*ask_for_project_filename)(char* filename,
                                                    char* name),
                  bool (*ask_for_overwrite)(char* filename, char* name));

song_idx_t song_idx();
part_idx_t part_idx();
pattern_idx_t pattern_idx();
row_idx_t row_idx();
tempo_t tempo();

#endif

extern project_t project;
