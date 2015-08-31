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

void get_pattern_row(char* buf, row_idx_t row_idx);

#endif

extern project_t project;

extern int studio_get_channel_polyphony(int idx);
extern int studio_get_channel_parameters(int idx);

static inline song_idx_t get_song_idx()
{
  return project.song_idx;
}

static inline part_idx_t get_part_idx()
{
  const song_idx_t s = get_song_idx();
  const song_part_idx_t sp = project.song[s].song_part_idx;
  return project.song[s].part_idx[sp];
}

static inline pattern_idx_t get_pattern_idx()
{
  const part_idx_t p = get_part_idx();
  const part_pattern_idx_t pp = project.part[p].part_pattern_idx;
  return project.part[p].pattern_idx[pp];
}

static inline pattern_idx_t get_pattern_rows()
{
  const pattern_idx_t p = get_pattern_idx();
  return project.pattern[p].rows;
}

static inline track_idx_t get_tracks()
{
  return project.tracks;
}

static inline note_idx_t get_notes(track_idx_t track_idx)
{
  return studio_get_channel_polyphony(track_idx);
}

static inline note_idx_t get_effects(track_idx_t track_idx)
{
  return studio_get_channel_parameters(track_idx);
}

static inline row_idx_t get_row_idx()
{
  const pattern_idx_t p = get_pattern_idx();
  return project.pattern[p].row_idx + 5;
}

static inline tempo_t get_tempo()
{
  const song_idx_t song = get_song_idx();
  const part_idx_t part = get_part_idx();
  const pattern_idx_t pattern = get_pattern_idx();
  const row_idx_t row = get_row_idx();

  const tempo_t tempo = project.tempo;
  const relative_tempo_t tempo_relative_to_project =
    project.song[song].tempo_relative_to_project;
  const relative_tempo_t tempo_relative_to_song =
    project.part[part].tempo_relative_to_song;
  const relative_tempo_t tempo_relative_to_part =
    project.pattern[pattern].tempo_relative_to_part;
  const relative_tempo_t tempo_relative_to_pattern =
    project.pattern[pattern].row[row].tempo_relative_to_pattern;

  return (tempo + tempo_relative_to_project + tempo_relative_to_song +
          tempo_relative_to_part + tempo_relative_to_pattern);
}
