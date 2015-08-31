#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"

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
} position_t;

typedef struct {
  WINDOW *stats;
  WINDOW *header;
  WINDOW *pos;
  WINDOW *pattern;
  int rows;
  int cols;
  int column;
  int positions;
  position_t position[100000]; // TODO: Use macros to calculate this.
} editor_t;

editor_t editor;

#define ADD_COLUMN(COLS, TYPE, SUB)                     \
  editor.position[column_idx].column = column;          \
  editor.position[column_idx].width = COLS;             \
  editor.position[column_idx].type = TYPE;              \
  editor.position[column_idx].track_idx = track;        \
  editor.position[column_idx].sub_idx = SUB;            \
  column_idx++;                                         \
  column += COLS

static void update_columns()
{
  int column_idx = 0;
  int column = 0;
  track_idx_t tracks = get_tracks();
  for (track_idx_t track = 0; track < tracks; track++) {
    note_idx_t notes = get_notes(track);
    effect_idx_t effects = get_effects(track);
    for (note_idx_t note = 0; note < notes; note++) {
      ADD_COLUMN(3, COLUMN_TYPE_NOTE, note);
      column++;
      ADD_COLUMN(1, COLUMN_TYPE_VELOCITY_1, note);
      ADD_COLUMN(1, COLUMN_TYPE_VELOCITY_2, note);
      column++;
    }

    for (effect_idx_t effect = 0; effect < effects; effect++) {
      ADD_COLUMN(1, COLUMN_TYPE_COMMAND_1, effect);
      ADD_COLUMN(1, COLUMN_TYPE_COMMAND_2, effect);
      ADD_COLUMN(1, COLUMN_TYPE_PARAMETER_1, effect);
      ADD_COLUMN(1, COLUMN_TYPE_PARAMETER_2, effect);
      column++;
    }
  }
  editor.positions = column_idx;
}

#undef ADD_COLUMN

void editor_init()
{
  memset(&editor, 0, sizeof(editor));
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  getmaxyx(stdscr, editor.rows, editor.cols);

  editor.column = 10;

  editor.stats = newwin(3, 3, 0, 0);
  editor.header = newwin(3, editor.cols - 2, 0, 3);
  editor.pos = newwin(editor.rows - 3, 3, 3, 0);
  editor.pattern = newwin(editor.rows - 3, editor.cols - 2, 3, 3);

  wborder(editor.stats, ' ', '|', ' ','-',' ','|','-','+');
  wborder(editor.header, ' ', ' ', ' ','-',' ','|','-','+');
  wborder(editor.pos, ' ', '|', ' ',' ',' ','|',' ','|');

  wrefresh(editor.stats);
  wrefresh(editor.header);
  wrefresh(editor.pos);
  wrefresh(editor.pattern);

}

void refresh_pattern()
{
  pattern_idx_t pattern = get_pattern_idx();
  row_idx_t row = get_row_idx();
  row_idx_t length = get_pattern_rows();

  update_columns();

  mvwprintw(editor.stats, 0, 0, "%02x", pattern);

  for (row_idx_t ridx = 0; ridx < length; ++ridx) {
    char buf[MAX_ROW_LENGTH + 1];
    char scrolled_buf[MAX_ROW_LENGTH + 1];
    get_pattern_row(buf, ridx);

    /* Truncate line length */
    /* TODO: Scroll the line horisontally using the editor.columns info */
    strncpy(scrolled_buf, buf, editor.cols - 3);

    const int current_row = (ridx == row);
    if (current_row) {
      wattron(editor.pos, A_REVERSE);
    }
    mvwprintw(editor.pos, ridx, 0, "%02x", ridx);
    if (current_row) {
      wattroff(editor.pos, A_REVERSE);
      wattron(editor.pattern, A_REVERSE);
    }
    mvwprintw(editor.pattern, ridx, 0, "%s", scrolled_buf);
    if (current_row) {
      wattroff(editor.pattern, A_REVERSE);
    }
  }

  wmove(editor.pattern, row, editor.position[editor.column].column);

  wrefresh(editor.stats);
  wrefresh(editor.pos);
  wrefresh(editor.pattern);
}

void editor_cleanup()
{
  sleep(5);
  delwin(editor.stats);
  delwin(editor.header);
  delwin(editor.pos);
  delwin(editor.pattern);
  endwin();
}
