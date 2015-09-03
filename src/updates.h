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
} updates_t;

void updates_init();
bool updates_call();
void updates_clear();
void updates_cleanup();

#endif

#include "error.h"
#include "editor.h"
#include "string.h"

extern updates_t updates;

static inline void updates_move_selected_line(row_idx_t old_row_idx,
                                              row_idx_t new_row_idx)
{
  if (updates.move_selected_line == false) {
    updates.move_selected_line_args.old_row_idx = old_row_idx;
  }
  if (updates.move_selected_line_args.old_row_idx != new_row_idx) {
    updates.move_selected_line_args.new_row_idx = new_row_idx;
    updates.move_selected_line = true;
  }
}

static inline void updates_refresh_pattern()
{
  updates.move_selected_line = false; /* Whole pattern redrawn, no need */
  updates.refresh_pattern = true;
}
