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

} pattern_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  device_idx_t device;
} track_t;

typedef struct
{
  pattern_idx_t pattern_idx;
} part_pattern_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_song;
  part_pattern_idx_t part_patterns;
  part_pattern_t part_pattern[MAX_PART_PATTERNS];

  part_pattern_idx_t part_pattern_idx;
} part_t;

typedef struct
{
  part_idx_t part_idx;
} song_part_t;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  relative_tempo_t tempo_relative_to_project;
  song_part_idx_t song_parts;
  song_part_t song_part[MAX_SONG_PARTS];

  song_part_idx_t song_part_idx;
} song_t;

typedef enum {
  PROJECT_MODE_STOPPED,
  PROJECT_MODE_PLAY_PROJECT,
  PROJECT_MODE_PLAY_SONG,
  PROJECT_MODE_PLAY_PART,
  PROJECT_MODE_PLAY_PATTERN
} project_mode_t;

typedef enum {
  COLUMN_TYPE_NOTE,
  COLUMN_TYPE_VELOCITY_1,
  COLUMN_TYPE_VELOCITY_2,
  COLUMN_TYPE_COMMAND_1,
  COLUMN_TYPE_COMMAND_2,
  COLUMN_TYPE_PARAMETER_1,
  COLUMN_TYPE_PARAMETER_2
} column_type_t;

typedef struct {
  int column;
  int width;
  track_idx_t track_idx;
  int sub_idx;
  column_type_t type;
} column_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  tempo_t tempo;
  track_idx_t tracks;
  track_t track[MAX_TRACKS + 1];
  pattern_idx_t patterns;
  pattern_t pattern[MAX_PATTERNS + 1];
  part_idx_t parts;
  part_t part[MAX_PARTS + 1];
  song_idx_t songs;
  song_t song[MAX_SONGS + 1];

  bool changed;
  char filename[MAX_FILE_NAME_LENGTH + 1];
  bool edit;
  project_mode_t mode;
  song_idx_t song_idx;
  row_idx_t row_idx;
  column_idx_t column_idx;
  column_idx_t columns;
  column_t column[MAX_COLUMNS];
} project_t;

void project_init();
bool project_load(const char* filename);
bool project_save(const char* filename,
                  char* (*ask_for_project_filename)(char* filename,
                                                    char* name),
                  bool (*ask_for_overwrite)(char* filename, char* name));

void get_pattern_row(char* buf, row_idx_t row_idx);
void project_update();

void project_step();

void play(project_mode_t mode);
void stop();
void edit();

#endif

#include "error.h"

extern project_t project;

extern int studio_get_channel_polyphony(int idx);
extern int studio_get_channel_parameters(int idx);

static inline bool get_edit()
{
  return project.edit;
}

static inline song_idx_t get_song_idx()
{
  return project.song_idx;
}

static inline song_idx_t set_song_idx(song_idx_t song_idx)
{
  return (project.song_idx = song_idx);
}

static inline song_idx_t get_songs() {
  return project.songs;
}

static inline song_part_idx_t get_song_parts()
{
  return project.song[get_song_idx()].song_parts;
}

static inline song_part_idx_t get_song_part_idx()
{
  const song_idx_t song_idx = get_song_idx();
  return project.song[song_idx].song_part_idx;
}

static inline song_part_idx_t set_song_part_idx(song_part_idx_t song_part_idx)
{
  const song_idx_t song_idx = get_song_idx();
  song_part_idx %= project.song[song_idx].song_parts;
  return (project.song[song_idx].song_part_idx = song_part_idx);
}

static inline part_idx_t get_part_idx()
{
  const song_idx_t s = get_song_idx();
  const song_part_idx_t sp = project.song[s].song_part_idx;
  return project.song[s].song_part[sp].part_idx;
}

static inline part_idx_t set_part_idx(part_idx_t part_idx)
{
  const song_idx_t s = get_song_idx();
  const song_part_idx_t sp = project.song[s].song_part_idx;
  part_idx %= project.parts;
  return project.song[s].song_part[sp].part_idx = part_idx;
}

static inline part_pattern_idx_t get_part_pattern_idx()
{
  const part_idx_t part_idx = get_part_idx();
  return project.part[part_idx].part_pattern_idx;
}

static inline part_pattern_idx_t get_part_patterns()
{
  const part_idx_t part_idx = get_part_idx();
  return project.part[part_idx].part_patterns;
}

static inline part_pattern_idx_t set_part_pattern_idx(part_pattern_idx_t part_pattern_idx)
{
  const part_idx_t part_idx = get_part_idx();
  part_pattern_idx %= get_part_patterns();
  return (project.part[part_idx].part_pattern_idx = part_pattern_idx);
}


static inline pattern_idx_t get_pattern_idx()
{
  const part_idx_t p = get_part_idx();
  const part_pattern_idx_t pp = project.part[p].part_pattern_idx;
  return project.part[p].part_pattern[pp].pattern_idx;
}

static inline pattern_idx_t set_pattern_idx(pattern_idx_t pattern_idx)
{
  const part_idx_t p = get_part_idx();
  const part_pattern_idx_t pp = project.part[p].part_pattern_idx;
  pattern_idx %= project.patterns;
  return (project.part[p].part_pattern[pp].pattern_idx = pattern_idx);
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

static inline track_idx_t get_track_idx()
{
  return 0; /* TODO: Fetch this from the cursor column. */
}

static inline note_idx_t get_note_idx()
{
  return 0; /* TODO: Fetch this from the cursor column. */
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
  return project.row_idx;
}

static inline void set_note(key_t key, velocity_t velocity)
{
  const pattern_idx_t pattern_idx = get_pattern_idx();
  const track_idx_t track_idx = get_track_idx();
  const note_idx_t note_idx = get_note_idx();
  const row_idx_t row_idx = get_row_idx();
  debug("Setting note %d on row %d of pattern %d",
        key, row_idx, pattern_idx);
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].key = key;
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].velocity = velocity;
}

static inline row_idx_t set_row_idx(row_idx_t row_idx)
{
  row_idx %= project.pattern[get_pattern_idx()].rows;
  return (project.row_idx = row_idx);
}

static inline column_idx_t get_column_idx()
{
  return project.column_idx;
}

static inline int get_column()
{
  return project.column[get_column_idx()].column;
}

static inline column_idx_t set_column_idx(column_idx_t column_idx)
{
  column_idx %= project.columns;
  return (project.column_idx = column_idx);
}

static inline project_mode_t get_mode()
{
  return project.mode;
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
