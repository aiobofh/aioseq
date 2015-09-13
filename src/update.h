#include <assert.h>

#include "helpers.h"
#include "error.h"
#include "studio.h"

#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "constants.h"
#include "types.h"

#define MAX_INSTRUMENT_EVENTS 256

typedef enum
{
  INSTRUMENT_EVENT_TYPE_NOTE_ON,
  INSTRUMENT_EVENT_TYPE_NOTE_OFF,
  INSTRUMENT_EVENT_TYPE_CONTROL
} instrument_event_type_t;

typedef struct
{
  unsigned char key;
  unsigned char velocity;
} instrument_event_note_t;

typedef struct
{
  unsigned char parameter;
  unsigned char value;
} instrument_event_control_t;

typedef struct
{
  int instrument_event_cnt;
  struct {
    instrument_event_type_t type;
    device_idx_t device_idx;
    instrument_idx_t instrument_idx;
    int channel;
    union {
      instrument_event_note_t note;
      instrument_event_control_t control;
    } data;
  } instrument_event_u[MAX_INSTRUMENT_EVENTS];

  int edit_cnt;
  struct {
    bool edit;
  } edit_u;

  int quantization_cnt;
  struct {
    quantization_t quantization;
  } quantization_u;

  int pattern_rows_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t rows;
  } pattern_rows_u;

  int project_mode_cnt;
  struct {
    project_mode_t mode;
  } project_mode_u;

  int song_idx_cnt;
  struct {
    song_idx_t song_idx;
  } song_idx_u;

  int song_part_idx_cnt;
  struct {
    song_idx_t song_idx;
    song_part_idx_t song_part_idx;
  } song_part_idx_u;

  int song_parts_cnt;
  struct {
    song_idx_t song_idx;
    song_part_idx_t song_parts;
  } song_parts_u;

  int part_idx_cnt;
  struct {
    song_idx_t song_idx;
    song_part_idx_t song_part_idx;
    part_idx_t part_idx;
  } part_idx_u;

  int part_pattern_cnt;
  struct {
    part_pattern_idx_t part_pattern_idx;
    pattern_idx_t pattern_idx;
  } part_pattern_u;

  int part_pattern_idx_cnt;
  struct {
    part_idx_t part_idx;
    part_pattern_idx_t part_pattern_idx;
  } part_pattern_idx_u;

  int part_patterns_cnt;
  struct {
    part_idx_t part_idx;
    part_pattern_idx_t part_patterns;
  } part_patterns_u;

  int pattern_idx_cnt;
  struct {
    part_idx_t part_idx;
    part_pattern_idx_t part_pattern_idx;
    pattern_idx_t pattern_idx;
  } pattern_idx_u;

  int patterns_cnt;
  struct {
    pattern_idx_t patterns;
  } patterns_u;

  int row_idx_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t row_idx;
  } row_idx_u;

  int column_idx_cnt;
  struct {
    column_idx_t column_idx;
  } column_idx_u;

  int key_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t row_idx;
    track_idx_t track_idx;
    note_idx_t note_idx;
    key_t key;
  } key_u[MAX_NOTES * MAX_TRACKS];

  int velocity_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t row_idx;
    track_idx_t track_idx;
    note_idx_t note_idx;
    velocity_t velocity;
  } velocity_u[MAX_NOTES * MAX_TRACKS];

  int command_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t row_idx;
    track_idx_t track_idx;
    note_idx_t effect_idx;
    command_t command;
  } command_u[MAX_EFFECTS * MAX_TRACKS];

  int parameter_cnt;
  struct {
    pattern_idx_t pattern_idx;
    row_idx_t row_idx;
    track_idx_t track_idx;
    note_idx_t effect_idx;
    parameter_t parameter;
  } parameter_u[MAX_EFFECTS * MAX_TRACKS];

} update_t;

void update_init();
void update_commit();

#endif

extern update_t update;

static inline void update_instrument_note_on(device_idx_t device_idx,
                                             instrument_idx_t instrument_idx,
                                             key_t key,
                                             velocity_t velocity)
{
  assert(MAX_INSTRUMENT_EVENTS > update.instrument_event_cnt);
  int idx = update.instrument_event_cnt;
  update.instrument_event_u[idx].type = INSTRUMENT_EVENT_TYPE_NOTE_ON;
  update.instrument_event_u[idx].device_idx = device_idx;
  update.instrument_event_u[idx].instrument_idx = instrument_idx;
  update.instrument_event_u[idx].channel = studio_get_channel(device_idx);
  update.instrument_event_u[idx].data.note.key = key - 1;
  update.instrument_event_u[idx].data.note.velocity = velocity;
  update.instrument_event_cnt++;
}

static inline void update_instrument_note_off(device_idx_t device_idx,
                                              instrument_idx_t instrument_idx,
                                              key_t key,
                                              velocity_t velocity)
{
  assert(MAX_INSTRUMENT_EVENTS > update.instrument_event_cnt);
  int idx = update.instrument_event_cnt;
  update.instrument_event_u[idx].type = INSTRUMENT_EVENT_TYPE_NOTE_OFF;
  update.instrument_event_u[idx].device_idx = device_idx;
  update.instrument_event_u[idx].instrument_idx = instrument_idx;
  update.instrument_event_u[idx].channel = studio_get_channel(device_idx);
  update.instrument_event_u[idx].data.note.key = key - 1;
  update.instrument_event_u[idx].data.note.velocity = velocity;
  update.instrument_event_cnt++;
}

static inline void update_instrument_control(device_idx_t device_idx,
                                             instrument_idx_t instrument_idx,
                                             command_t command,
                                             parameter_t parameter)
{
  assert(MAX_INSTRUMENT_EVENTS > update.instrument_event_cnt);
  int idx = update.instrument_event_cnt;
  update.instrument_event_u[idx].type = INSTRUMENT_EVENT_TYPE_CONTROL;
  update.instrument_event_u[idx].device_idx = device_idx;
  update.instrument_event_u[idx].instrument_idx = instrument_idx;
  update.instrument_event_u[idx].channel = studio_get_channel(device_idx);
  update.instrument_event_u[idx].data.control.parameter = command;
  update.instrument_event_u[idx].data.control.value = parameter;
  update.instrument_event_cnt++;
}

static inline void update_quantization(quantization_t quantization,
                                       quantization_t max)
{
  assert(0 == update.quantization_cnt);
  update.quantization_u.quantization = wrap(quantization, max);
  update.quantization_cnt++;
  assert(1 == update.quantization_cnt);
}

static inline void update_pattern_rows(pattern_idx_t pattern_idx,
                                       row_idx_t rows)
{
  assert(0 == update.pattern_rows_cnt);
  update.pattern_rows_u.pattern_idx = pattern_idx;
  if (rows < 1) {
    rows = 1;
  }
  if (rows > MAX_ROWS) {
    rows = MAX_ROWS;
  }
  update.pattern_rows_u.rows = rows;
  update.pattern_rows_cnt++;
  assert(1 == update.pattern_rows_cnt);
}

static inline void update_edit(const bool edit)
{
  assert(0 == update.edit_cnt);
  update.edit_u.edit = edit;
  update.edit_cnt++;
  assert(1 == update.edit_cnt);
}

static inline void update_project_mode(const project_mode_t mode)
{
  assert(0 == update.project_mode_cnt);
  update.project_mode_u.mode = mode;
  update.project_mode_cnt++;
  assert(1 == update.project_mode_cnt);
}

static inline
void update_part_pattern(const part_pattern_idx_t part_pattern_idx,
                         const pattern_idx_t pattern_idx)
{
  assert(0 == update.part_pattern_cnt);
  update.part_pattern_u.part_pattern_idx = part_pattern_idx;
  update.part_pattern_u.pattern_idx = pattern_idx;
  update.part_pattern_cnt++;
  assert(1 == update.part_pattern_cnt);
}

static inline
void update_pattern_idx(const part_idx_t part_idx,
                        const part_pattern_idx_t part_pattern_idx,
                        const pattern_idx_t pattern_idx)
{
  assert(0 == update.pattern_idx_cnt);
  update.pattern_idx_u.part_idx = part_idx;
  update.pattern_idx_u.part_pattern_idx = part_pattern_idx;
  update.pattern_idx_u.pattern_idx = pattern_idx;
  update.pattern_idx_cnt++;
  assert(1 == update.pattern_idx_cnt);
}

static inline
void update_patterns(const pattern_idx_t patterns)
{
  assert(0 == update.patterns_cnt);
  update.patterns_u.patterns = patterns;
  update.patterns_cnt++;
  assert(1 == update.patterns_cnt);
}

static inline void update_song_idx(const song_idx_t song_idx)
{
  assert(0 == update.song_idx_cnt);
  update.song_idx_u.song_idx = song_idx;
  update.song_idx_cnt++;
  assert(1 == update.song_idx_cnt);
}

static inline void update_song_part_idx(const song_idx_t song_idx,
                                        const song_part_idx_t song_part_idx)
{
  assert(0 == update.song_part_idx_cnt);
  update.song_part_idx_u.song_idx = song_idx;
  update.song_part_idx_u.song_part_idx = song_part_idx;
  update.song_part_idx_cnt++;
  assert(1 == update.song_part_idx_cnt);
}

static inline void update_song_parts(const song_idx_t song_idx,
                                     const song_part_idx_t song_parts)
{
  assert(0 == update.song_parts_cnt);
  update.song_parts_u.song_idx = song_idx;
  update.song_parts_u.song_parts = song_parts;
  update.song_parts_cnt++;
  assert(1 == update.song_parts_cnt);
}

static inline void update_part_idx(const song_idx_t song_idx,
                                   const song_part_idx_t song_part_idx,
                                   const part_idx_t part_idx)
{
  assert(0 == update.part_idx_cnt);
  update.part_idx_u.song_idx = song_idx;
  update.part_idx_u.song_part_idx = song_part_idx;
  update.part_idx_u.part_idx = part_idx;
  update.part_idx_cnt++;
  assert(1 == update.part_idx_cnt);
}

static inline
void update_part_pattern_idx(const part_idx_t part_idx,
                             const part_pattern_idx_t part_pattern_idx)
{
  assert(0 == update.part_pattern_idx_cnt);
  update.part_pattern_idx_u.part_idx = part_idx;
  update.part_pattern_idx_u.part_pattern_idx = part_pattern_idx;
  update.part_pattern_idx_cnt++;
  assert(1 == update.part_pattern_idx_cnt);
}

static inline void update_part_patterns(const part_idx_t part_idx,
                                        const part_pattern_idx_t part_patterns)
{
  assert(0 == update.part_patterns_cnt);
  update.part_patterns_u.part_idx = part_idx;
  update.part_patterns_u.part_patterns = part_patterns;
  update.part_patterns_cnt++;
  assert(1 == update.part_patterns_cnt);
}

static inline void update_row_idx(const pattern_idx_t pattern_idx,
                                  const row_idx_t row_idx,
                                  const row_idx_t rows)
{
  assert(0 == update.row_idx_cnt);
  update.row_idx_u.pattern_idx = pattern_idx;
  update.row_idx_u.row_idx = wrap(row_idx, rows - 1);
  update.row_idx_cnt++;
  assert(1 == update.row_idx_cnt);
}

static inline void update_column_idx(const column_idx_t column_idx,
                                     const column_idx_t columns)
{
  assert(0 == update.column_idx_cnt);
  update.column_idx_u.column_idx = wrap(column_idx, columns - 1);
  update.column_idx_cnt++;
  assert(1 == update.column_idx_cnt);
}

static inline void update_key(const pattern_idx_t pattern_idx,
                              const row_idx_t row_idx,
                              const track_idx_t track_idx,
                              const note_idx_t note_idx,
                              const key_t key)
{
  assert(MAX_NOTES * MAX_TRACKS > update.key_cnt);
  update.key_u[update.key_cnt].pattern_idx = pattern_idx;
  update.key_u[update.key_cnt].row_idx = row_idx;
  update.key_u[update.key_cnt].track_idx = track_idx;
  update.key_u[update.key_cnt].note_idx = note_idx;
  update.key_u[update.key_cnt].key = key;
  update.key_cnt++;
}

static inline void update_velocity(const pattern_idx_t pattern_idx,
                                   const row_idx_t row_idx,
                                   const track_idx_t track_idx,
                                   const note_idx_t note_idx,
                                   const velocity_t velocity)
{
  assert(MAX_NOTES * MAX_TRACKS > update.velocity_cnt);
  update.velocity_u[update.velocity_cnt].pattern_idx = pattern_idx;
  update.velocity_u[update.velocity_cnt].row_idx = row_idx;
  update.velocity_u[update.velocity_cnt].track_idx = track_idx;
  update.velocity_u[update.velocity_cnt].note_idx = note_idx;
  update.velocity_u[update.velocity_cnt].velocity = velocity;
  update.velocity_cnt++;
}

static inline void update_command(const pattern_idx_t pattern_idx,
                                  const row_idx_t row_idx,
                                  const track_idx_t track_idx,
                                  const effect_idx_t effect_idx,
                                  const command_t command)
{
  assert(MAX_NOTES * MAX_TRACKS > update.command_cnt);
  update.command_u[update.command_cnt].pattern_idx = pattern_idx;
  update.command_u[update.command_cnt].row_idx = row_idx;
  update.command_u[update.command_cnt].track_idx = track_idx;
  update.command_u[update.command_cnt].effect_idx = effect_idx;
  update.command_u[update.command_cnt].command = command;
  update.command_cnt++;
}

static inline void update_parameter(const pattern_idx_t pattern_idx,
                                    const row_idx_t row_idx,
                                    const track_idx_t track_idx,
                                    const effect_idx_t effect_idx,
                                    const parameter_t parameter)
{
  assert(MAX_NOTES * MAX_TRACKS > update.parameter_cnt);
  update.parameter_u[update.parameter_cnt].pattern_idx = pattern_idx;
  update.parameter_u[update.parameter_cnt].row_idx = row_idx;
  update.parameter_u[update.parameter_cnt].track_idx = track_idx;
  update.parameter_u[update.parameter_cnt].effect_idx = effect_idx;
  update.parameter_u[update.parameter_cnt].parameter = parameter;
  update.parameter_cnt++;
}
