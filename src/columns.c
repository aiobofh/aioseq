#include <string.h>

#include "columns.h"

#include "constants.h"
#include "project.h"
#include "studio.h"
#include "types.h"

static columns_t columns;

/*
 * Helper macro only used in the update_columns() function.
 */
#define ADD_COLUMN(COLS, TYPE, SUB)                     \
  columns.column[column_idx].column = column;           \
  columns.column[column_idx].width = COLS;              \
  columns.column[column_idx].type = TYPE;               \
  columns.column[column_idx].track_idx = track_idx;     \
  columns.column[column_idx].sub_idx = SUB;             \
  column_idx++;                                         \
  column += COLS

void columns_update()
{
  memset(&columns, 0, sizeof(columns));
  int column_idx = 0;
  int column = 0;
  track_idx_t tracks = project_get_tracks();
  song_idx_t song_idx = project_get_song_idx();
  song_part_idx_t song_part_idx = project_get_song_part_idx(song_idx);
  part_idx_t part_idx = project_get_part_idx(song_idx, song_part_idx);
  part_pattern_idx_t part_pattern_idx =
    project_get_part_pattern_idx(part_idx);
  pattern_idx_t pattern_idx = project_get_pattern_idx(part_idx, part_pattern_idx);
  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {
    note_idx_t notes = project_get_notes(pattern_idx, track_idx);
    effect_idx_t effects = project_get_effects(pattern_idx, track_idx);
    for (note_idx_t note_idx = 0; note_idx < notes; note_idx++) {
      ADD_COLUMN(3, COLUMN_TYPE_NOTE, note_idx);
      column++;
      ADD_COLUMN(1, COLUMN_TYPE_VELOCITY_1, note_idx);
      ADD_COLUMN(1, COLUMN_TYPE_VELOCITY_2, note_idx);
      column++;
    }

    for (effect_idx_t effect_idx = 0; effect_idx < effects; effect_idx++) {
      ADD_COLUMN(1, COLUMN_TYPE_COMMAND_1, effect_idx);
      ADD_COLUMN(1, COLUMN_TYPE_COMMAND_2, effect_idx);
      ADD_COLUMN(1, COLUMN_TYPE_PARAMETER_1, effect_idx);
      ADD_COLUMN(1, COLUMN_TYPE_PARAMETER_2, effect_idx);
      column++;
    }
  }
  columns.columns = column_idx;
  columns.width = column;
}

#undef ADD_COLUMN

int columns_get_column(column_idx_t column_idx)
{
  return columns.column[column_idx].column;
}

column_idx_t columns_get_columns()
{
  return columns.columns;
}

column_idx_t columns_set_column_idx(column_idx_t column_idx)
{
  return (columns.column_idx = column_idx);
}

column_idx_t columns_get_column_idx()
{
  return columns.column_idx;
}

column_type_t columns_get_column_type(column_idx_t column_idx)
{
  return columns.column[column_idx].type;
}

track_idx_t columns_get_track_idx(column_idx_t column_idx)
{
  return columns.column[column_idx].track_idx;
}

note_idx_t columns_get_note_idx(column_idx_t column_idx)
{
  assert((COLUMN_TYPE_NOTE == columns.column[column_idx].type) ||
         (COLUMN_TYPE_VELOCITY_1 == columns.column[column_idx].type) ||
         (COLUMN_TYPE_VELOCITY_2 == columns.column[column_idx].type));
  return columns.column[column_idx].sub_idx;
}

effect_idx_t columns_get_effect_idx(column_idx_t column_idx)
{
  assert((COLUMN_TYPE_COMMAND_1 == columns.column[column_idx].type) ||
         (COLUMN_TYPE_COMMAND_2 == columns.column[column_idx].type) ||
         (COLUMN_TYPE_PARAMETER_1 == columns.column[column_idx].type) ||
         (COLUMN_TYPE_PARAMETER_2 == columns.column[column_idx].type));
  return columns.column[column_idx].sub_idx;
}
