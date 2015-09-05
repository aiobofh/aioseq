#include "constants.h"
#include "types.h"

#ifndef _UPDATES_H_
#define _UPDATES_H_


typedef enum {
  UPDATES_MOVE_SELECTED_LINE,
  UPDATES_REFRESH_PATTERN
} update_type_t;

typedef struct {
  bool move_selected_line;
  struct {
    row_idx_t old_row_idx;
    row_idx_t new_row_idx;
  } move_selected_line_args;
  bool refresh_pattern;
  bool song;
  int song_idx;
  bool part;
  int part_idx;
  bool pattern;
  int pattern_idx;
  bool edit;
  bool mode;
  bool tempo;
  tempo_t tempo_set;
} updates_t;

void updates_init();
void updates_move_selected_line(row_idx_t old_row_idx,
                                row_idx_t new_row_idx);
void updates_set_song(song_idx_t song_idx);
void updates_set_part(part_idx_t part_idx);
void updates_set_pattern(pattern_idx_t pattern_idx);
void updates_set_tempo();
void updates_set_edit();
void updates_set_mode();
void updates_refresh_pattern();
bool updates_call();
void updates_clear();
void updates_cleanup();

#endif

#include "error.h"
#include "editor.h"
#include "string.h"

extern updates_t updates;
