#include <unistd.h>
#include <string.h>
#include <ncurses.h>

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

  /*
  wrefresh(editor.pos);
  wrefresh(editor.pattern);
  */
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
