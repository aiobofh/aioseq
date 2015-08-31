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
}

void refresh_pattern()
{
  pattern_idx_t pattern = get_pattern_idx();
  row_idx_t row = get_row_idx();
  row_idx_t length = get_pattern_rows();

  mvwprintw(editor.stats, 0, 0, "%02x", pattern);

  for (row_idx_t ridx = 0; ridx < length; ++ridx) {
    char buf[MAX_ROW_LENGTH + 1];
    get_pattern_row(buf, ridx);
    const int current_row = (ridx == row);
    if (current_row) {
      wattron(editor.pos, A_REVERSE);
    }
    mvwprintw(editor.pos, ridx, 0, "%02x", ridx);
    if (current_row) {
      wattroff(editor.pos, A_REVERSE);
      wattron(editor.pattern, A_REVERSE);
    }
    mvwprintw(editor.pattern, ridx, 0, "%s", buf);
    if (current_row) {
      wattroff(editor.pattern, A_REVERSE);
    }
  }

  wmove(editor.pattern, row, 0);

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
