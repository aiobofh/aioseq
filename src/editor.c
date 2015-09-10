#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"
#include "update.h"
#include "event.h"
#include "columns.h"

editor_t editor;

static bool editor_initialized = false;

extern bool m_quit;

static int key_to_note[256];

#define K(CHAR) key_to_note[CHAR] = cnt++

/*
 * Set up the editors different windows
 *
 * AiOSeq - New Project
 * Song: 00 My song                          00 01 02 03
 * Part: 01 My part                          01 02 03 04 05
 * Patt: 03 My pattern                       03 04 05
 * --+-----------------------------------+------------------------------
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

  const int heading_h = 8;
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
  //  wborder(editor.pos, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wborder(editor.console, ' ', ' ', '-',' ',' ',' ',' ',' ');

  scrollok(editor.pos, true);
  scrollok(editor.pattern, true);

  noecho();
  cbreak();
  wtimeout(editor.pattern, 1);
  keypad(editor.pattern, TRUE);
  if (true == debug_enabled) {
    wtimeout(editor.console, 1);
    keypad(editor.console, TRUE);
  }

  editor_set_octave(1);

  editor.refresh.stats = true;
  editor.refresh.header = true;
  editor.refresh.pos = true;
  editor.refresh.pattern = true;
  editor.refresh.console = true;

  editor_initialized = true;
}

void refresh_row(row_idx_t row_idx, int width)
{
  assert(true == editor_initialized);

  if ((row_idx < editor.row_offset) ||
      (row_idx > editor.rows)) {
    return;
  }

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
  mvwprintw(editor.pos, row_idx - editor.row_offset, 0, "%02x", row_idx);
  if (true == current_row) {
    wattroff(editor.pos, A_REVERSE);
    wattron(editor.pattern, A_REVERSE);
  }

  int offset = editor.col_offset;

  scrolled_buf[width - 1 + offset] = 0;

  mvwprintw(editor.pattern, row_idx - editor.row_offset, 0, "%s", &scrolled_buf[offset]);
  if (true == current_row) {
    wattroff(editor.pattern, A_REVERSE);
  }

  editor.refresh.pos = true;
  editor.refresh.pattern = true;
}

static void strcatpad(char* buf, const char* str, int pad)
{
  strcat(buf, str);
  int end_of_buf = strlen(buf);
  for (int i = strlen(str); i <= pad; i++) {
    buf[end_of_buf++] = ' ';
  }
  buf[end_of_buf] = 0;
}

static int get_track_width(pattern_idx_t pattern_idx, track_idx_t track_idx)
{
  const note_idx_t notes = get_notes(pattern_idx, track_idx);
  const note_idx_t effects = get_effects(pattern_idx, track_idx);

  const int track_width = ((MAX_NOTE_LENGTH + 1 +
                            MAX_VELOCITY_LENGTH + 1) * notes - 1 +
                           (MAX_COMMAND_LENGTH +
                            MAX_PARAMETER_LENGTH + 1) * effects - 1);
  return track_width;
}

#define CONCAT(NAME)                                    \
  buf[0] = 0;                                           \
  sprintf(buf, fmt, NAME ## _idx, NAME ## _name);       \
  buf[track_width + 1] = 0;                             \
  strcatpad(NAME ## _buf, buf, track_width);            \
  if (track_idx < tracks - 1) {                         \
    strcat(NAME ## _buf, "|");                          \
  }

void refresh_devices()
{
  char device_buf[MAX_ROW_LENGTH];
  char instrument_buf[MAX_ROW_LENGTH];
  char setting_buf[MAX_ROW_LENGTH];

  device_buf[0] = instrument_buf[0] = setting_buf[0] = 0;

  int width = 0;
  const track_idx_t tracks = get_tracks();
  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {
    const pattern_idx_t pattern_idx = get_pattern_idx();
    const device_idx_t device_idx = get_device_idx(track_idx);
    const instrument_idx_t instrument_idx = get_instrument_idx(pattern_idx,
                                                               track_idx);
    const setting_idx_t setting_idx = get_setting_idx(pattern_idx,
                                                      track_idx);

    const char* device_name = studio_get_device_name(device_idx);
    const char* instrument_name = studio_get_instrument_name(device_idx,
                                                             instrument_idx);
    const char* setting_name = studio_get_setting_name(device_idx,
                                                       instrument_idx,
                                                       setting_idx);

    const int track_width = get_track_width(pattern_idx, track_idx);

    char buf[MAX_TRACK_LENGTH];
    char fmt[10];
    buf[0] = 0;
    fmt[0] = 0;

    sprintf(fmt, "%%02x:%%s");

    CONCAT(device);
    CONCAT(instrument);
    CONCAT(setting);

    if (track_idx != tracks -1) {
      width += track_width + 2;
      mvwprintw(editor.header, 6, width - 1, "|");
    }
  }

  int w, h;

  getmaxyx(editor.header, h, w);

  h = h;

  if (strlen(device_buf) > (unsigned int)w) {
    device_buf[w + editor.col_offset] = 0;
    instrument_buf[w + editor.col_offset] = 0;
    setting_buf[w + editor.col_offset] = 0;
  }

  mvwprintw(editor.header, 3, 0, &(device_buf[editor.col_offset]));
  mvwprintw(editor.header, 4, 0, &(instrument_buf[editor.col_offset]));
  mvwprintw(editor.header, 5, 0, &(setting_buf[editor.col_offset]));

  editor.refresh.header = true;
}

#undef CONCAT

/*
 * Render a pattern in the pattern editor.
 */
static void refresh_pattern(pattern_idx_t pattern_idx)
{
  int w, h;

  assert(true == editor_initialized);

  getmaxyx(editor.pattern, h, w);

  row_idx_t length = get_pattern_rows();

  debug("Rendering pattern of %d rows", length);

  mvwprintw(editor.stats, 0, 3, "%02x", pattern_idx);

  if (h < length)
    length = h;

  werase(editor.pos);
  werase(editor.pattern);

  for (row_idx_t ridx = editor.row_offset; ridx < length + editor.row_offset; ++ridx) {
    refresh_row(ridx, w);
  }

  editor.refresh.stats = true;

  refresh_devices();
}

void editor_refresh_tempo()
{
  const tempo_t tempo = get_tempo();
  mvwprintw(editor.stats, 3, 0, "%02x", tempo);
  editor.refresh.stats = true;
}

static unsigned char key_to_hex(const char key)
{
  if (key >= '0' && key <= '9') {
    return key - '0';
  }
  if (key >= 'a' && key <= 'f') {
    return key - 'a' + 10;
  }
  return 0;
}

#define MSN(NAME, IDXTYPE, ROW_INC, COL_INC)                            \
  {                                                                     \
  const unsigned char hex = key_to_hex(c);                              \
  if ((0 != hex) && (true == edit)) {                                   \
    const IDXTYPE ## _idx_t IDXTYPE ## _idx                             \
      = columns_get_ ## IDXTYPE ## _idx(column_idx);                    \
    NAME ## _t NAME = project_get_ ## NAME(pattern_idx,                 \
                                           row_idx,                     \
                                           track_idx,                   \
                                           IDXTYPE ## _idx);            \
    NAME = ((((hex & 0xf) << 4) & 0xf0) | (NAME & 0xf));                \
    update_ ## NAME(pattern_idx, row_idx, track_idx, IDXTYPE ## _idx,   \
                    NAME);                                              \
    update_row_idx(pattern_idx, row_idx + ROW_INC, rows);               \
    update_column_idx(column_idx + COL_INC, columns_get_columns());     \
  }                                                                     \
  }

#define LSN(NAME, IDXTYPE, ROW_INC, COL_INC)                            \
  {                                                                     \
  const unsigned char hex = key_to_hex(c);                              \
  if ((0 != hex) && (true == edit)) {                                   \
    const IDXTYPE ## _idx_t IDXTYPE ## _idx                             \
      = columns_get_ ## IDXTYPE ## _idx(column_idx);                    \
    NAME ## _t NAME = project_get_ ## NAME(pattern_idx,                 \
                                           row_idx,                     \
                                           track_idx,                   \
                                           IDXTYPE ## _idx);            \
    NAME = ((hex & 0x0f) | (NAME & 0xf0));                              \
    update_ ## NAME(pattern_idx, row_idx, track_idx, IDXTYPE ## _idx,   \
                    NAME);                                              \
    update_row_idx(pattern_idx, row_idx + ROW_INC, rows);               \
    update_column_idx(column_idx + COL_INC, columns_get_columns());     \
  }                                                                     \
  }

void editor_read_kbd() {
  assert(true == editor_initialized);

  const pattern_idx_t pattern_idx = get_pattern_idx();
  const row_idx_t rows = project_get_pattern_rows(pattern_idx);
  const row_idx_t row_idx = get_row_idx();
  const column_idx_t column_idx = columns_get_column_idx();
  const column_type_t column_type = columns_get_column_type(column_idx);
  const project_mode_t mode = get_mode();

  const bool edit = project_get_edit();
  const track_idx_t track_idx = columns_get_track_idx(column_idx);
  const device_idx_t device_idx = get_device_idx(track_idx);
  const instrument_idx_t instrument_idx = get_instrument_idx(pattern_idx,
                                                             track_idx);

  const quantization_t quantization = project_get_quantization();

  const song_idx_t song_idx = project_get_song_idx();
  const song_part_idx_t song_parts = project_get_song_parts(song_idx);
  const song_part_idx_t song_part_idx =
    project_get_song_part_idx(song_idx);
  const part_idx_t part_idx =
    project_get_part_idx(song_idx, song_part_idx);
  const part_pattern_idx_t part_patterns =
    project_get_part_patterns(part_idx);
  const part_pattern_idx_t part_pattern_idx =
    project_get_part_pattern_idx(part_idx);

  int c = wgetch(editor.pattern);

  if (27 == c) {
    c = wgetch(editor.pattern);
    debug("1: %d", c);
    if (79 == c) {
      const part_pattern_idx_t part_pattern_idx =
        project_get_part_pattern_idx(part_idx);

      c = wgetch(editor.pattern);
      debug("4: %d", c);
      if (100 == c) { // Ctrl + LEFT
        update_pattern_idx(part_idx, part_pattern_idx, pattern_idx - 1);
        c = -1;
      } else if (99 == c) { // Ctrl + RIGHT
        update_pattern_idx(part_idx, part_pattern_idx, pattern_idx + 1);
        c = -1;
      }
    } else if (27 == c) {
      c = wgetch(editor.pattern);
      debug("5: %d", c);
      if (79 == c) {
        const song_part_idx_t song_part_idx =
        project_get_song_part_idx(song_idx);
        c = wgetch(editor.pattern);
        debug("6: %d", c);
        if (100 == c) { // Ctrl + Alt + LEFT
          update_song_part_idx(song_idx, song_part_idx - 1);
          c = -1;
        } else if (99 == c) { // Ctrl + Alt + RIGHT
          update_song_part_idx(song_idx, song_part_idx + 1);
          c = -1;
        }
      }
    } else if (91 == c) {
      c = wgetch(editor.pattern);
      debug("2: %d", c);
      if (49 == c) {
        c = wgetch(editor.pattern);
        debug("3: %d", c);
        switch (c) {
        case 49: // Ctrl + F1
          update_quantization(quantization - 1, 0xf);
          c = -1;
          break;
        case 50: // Ctrl + F2
          update_quantization(quantization + 1, 0xf);
          c = -1;
          break;
        }
      }
    }
    else if (260 == c) {
      update_part_pattern_idx(part_idx, part_pattern_idx - 1);
      c = -1;
    }
    else if (261 == c) {
      update_part_pattern_idx(part_idx, part_pattern_idx + 1);
      c = -1;
    }
  }

  // This static struct keeps track of what note to turn off from last call.
  static struct {
    device_idx_t device_idx;
    instrument_idx_t instrument_idx;
    key_t key;
  } note_off;

  if (note_off.key != 0) {
    update_instrument_note_off(note_off.device_idx,
                               note_off.instrument_idx,
                               note_off.key,
                               127);
    memset(&note_off, 0, sizeof(note_off));
  }

  switch (c) {
  case KEY_F(1):
    if (editor.octave > 1) {
      editor_set_octave(editor.octave - 1);
    }
    break;
  case KEY_F(2):
    if (editor.octave < 9) {
      editor_set_octave(editor.octave + 1);
    }
    break;
  case KEY_F(9):
    if (PROJECT_MODE_STOPPED == mode)
      update_mode(PROJECT_MODE_PLAY_PATTERN);
    break;
  case KEY_F(10):
    if (PROJECT_MODE_STOPPED == mode)
      update_mode(PROJECT_MODE_PLAY_PART);
    break;
  case KEY_F(11):
    if (PROJECT_MODE_STOPPED == mode)
      update_mode(PROJECT_MODE_PLAY_SONG);
    break;
  case KEY_F(12):
    if (PROJECT_MODE_STOPPED == mode)
      update_mode(PROJECT_MODE_PLAY_PROJECT);
    break;
  case KEY_LEFT:
    update_column_idx(column_idx - 1, columns_get_columns());
    break;
  case KEY_RIGHT:
    update_column_idx(column_idx + 1, columns_get_columns());
    break;
  case KEY_UP:
    if (PROJECT_MODE_STOPPED == mode)
      update_row_idx(pattern_idx, row_idx - 1, rows);
    break;
  case KEY_DOWN:
    if (PROJECT_MODE_STOPPED == mode)
      update_row_idx(pattern_idx, row_idx + 1, rows);
    break;
  case ' ':
    if (PROJECT_MODE_STOPPED != mode)
      update_mode(PROJECT_MODE_STOPPED);
    update_edit(!get_edit());
    break;
  default:
    switch (column_type) {
    case COLUMN_TYPE_NOTE:{
      key_t key = key_to_note[c];
      if (0 != key) {
        key += 12 * (editor.octave - 1);
        if (true == edit) {
          const note_idx_t note_idx = columns_get_note_idx(column_idx);
          update_key(pattern_idx, row_idx, track_idx, note_idx, key);
          update_velocity(pattern_idx, row_idx, track_idx, note_idx,
                          127);
          update_row_idx(pattern_idx, row_idx + (quantization + 1), rows);
        }
        // Make the instrument play the note
        update_instrument_note_on(device_idx, instrument_idx, key, 127);
        // Remember which note to turn off
        note_off.device_idx = device_idx;
        note_off.instrument_idx = instrument_idx;
        note_off.key = key;
        c = -1;
      }
      break;
    }
    case COLUMN_TYPE_VELOCITY_1:
      MSN(velocity, note, 0, 1);
      break;
    case COLUMN_TYPE_VELOCITY_2:
      LSN(velocity, note, (quantization + 1), 0);
      break;
    case COLUMN_TYPE_COMMAND_1:
      MSN(command, effect, 0, 1);
      break;
    case COLUMN_TYPE_COMMAND_2:
      LSN(command, effect, 0, 1);
      break;
    case COLUMN_TYPE_PARAMETER_1:
      MSN(parameter, effect, 0, 1);
      break;
    case COLUMN_TYPE_PARAMETER_2:
      LSN(parameter, effect, (quantization + 1), 0);
      break;
    }
    if (17 == c) { /* CTRL+Q = Quit */
      m_quit = true;
      break;
    } else if ('+' == c) {
      update_pattern_rows(pattern_idx, rows + 1);
    } else if ('\'' == c) {
      update_pattern_rows(pattern_idx, rows - 1);
    } else if ('?' == c) {
      update_song_parts(song_idx, song_parts + 1);
    } else if ('*' == c) {
      update_song_parts(song_idx, song_parts - 1);
    } else if ('\\' == c) {
      update_part_patterns(part_idx, part_patterns + 1);
    } else if ('´' == c) {
      update_part_patterns(part_idx, part_patterns - 1);
    } else if (-1 != c) {
      debug("Unhandled key %d", c);
    }
    break;
  }
}

#undef MSN
#undef LSN

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

  const column_idx_t column_idx = columns_get_column_idx();
  const row_idx_t row_idx = project_get_row_idx();

  wmove(editor.pattern, row_idx - editor.row_offset, columns_get_column(column_idx) - editor.col_offset);
}

#undef REFRESH

void editor_set_octave(int octave)
{
  assert((octave >= 1) && (octave <= 9));
  editor.octave = octave;
  mvwprintw(editor.stats, 5, 1, "%x", editor.octave);
  editor.refresh.stats = true;
}

void editor_cleanup()
{
  assert(true == editor_initialized);

  echo();
  noraw();
  nocbreak();

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


void editor_set_edit(const bool edit)
{
  assert(0 == (int)false);
  assert(1 == (int)true);

  const static char c[2] = {' ', 'E'};

  mvwprintw(editor.stats, 4, 1, "%c", c[edit]);

  editor.refresh.stats = true;
}

void editor_set_quantization(const quantization_t quantization)
{
  assert(quantization <= 0xf);

  mvwprintw(editor.stats, 5, 0, "%x", quantization);

  editor.refresh.stats = true;
}

void editor_set_mode(const project_mode_t mode)
{
  assert(0 == PROJECT_MODE_STOPPED);
  assert(1 == PROJECT_MODE_PLAY_PROJECT);
  assert(2 == PROJECT_MODE_PLAY_SONG);
  assert(3 == PROJECT_MODE_PLAY_PART);
  assert(4 == PROJECT_MODE_PLAY_PATTERN);

  const static char c[6] = {' ', 'a', 's', 'p', '>'};

  mvwprintw(editor.stats, 4, 1, "%c", c[mode]);

  editor.refresh.stats = true;
}

static void refresh_song_list(song_idx_t song_idx)
{
  song_idx_t songs = get_songs();
  for (song_idx_t idx = 0; idx < songs; idx++) {
    int column = MAX_NAME_LENGTH + idx * 3;
    if (song_idx == idx) {
      wattron(editor.header, A_REVERSE);
    }
    mvwprintw(editor.header, 0, column, "%02x", idx);
    if (song_idx == idx) {
      wattroff(editor.header, A_REVERSE);
    }
  }
  editor.refresh.header = true;
}

static void refresh_song_part_list(song_idx_t song_idx,
                                  song_part_idx_t song_part_idx)
{
  const song_part_idx_t song_parts = project_get_song_parts(song_idx);

  for (song_part_idx_t idx = 0; idx < song_parts; idx++) {
    const part_idx_t part_idx = project_get_part_idx(song_idx, idx);
    const int column = MAX_NAME_LENGTH + idx * 3;
    if (song_part_idx == idx) {
      wattron(editor.header, A_REVERSE);
    }
    mvwprintw(editor.header, 1, column, "%02x", part_idx);
    if (song_part_idx == idx) {
      wattroff(editor.header, A_REVERSE);
    }
  }
  editor.refresh.header = true;
}

static void refresh_part_pattern_list(part_idx_t part_idx,
                                      part_pattern_idx_t part_pattern_idx)
{
  const part_pattern_idx_t part_patterns =
    project_get_part_patterns(part_idx);

  for (part_pattern_idx_t idx = 0; idx < part_patterns; idx++) {
    const pattern_idx_t pattern_idx = project_get_pattern_idx(part_idx, idx);
    const int column = MAX_NAME_LENGTH + idx * 3;
    if (part_pattern_idx == idx) {
      wattron(editor.header, A_REVERSE);
    }
    mvwprintw(editor.header, 2, column, "%02x", pattern_idx);
    if (part_pattern_idx == idx) {
      wattroff(editor.header, A_REVERSE);
    }
  }
  editor.refresh.header = true;
}

void editor_set_song_idx(const song_idx_t song_idx)
{
  mvwprintw(editor.stats, 0, 0, "%02x", song_idx);
  mvwprintw(editor.header, 0, 0, "%s", "GET SONG NAME");

  refresh_song_list(song_idx);
  refresh_song_part_list(song_idx, project_get_song_part_idx(song_idx));

  editor.refresh.stats = true;
  editor.refresh.header = true;
}

void editor_set_song_part_idx(const song_idx_t song_idx,
                              const song_part_idx_t song_part_idx)
{
  const part_idx_t part_idx = project_get_part_idx(song_idx, song_part_idx);

  refresh_song_part_list(song_idx, song_part_idx);
  refresh_part_pattern_list(part_idx, project_get_part_pattern_idx(part_idx));
}

void editor_set_song_parts(const song_idx_t song_idx,
                           const song_part_idx_t song_parts)
{
  const song_part_idx_t song_part_idx = project_get_song_part_idx(song_idx);
  const part_idx_t part_idx = project_get_part_idx(song_idx, song_part_idx);

  refresh_song_part_list(song_idx, song_part_idx);
  refresh_part_pattern_list(part_idx, project_get_part_pattern_idx(part_idx));
}

void editor_set_part_idx(const song_idx_t song_idx,
                         const song_part_idx_t song_part_idx,
                         const part_idx_t part_idx)
{
  mvwprintw(editor.stats, 1, 0, "%02x", part_idx);
  mvwprintw(editor.header, 1, 0, "%s", "GET PART NAME");

  editor.refresh.stats = true;
  editor.refresh.header = true;
}

void editor_set_part_pattern_idx(const part_idx_t part_idx,
                                 const part_pattern_idx_t part_pattern_idx)
{
  refresh_part_pattern_list(part_idx, part_pattern_idx);
  refresh_pattern(project_get_pattern_idx(part_idx, part_pattern_idx));
}

void editor_set_part_patterns(const part_idx_t part_idx,
                              const part_pattern_idx_t part_patterns)
{
  part_pattern_idx_t part_pattern_idx = project_get_part_pattern_idx(part_idx);

  refresh_part_pattern_list(part_idx, part_pattern_idx);
  refresh_pattern(project_get_pattern_idx(part_idx, part_pattern_idx));
}

void editor_set_pattern_idx(const part_idx_t part_idx,
                            const part_pattern_idx_t part_pattern_idx,
                            const pattern_idx_t pattern_idx)
{
  mvwprintw(editor.stats, 2, 0, "%02x", pattern_idx);
  mvwprintw(editor.header, 2, 0, "%s", "GET PATTERN NAME");

  editor.refresh.stats = true;
  editor.refresh.header = true;

  refresh_part_pattern_list(part_idx, part_pattern_idx);
  refresh_pattern(pattern_idx);
}

void editor_set_pattern_rows(const pattern_idx_t pattern_idx,
                             const row_idx_t rows)
{
  mvwprintw(editor.stats, 6, 0, "%02x", rows);

  editor.refresh.stats = true;

  refresh_pattern(pattern_idx);
}

static void calculate_vertical_scroll(row_idx_t row_idx,
                                      row_idx_t rows,
                                      int height)
{
  if ((rows > height) && (row_idx >= (height / 2))) {
    editor.row_offset = (row_idx - (height / 2));
    if (editor.row_offset > (rows - height)) {
      editor.row_offset = (rows - height);
    }
  }
  else {
    editor.row_offset = 0;
  }
}

void editor_set_row_idx(const pattern_idx_t pattern_idx,
                        const row_idx_t row_idx)
{
  int h, w;
  int old_row_offset = editor.row_offset;
  getmaxyx(editor.pattern, h, w);
  h = h;
  calculate_vertical_scroll(row_idx, project_get_pattern_rows(pattern_idx), h);
  row_idx_t old_row_idx = editor.row_idx;

  int scroll_delta = editor.row_offset - old_row_offset;

  wscrl(editor.pattern, scroll_delta);
  wscrl(editor.pos, scroll_delta);

  refresh_row(old_row_idx, w);
  editor.row_idx = row_idx;
  refresh_row(row_idx, w);

  int start = 0;
  if (scroll_delta > 0) {
    start = h + old_row_offset;
  }
  else if (scroll_delta < 0) {
    scroll_delta *= -1; // abs;
    start = editor.row_offset;
  }
  for (int i = start ; i < start + scroll_delta; i++) {
    refresh_row(i, w);
  }
}

void editor_set_column_idx(const column_idx_t column_idx)
{
  editor.column = columns_get_column(column_idx);
  /* TODO: Implement horizontal scolling */
  wmove(editor.pattern,
        editor.row_idx - editor.row_offset,
        editor.column - editor.col_offset);
}
