#ifndef _COLUMNS_H_
#define _COLUMNS_H_

#include "constants.h"
#include "types.h"

typedef enum __attribute__((__packed__)) {
  COLUMN_TYPE_NOTE,
  COLUMN_TYPE_VELOCITY_1,
  COLUMN_TYPE_VELOCITY_2,
  COLUMN_TYPE_COMMAND_1,
  COLUMN_TYPE_COMMAND_2,
  COLUMN_TYPE_PARAMETER_1,
  COLUMN_TYPE_PARAMETER_2
} column_type_t;

typedef struct __attribute__((__packed__)) {
  int column;
  int width;
  track_idx_t track_idx;
  int sub_idx;
  column_type_t type;
} column_t;

typedef struct __attribute__((__packed__)) {
  column_idx_t column_idx;
  column_idx_t columns;
  column_t column[MAX_COLUMNS];
  int width;
} columns_t;

void columns_update();
int columns_get_column(column_idx_t column_idx);
column_idx_t columns_get_columns();
column_type_t columns_get_column_type(column_idx_t column_idx);
column_idx_t columns_set_column_idx(column_idx_t column_idx);
column_idx_t columns_get_column_idx();
column_type_t columns_get_column_type(column_idx_t column_idx);
track_idx_t columns_get_track_idx(column_idx_t column_idx);
note_idx_t columns_get_note_idx(column_idx_t column_idx);
effect_idx_t columns_get_effect_idx(column_idx_t column_idx);

#endif
