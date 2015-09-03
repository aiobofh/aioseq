#include <assert.h>
#include <string.h>

#include "constants.h"

#include "updates.h"

updates_t updates;

static bool updates_initialized = false;

void updates_init()
{
  memset(&updates, 0, sizeof(updates));
  updates_initialized = true;
}

bool updates_call()
{
  bool done_something = false;
  if (true == updates.move_selected_line) {
    editor_move_selected_line(updates.move_selected_line_args.old_row_idx,
                              updates.move_selected_line_args.new_row_idx);
    done_something = true;
  }
  if (true == updates.refresh_pattern) {
    editor_refresh_pattern();
    done_something = true;
  }
  memset(&updates, 0, sizeof(updates));
  return done_something;
}

void updates_clear()
{
  assert(true == updates_initialized);
}

void updates_cleanup()
{
  assert(true == updates_initialized);
}
