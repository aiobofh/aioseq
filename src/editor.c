#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"
#include "updates.h"
#include "event.h"

editor_t editor;

static bool editor_initialized = false;

extern bool m_quit;

static int key_to_note[256];

#define K(CHAR) key_to_note[CHAR] = cnt++

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
 * 01|--- 00 --- 00 --- 00 0000 0000 0000|
 * ---------------------------------------------------------------------
 * Debug
 *
 */
void editor_init()
{
  /* Needed by th K-macro */
  int cnt = 1;

  memset(&key_to_note, 0, sizeof(key_to_note));
  memset(&editor, 0, sizeof(editor));
  initscr();
  raw();

  keypad(stdscr, TRUE);
  noecho();

  K('z'); K('s'); K('x'); K('d'); K('c'); K('v'); K('g'); K('b');
  K('h'); K('n'); K('j'); K('m');

  K('q'); K('2'); K('w'); K('3'); K('e'); K('r'); K('5'); K('t');
  K('6'); K('y'); K('7'); K('u');

  K('i'); K('9'); K('o'); K('0'); K('p');

  getmaxyx(stdscr, editor.rows, editor.cols);

  int debug_rows;
  if (true == debug_enabled)
    debug_rows = 10;
  else
    debug_rows = 0;

  const int heading_h = 6;
  const int pos_w = 3;
  const int pattern_h = editor.rows - heading_h - debug_rows;
  const int pattern_w = editor.cols - pos_w;

  editor.stats   = newwin(heading_h,     pos_w,         0, 0);
  editor.header  = newwin(heading_h, pattern_w,         0, pos_w);
  editor.pos     = newwin(pattern_h,     pos_w, heading_h, 0);
  editor.pattern = newwin(pattern_h, pattern_w, heading_h, pos_w);

  if (true == debug_enabled) {
    editor.console = newwin(debug_rows, editor.cols, editor.rows - 3 - debug_rows + 3, 0);
    scrollok(editor.console, TRUE);
    wsetscrreg(editor.console, 0, debug_rows);
  }

  wborder(editor.stats, ' ', '|', ' ','-',' ','|','-','+');
  wborder(editor.header, ' ', ' ', ' ','-',' ','|','-','+');
  wborder(editor.pos, ' ', '|', ' ',' ',' ','|',' ','|');
  wborder(editor.console, ' ', ' ', '-',' ',' ',' ',' ',' ');

  noecho();
  cbreak();
  wtimeout(editor.pattern, 1);
  keypad(editor.pattern, TRUE);
  if (true == debug_enabled) {
    wtimeout(editor.console, 1);
    keypad(editor.console, TRUE);
  }

  editor.refresh.stats = true;
  editor.refresh.header = true;
  editor.refresh.pos = true;
  editor.refresh.pattern = true;
  editor.refresh.console = true;

  editor_initialized = true;
}

void refresh_row(row_idx_t row_idx)
{
  assert(true == editor_initialized);

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

  wmove(editor.pattern, current_row_idx, get_column_from_column());
}

static inline void refresh_pattern_window()
{
  editor.refresh.pos = true;
  editor.refresh.pattern = true;
}

void editor_move_selected_line(row_idx_t old_row_idx, row_idx_t new_row_idx)
{
  assert(true == editor_initialized);

  refresh_row(old_row_idx);
  refresh_row(new_row_idx);
  refresh_pattern_window();
}

/*
 * Render a pattern in the pattern editor.
 */
void editor_refresh_pattern()
{
  assert(true == editor_initialized);

  pattern_idx_t pattern_idx = get_pattern_idx();
  row_idx_t length = get_pattern_rows();

  mvwprintw(editor.stats, 0, 3, "%02x", pattern_idx);

  for (row_idx_t ridx = 0; ridx < length; ++ridx) {
    refresh_row(ridx);
  }

  editor.refresh.stats = true;
  refresh_pattern_window();
}

void editor_refresh_song_idx()
{
  const song_idx_t song_idx = get_song_idx();
  mvwprintw(editor.stats, 0, 0, "%02x", song_idx);
  mvwprintw(editor.header, 0, 0, "%s", get_song_name());
  editor.refresh.stats = true;
  editor.refresh.header = true;
}

void editor_refresh_part_idx()
{
  const part_idx_t part_idx = get_part_idx();
  mvwprintw(editor.stats, 1, 0, "%02x", part_idx);
  mvwprintw(editor.header, 1, 0, "%s", get_part_name());
  editor.refresh.stats = true;
  editor.refresh.header = true;
}

void editor_refresh_pattern_idx()
{
  const pattern_idx_t pattern_idx = get_pattern_idx();
  mvwprintw(editor.stats, 2, 0, "%02x", pattern_idx);
  mvwprintw(editor.header, 2, 0, "%s", get_pattern_name());
  editor.refresh.stats = true;
  editor.refresh.header = true;
}

void editor_refresh_tempo()
{
  const tempo_t tempo = get_tempo();
  mvwprintw(editor.stats, 3, 0, "%02x", tempo);
  editor.refresh.stats = true;
}

void editor_refresh_status()
{
  char status[3];
  status[0] = ' ';
  switch (get_mode()) {
  case PROJECT_MODE_STOPPED:
    status[0] = ' ';
    break;
  case PROJECT_MODE_PLAY_PROJECT:
    status[0] = 'P';
    break;
  case PROJECT_MODE_PLAY_SONG:
    status[0] = 's';
    break;
  case PROJECT_MODE_PLAY_PART:
    status[0] = 'p';
    break;
  case PROJECT_MODE_PLAY_PATTERN:
    status[0] = '>';
    break;
  }
  if (true == get_edit()) {
    status[1] = 'E';
  }
  else {
    status[1] = ' ';
  }
  status[2] = 0;
  mvwprintw(editor.stats, 4, 0, "%s", status);
  editor.refresh.stats = true;
}

void editor_read_kbd() {
  assert(true == editor_initialized);
  static int last_note = -1;
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
    wmove(editor.pattern, row_idx, get_column_from_column());
    break;
  case KEY_RIGHT:
    set_column_idx(column_idx + 1);
    wmove(editor.pattern, row_idx, get_column_from_column());
    break;
  case KEY_UP: {
    set_row_idx(row_idx - 1);
    updates_move_selected_line(row_idx, get_row_idx());
    break;
  }
  case KEY_DOWN:
    set_row_idx(row_idx + 1);
    updates_move_selected_line(row_idx, get_row_idx());
    break;
  case ' ':
    if (-1 != last_note) {
      event_type_args_t args;
      event_type_note_off_t* note_off = &args.event_type_note_off;
      note_off->note = last_note;
      note_off->velocity = 127;
      note_off->channel = 0; /* Get the channel from the columns list */
      event_add(EVENT_TYPE_NOTE_OFF, args);
    }
    set_edit(!get_edit());
    stop();
    editor.refresh.header = true;
    break;
  default:
    /*
     * Emulate the master keyboard
     */
    if (0 != key_to_note[c]) {
      if (-1 != last_note) {
        event_type_args_t args;
        event_type_note_off_t* note_off = &args.event_type_note_off;
        note_off->note = last_note;
        note_off->velocity = 127;
        note_off->channel = 0; /* Get the channel from the columns list */
        event_add(EVENT_TYPE_NOTE_OFF, args);
      }
      event_type_args_t args;
      event_type_note_on_t* note_on = &args.event_type_note_on;
      note_on->note = key_to_note[c];
      note_on->velocity = 127;
      note_on->channel = 0; /* Get the channel from the columns list */
      event_add(EVENT_TYPE_NOTE_ON, args);
      last_note = key_to_note[c];
      break;
    }
    if (17 == c) { /* CTRL+Q = Quit */
      m_quit = true;
      break;
    }
    if (-1 != c) {
      debug("Unhandled key %d", c);
    }
  }
}

#define REFRESH(NAME)                           \
  if (true == editor.refresh.NAME) {            \
    wrefresh(editor.NAME);                      \
    editor.refresh.NAME = false;                \
  }

void editor_refresh_windows() {
  assert(true == editor_initialized);

  REFRESH(console);
  REFRESH(stats);
  REFRESH(header);
  REFRESH(pos);
  REFRESH(pattern);
}

#undef REFRESH

void editor_cleanup()
{
  assert(true == editor_initialized);

  delwin(editor.stats);
  delwin(editor.header);
  delwin(editor.pos);
  delwin(editor.pattern);
  if (true == debug_enabled)
    delwin(editor.console);
  endwin();
}

int editor_debug(const char *format, ...) {
  va_list arg;
  int done;

  char buf[10000];
  buf[0] = 0;

  va_start(arg, format);
  done = vsprintf(buf, format, arg);
  va_end(arg);

  wprintw(editor.console, "DEBUG: %s\n", buf);

  wrefresh(editor.console);

  return done;
}
