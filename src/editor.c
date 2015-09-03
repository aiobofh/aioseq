#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"
#include "updates.h"

typedef struct {
  WINDOW *stats;
  WINDOW *header;
  WINDOW *pos;
  WINDOW *pattern;
  struct {
    bool stats;
    bool header;
    bool pos;
    bool pattern;
  } refresh;
  int rows;
  int cols;
} editor_t;

editor_t editor;

extern bool m_quit;


/*
 * Set up the editors different windows
 *
 * Stats|Headers
 * -----+--------------
 * Pos  |Pattern data
 *
 * AiOSeq - New Project
 * Songs [03]:                Song-parts [05]:     Part-patterns [02]:
 * 00 My song                 00 Intro             00 Intro pt1
 * 01 Another song            01 Vers 1            01 Intro pt2
 * 03 Foo song                02 Chorus
 * ---------------------------------------------------------------------
 * 00|00 My Cool MIDI Device             |
 *   |01 Polysynth                       |
 *   |00 Cool filter settings            |
 * --+-----------------------------------+
 * 00|--- 00 --- 00 --- 00 0000 0000 0000|
 */
void editor_init()
{
  memset(&editor, 0, sizeof(editor));
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  getmaxyx(stdscr, editor.rows, editor.cols);

  editor.stats = newwin(3, 3, 0, 0);
  editor.header = newwin(3, editor.cols - 2, 0, 3);
  editor.pos = newwin(editor.rows - 3, 3, 3, 0);
  editor.pattern = newwin(editor.rows - 3, editor.cols - 2, 3, 3);

  wborder(editor.stats, ' ', '|', ' ','-',' ','|','-','+');
  wborder(editor.header, ' ', ' ', ' ','-',' ','|','-','+');
  wborder(editor.pos, ' ', '|', ' ',' ',' ','|',' ','|');

  noecho();
  cbreak();
  wtimeout(editor.pattern, 1);
  keypad(editor.pattern, TRUE);

  editor.refresh.stats = true;
  editor.refresh.header = true;
  editor.refresh.pos = true;
  editor.refresh.pattern = true;
}

void refresh_row(row_idx_t row_idx)
{
  row_idx_t current_row_idx = get_row_idx();

  /* WANNADO: Optimize this for various use cases. */
  char buf[MAX_ROW_LENGTH + 1];
  char scrolled_buf[MAX_ROW_LENGTH + 1];
  scrolled_buf[0] = 0;
  get_pattern_row(buf, row_idx);

  /* Truncate line length */
  /* TODO: Scroll the line horisontally using the editor.columns info */
  strncpy(scrolled_buf, buf, editor.cols - 3);
  scrolled_buf[editor.cols - 2] = 0;

  const bool current_row = ((row_idx == current_row_idx));
  if (true == current_row) {
    wattron(editor.pos, A_REVERSE);
  }
  mvwprintw(editor.pos, row_idx, 0, "%02x", row_idx);
  if (true == current_row) {
    wattroff(editor.pos, A_REVERSE);
    wattron(editor.pattern, A_REVERSE);
  }
  mvwprintw(editor.pattern, row_idx, 0, "%s", scrolled_buf);
  if (true == current_row) {
    wattroff(editor.pattern, A_REVERSE);
  }

  wmove(editor.pattern, current_row_idx, get_column());
}

static inline void refresh_pattern_window()
{
  editor.refresh.pos = true;
  editor.refresh.pattern = true;
}

void editor_move_selected_line(row_idx_t old_row_idx, row_idx_t new_row_idx)
{
  refresh_row(old_row_idx);
  refresh_row(new_row_idx);
  refresh_pattern_window();
}

/*
 * Render a pattern in the pattern editor.
 */
void editor_refresh_pattern()
{
  pattern_idx_t pattern = get_pattern_idx();
  row_idx_t length = get_pattern_rows();

  mvwprintw(editor.stats, 0, 0, "%02x", pattern);

  for (row_idx_t ridx = 0; ridx < length; ++ridx) {
    refresh_row(ridx);
  }

  editor.refresh.stats = true;
  refresh_pattern_window();
}

void editor_read_kbd() {
  int c = wgetch(editor.pattern);
  const row_idx_t row_idx = get_row_idx();
  const column_idx_t column_idx = get_column_idx();
  switch (c) {
  case KEY_F(9):
    play(PROJECT_MODE_PLAY_PATTERN);
    mvwprintw(editor.header, 0, 0, "Play pattern");
    editor.refresh.header = true;
    break;
  case KEY_F(10):
    play(PROJECT_MODE_PLAY_PART);
    mvwprintw(editor.header, 0, 0, "Play part");
    editor.refresh.header = true;
    break;
  case KEY_F(11):
    play(PROJECT_MODE_PLAY_SONG);
    mvwprintw(editor.header, 0, 0, "Play song");
    editor.refresh.header = true;
    break;
  case KEY_F(12):
    play(PROJECT_MODE_PLAY_PROJECT);
    mvwprintw(editor.header, 0, 0, "Play project");
    editor.refresh.header = true;
    break;
  case KEY_LEFT:
    set_column_idx(column_idx - 1);
    wmove(editor.pattern, row_idx, get_column());
    break;
  case KEY_RIGHT:
    set_column_idx(column_idx + 1);
    wmove(editor.pattern, row_idx, get_column());
    break;
  case KEY_UP: {
    set_row_idx(row_idx - 1); /* The project will call the refresh_row() */
    updates_move_selected_line(row_idx, get_row_idx());
    break;
  }
  case KEY_DOWN:
    set_row_idx(row_idx + 1); /* The project will call the refresh_row() */
    updates_move_selected_line(row_idx, get_row_idx());
    break;
  case 'q':
    m_quit = true;
    break;
  case ' ':
    stop();
    mvwprintw(editor.header, 0, 0, "Stop");
    editor.refresh.header = true;
    break;
  }
}

#define REFRESH(NAME)                           \
  if (true == editor.refresh.NAME) {            \
    wrefresh(editor.NAME);                      \
    editor.refresh.NAME = false;                \
  }

void editor_refresh_windows() {
  REFRESH(stats);
  REFRESH(header);
  REFRESH(pos);
  REFRESH(pattern);
}

#undef REFRESH

void editor_cleanup()
{
  delwin(editor.stats);
  delwin(editor.header);
  delwin(editor.pos);
  delwin(editor.pattern);
  endwin();
}
