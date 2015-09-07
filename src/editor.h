#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <ncurses.h>

#include "types.h"

typedef struct {
  WINDOW *stats;
  WINDOW *header;
  WINDOW *pos;
  WINDOW *pattern;
  WINDOW *console;
  struct {
    bool stats;
    bool header;
    bool pos;
    bool pattern;
    bool console;
  } refresh;
  int rows;
  int cols;
  int row_offset;
  int col_offset;
} editor_t;

void editor_init();
void editor_cleanup();
void editor_move_selected_line(row_idx_t old_row_idx,
                               row_idx_t new_row_idx);
void editor_refresh_pattern();
void editor_read_kbd();
void editor_refresh_windows();

void editor_refresh_song_idx();
void editor_refresh_part_idx();
void editor_refresh_pattern_idx();
void editor_refresh_tempo();
void editor_refresh_status();
void editor_refresh_devices();

int editor_debug(const char *format, ...);

#endif
