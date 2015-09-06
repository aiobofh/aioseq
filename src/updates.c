#include <assert.h>
#include <string.h>

#include "constants.h"
#include "editor.h"
#include "updates.h"

updates_t updates;

static bool updates_initialized = false;

void updates_init()
{
  memset(&updates, 0, sizeof(updates));
  updates_initialized = true;
}

void updates_move_selected_line(row_idx_t old_row_idx,
                                              row_idx_t new_row_idx)
{
  assert(true == updates_initialized);
  if (updates.move_selected_line == false) {
    updates.move_selected_line_args.old_row_idx = old_row_idx;
  }
  if (updates.move_selected_line_args.old_row_idx != new_row_idx) {
    updates.move_selected_line_args.new_row_idx = new_row_idx;
    updates.move_selected_line = true;
  }
}

void updates_set_song(song_idx_t song_idx)
{
  assert(true == updates_initialized);
  updates.song_idx = song_idx;
  updates.song = true;
}

void updates_set_part(part_idx_t part_idx)
{
  assert(true == updates_initialized);
  updates.part_idx = part_idx;
  updates.part = true;
}

void updates_set_pattern(pattern_idx_t pattern_idx)
{
  assert(true == updates_initialized);
  updates.pattern_idx = pattern_idx;
  updates.pattern = true;
}

void updates_refresh_pattern()
{
  assert(true == updates_initialized);
  updates.move_selected_line = false; /* Whole pattern redrawn, no need */
  updates.refresh_pattern = true;
}

void updates_set_tempo() {
  assert(true == updates_initialized);
  updates.tempo = true;
}

void updates_set_edit()
{
  assert(true == updates_initialized);
  updates.edit = true;
}

void updates_set_mode()
{
  assert(true == updates_initialized);
  updates.mode = true;
}

bool updates_call()
{
  assert(true == updates_initialized);
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
  if (true == updates.song) {
    editor_refresh_song_idx();
    done_something = true;
  }
  if (true == updates.part) {
    editor_refresh_part_idx();
    done_something = true;
  }
  if (true == updates.pattern) {
    editor_refresh_pattern_idx();
    done_something = true;
  }
  if ((true == updates.mode) || (true == updates.edit)) {
    editor_refresh_status();
    done_something = true;
  }
  if (true == updates.tempo) {
    editor_refresh_tempo();
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
