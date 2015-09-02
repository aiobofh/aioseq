#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"

typedef struct {
  WINDOW *stats;
  WINDOW *header;
  WINDOW *pos;
  WINDOW *pattern;
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

  wrefresh(editor.stats);
  wrefresh(editor.header);
  wrefresh(editor.pos);
  wrefresh(editor.pattern);

  noecho();
  timeout(1);
  keypad(editor.pattern, TRUE);
}

static void refresh_row(row_idx_t row_idx)
{
  row_idx_t current_row_idx = get_row_idx();

  /* WANNADO: Optimize this for various use cases. */
  char buf[MAX_ROW_LENGTH + 1];
  char scrolled_buf[MAX_ROW_LENGTH + 1];
  get_pattern_row(buf, row_idx);

  /* Truncate line length */
  /* TODO: Scroll the line horisontally using the editor.columns info */
  strncpy(scrolled_buf, buf, editor.cols - 3);

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

/*
 * Render a pattern in the pattern editor.
 */
void refresh_pattern()
{
  pattern_idx_t pattern = get_pattern_idx();
  row_idx_t length = get_pattern_rows();

  mvwprintw(editor.stats, 0, 0, "%02x", pattern);

  for (row_idx_t ridx = 0; ridx < length; ++ridx) {
    refresh_row(ridx);
  }

  wrefresh(editor.stats);
  wrefresh(editor.pos);
  wrefresh(editor.pattern);
}

void read_kbd() {
  int c = wgetch(editor.pattern);
  const row_idx_t row_idx = get_row_idx();
  const column_idx_t column_idx = get_column_idx();
  switch (c) {
  case KEY_F(9):
    play(PROJECT_MODE_PLAY_PATTERN);
    break;
  case KEY_F(10):
    play(PROJECT_MODE_PLAY_PART);
    break;
  case KEY_F(11):
    play(PROJECT_MODE_PLAY_SONG);
    break;
  case KEY_F(12):
    play(PROJECT_MODE_PLAY_PROJECT);
    break;
  case KEY_LEFT:
    set_column_idx(column_idx - 1);
    refresh_row(row_idx);
    wrefresh(editor.pattern);
    break;
  case KEY_RIGHT:
    set_column_idx(column_idx + 1);
    refresh_row(row_idx);
    wrefresh(editor.pattern);
    break;
  case 'i':
  case KEY_UP: {
    set_row_idx(row_idx - 1); /* The project will call the refresh_row() */
    refresh_row(row_idx);
    refresh_row(get_row_idx());
    wrefresh(editor.pos);
    wrefresh(editor.pattern);
    break;
  }
  case 'k':
  case KEY_DOWN:
    set_row_idx(row_idx + 1); /* The project will call the refresh_row() */
    refresh_row(row_idx);
    refresh_row(get_row_idx());
    wrefresh(editor.pos);
    wrefresh(editor.pattern);
    break;
  case 'q':
    m_quit = true;
    break;
  }
}

void editor_cleanup()
{
  delwin(editor.stats);
  delwin(editor.header);
  delwin(editor.pos);
  delwin(editor.pattern);
  endwin();
}
