#include <stdarg.h>
#include <assert.h>
#include <form.h>
#include <cdk/cdk.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "studio.h"
#include "project.h"
#include "update.h"
#include "event.h"
#include "columns.h"

typedef enum {
  EDITOR_MODE_PATTERN,
  EDITOR_MODE_STUDIO
} editor_mode_t;

typedef struct {
  CDKSCREEN* cdk_screen;
  WINDOW* ncurses_screen;
  WINDOW* console;
  struct {
    WINDOW* stats;
    WINDOW* header;
    WINDOW* pos;
    WINDOW* main;

    row_idx_t row_idx;
    int column;

    int rows;
    int cols;
    int row_offset;
    int col_offset;

    int octave;
  } pattern;
  struct {
    WINDOW* count;
    WINDOW* header;
    WINDOW* pos;
    WINDOW* main;

    device_idx_t device_idx;
    int column;

    int rows;
    int cols;
    int row_offset;
    int col_offset;
  } studio;
  struct {
    bool console;
    struct {
      bool stats;
      bool header;
      bool pos;
      bool main;
    } pattern;
    struct {
      bool count;
      bool header;
      bool pos;
      bool main;
    } studio;
  } refresh;

  editor_mode_t mode;
} editor_t;

editor_t editor;

static bool editor_initialized = false;

extern bool m_quit;

static int key_to_note[256];

#define K(CHAR) key_to_note[CHAR] = cnt++

/*
 * Set up the editors different windows
 */
void editor_init()
{
  /* Needed by th K-macro */
  int cnt = 1;

  memset(&key_to_note, 0, sizeof(key_to_note));
  memset(&editor, 0, sizeof(editor));
  editor.ncurses_screen = initscr();
  editor.cdk_screen = initCDKScreen(editor.ncurses_screen);

  raw();

  keypad(stdscr, TRUE);
  noecho();

  K('z'); K('s'); K('x'); K('d'); K('c'); K('v'); K('g'); K('b');
  K('h'); K('n'); K('j'); K('m');

  K('q'); K('2'); K('w'); K('3'); K('e'); K('r'); K('5'); K('t');
  K('6'); K('y'); K('7'); K('u');

  K('i'); K('9'); K('o'); K('0'); K('p');

  getmaxyx(stdscr, editor.pattern.rows, editor.pattern.cols);

  int debug_rows;
  if (true == debug_enabled)
    debug_rows = 10;
  else
    debug_rows = 0;

  const int heading_h = 8;
  const int pos_w = 3;
  const int pattern_h = editor.pattern.rows - heading_h - debug_rows;
  const int pattern_w = editor.pattern.cols - pos_w;

  editor.pattern.stats  = newwin(heading_h,     pos_w,         0, 0);
  editor.pattern.header = newwin(heading_h, pattern_w,         0, pos_w);
  editor.pattern.pos    = newwin(pattern_h,     pos_w, heading_h, 0);
  editor.pattern.main   = newwin(pattern_h, pattern_w, heading_h, pos_w);

  editor.studio.count   = newwin(        2,     pos_w, 0, 0);
  editor.studio.header  = newwin(        2, pattern_w, 0, pos_w);
  editor.studio.pos     = newwin(pattern_h,     pos_w, 2, 0);
  editor.studio.main    = newwin(pattern_h, pattern_w, 2, pos_w);

  if (true == debug_enabled) {
    const int height = editor.pattern.rows - 3 - debug_rows + 3;
    editor.console = newwin(debug_rows, editor.pattern.cols, height, 0);
    scrollok(editor.console, TRUE);
    wsetscrreg(editor.console, 0, debug_rows);
  }

  wborder(editor.console, ' ', ' ', '-',' ',' ',' ',' ',' ');

  wborder(editor.pattern.stats, ' ', '|', ' ','-',' ','|','-','+');
  wborder(editor.pattern.header, ' ', ' ', ' ','-',' ','|','-','+');

  wborder(editor.studio.count, ' ', '|', ' ','-',' ','|','-','+');
  wborder(editor.studio.header, ' ', ' ', ' ','-',' ','|','-','+');

  scrollok(editor.pattern.pos, true);
  scrollok(editor.pattern.main, true);

  scrollok(editor.studio.pos, true);
  scrollok(editor.studio.main, true);

  noecho();
  cbreak();
  wtimeout(editor.pattern.main, 1);
  keypad(editor.pattern.main, TRUE);

  wtimeout(editor.studio.main, 1);
  keypad(editor.studio.main, TRUE);

  if (true == debug_enabled) {
    wtimeout(editor.console, 1);
    keypad(editor.console, TRUE);
  }

  editor_set_octave(1);

  editor.refresh.pattern.stats = true;
  editor.refresh.pattern.header = true;
  editor.refresh.pattern.pos = true;
  editor.refresh.pattern.main = true;

  editor.refresh.console = true;

  editor_initialized = true;
}

char* editor_ask_for_project_filename(char* filename, char* name)
{
  CDKFSELECT *fselect;
  char *holder;
  fselect = newCDKFselect(editor.cdk_screen, CENTER, CENTER, -4, -20,
                          name, "File: ",
                          A_NORMAL, '_', A_REVERSE,
                          "</5>", "</48>", "</N>", "</N>",
                          TRUE, FALSE);
  holder = activateCDKFselect(fselect, 0);
  if (fselect->exitType != vNORMAL) {
    destroyCDKFselect(fselect);
    refreshCDKScreen(editor.cdk_screen);
    return NULL;
  }

  filename = copyChar(holder);
  destroyCDKFselect(fselect);
  refreshCDKScreen(editor.cdk_screen);
  return filename;
}

bool editor_ask_for_overwrite(char* filename, char* name)
{
  CDKDIALOG *popup;
  int choice;
  char message_txt[100];
  char* message[1];
  message[0] = &message_txt[0];

  sprintf(message_txt, "Overwrite %s as '%s'?", name, filename);

  char* button[2];

  button[0] = "No";
  button[1] = "Yes";

  popup = newCDKDialog(editor.cdk_screen, CENTER, CENTER,
                       message, 1,
                       button, 2,
                       A_REVERSE, TRUE,
                       TRUE, FALSE);

  drawCDKDialog (popup, TRUE);

  choice = activateCDKDialog (popup, 0);

  destroyCDKDialog (popup);

  refreshCDKScreen (editor.cdk_screen);

  return choice;
}

static void refresh_pattern_row(pattern_idx_t pattern_idx,
                         row_idx_t row_idx,
                         int width)
{
  assert(true == editor_initialized);

  if ((row_idx < editor.pattern.row_offset) ||
      (row_idx > editor.pattern.rows)) {
    return;
  }

  row_idx_t current_row_idx = project_get_row_idx(pattern_idx);

  /* WANNADO: Optimize this for various use cases. */
  char buf[MAX_ROW_LENGTH + 1];
  char scrolled_buf[MAX_ROW_LENGTH + 1];
  scrolled_buf[0] = 0;
  project_get_pattern_row(buf, pattern_idx, row_idx);

  /* Truncate line length */
  /* TODO: Scroll the line horisontally using the editor.columns info */
  strncpy(scrolled_buf, buf, editor.pattern.cols - 3);
  scrolled_buf[editor.pattern.cols - 2] = 0;

  const bool current_row = ((row_idx == current_row_idx));

  /* Mark the cursor of pattern row index with reverse video */
  if (true == current_row) {
    wattron(editor.pattern.pos, A_REVERSE);
  }

  /* Pattern row index */
  mvwprintw(editor.pattern.pos,
            row_idx - editor.pattern.row_offset,
            0, "%02x", row_idx);

  /* Go back to normal video if in reverse mode, but do the same for the
   * pattern window. */
  if (true == current_row) {
    wattroff(editor.pattern.pos, A_REVERSE);
    wattron(editor.pattern.main, A_REVERSE);
  }

  int offset = editor.pattern.col_offset;

  scrolled_buf[width - 1 + offset] = 0;

  const int row = row_idx - editor.pattern.row_offset;

  mvwprintw(editor.pattern.main, row, 0, "%s", &scrolled_buf[offset]);

  /* Go back to normal video */
  if (true == current_row) {
    wattroff(editor.pattern.main, A_REVERSE);
  }

  editor.refresh.pattern.pos = true;
  editor.refresh.pattern.main = true;
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
  const note_idx_t notes = project_get_notes(pattern_idx, track_idx);
  const note_idx_t effects = project_get_effects(pattern_idx, track_idx);

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

void refresh_pattern_header()
{
  char device_buf[MAX_ROW_LENGTH];
  char instrument_buf[MAX_ROW_LENGTH];
  char setting_buf[MAX_ROW_LENGTH];

  device_buf[0] = instrument_buf[0] = setting_buf[0] = 0;

  int width = 0;

  CONST_GET(project, song_idx)();
  CONST_GET(project, song_part_idx)(song_idx);
  CONST_GET(project, part_idx)(song_idx, song_part_idx);
  CONST_GET(project, part_pattern_idx)(part_idx);
  CONST_GET(project, pattern_idx)(part_idx, part_pattern_idx);

  const track_idx_t tracks = project_get_tracks();

  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {

    CONST_GET(project, device_idx)(track_idx);
    CONST_GET(project, instrument_idx)(pattern_idx, track_idx);
    CONST_GET(project, setting_idx)(pattern_idx, track_idx);

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
      mvwprintw(editor.pattern.header, 6, width - 1, "|");
    }
  }

  int w, h;

  getmaxyx(editor.pattern.header, h, w);

  h = h;

  if (strlen(device_buf) > (unsigned int)w) {
    device_buf[w + editor.pattern.col_offset] = 0;
    instrument_buf[w + editor.pattern.col_offset] = 0;
    setting_buf[w + editor.pattern.col_offset] = 0;
  }

  mvwprintw(editor.pattern.header, 3, 0, &(device_buf[editor.pattern.col_offset]));
  mvwprintw(editor.pattern.header, 4, 0, &(instrument_buf[editor.pattern.col_offset]));
  mvwprintw(editor.pattern.header, 5, 0, &(setting_buf[editor.pattern.col_offset]));

  editor.refresh.pattern.header = true;
}

#undef CONCAT

/*
 * Render a pattern in the pattern editor.
 */
static void refresh_pattern(pattern_idx_t pattern_idx)
{
  int w, h;

  assert(true == editor_initialized);

  getmaxyx(editor.pattern.main, h, w);

  row_idx_t length = project_get_pattern_rows(pattern_idx);

  debug("Rendering pattern of %d rows", length);

  mvwprintw(editor.pattern.stats, 0, 3, "%02x", pattern_idx);

  if (h < length)
    length = h;

  werase(editor.pattern.pos);
  werase(editor.pattern.main);

  for (row_idx_t ridx = editor.pattern.row_offset; ridx < length + editor.pattern.row_offset; ++ridx) {
    refresh_pattern_row(pattern_idx, ridx, w);
  }

  editor.refresh.pattern.stats = true;

  refresh_pattern_header();
}

void editor_refresh_tempo()
{
  const tempo_t tempo = 0; //get_tempo();
  mvwprintw(editor.pattern.stats, 3, 0, "%02x", tempo);
  editor.refresh.pattern.stats = true;
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

static void refresh_studio_row(device_idx_t device_idx, int width) {
  if ((device_idx < editor.studio.row_offset) ||
      (device_idx > editor.studio.rows)) {
    return;
  }

  /* WANNADO: Optimize this for various use cases. */
  char buf[MAX_ROW_LENGTH + 1];
  char scrolled_buf[MAX_ROW_LENGTH + 1];
  scrolled_buf[0] = 0;
  sprintf(buf, "%s", studio_get_device_name(device_idx));

  /* Truncate line length */
  /* TODO: Scroll the line horisontally using the editor.columns info */
  strncpy(scrolled_buf, buf, editor.studio.cols - 3);
  scrolled_buf[editor.studio.cols - 2] = 0;

  const bool current_row = ((device_idx == editor.studio.device_idx));

  /* Mark the cursor of pattern row index with reverse video */
  if (true == current_row) {
    wattron(editor.studio.pos, A_REVERSE);
  }

  /* Pattern row index */
  mvwprintw(editor.studio.pos, device_idx - editor.studio.row_offset, 0, "%02x", device_idx);

  /* Go back to normal video if in reverse mode, but do the same for the
   * pattern window. */
  if (true == current_row) {
    wattroff(editor.studio.pos, A_REVERSE);
    wattron(editor.studio.main, A_REVERSE);
  }

  //int offset = editor.studio_col_offset;
  int offset = 0;

  scrolled_buf[width - 1 + offset] = 0;

  const int row = device_idx; //  - editor.studio_row_offset;

  mvwprintw(editor.studio.main, row, 0, "%s", &scrolled_buf[offset]);

  /* Go back to normal video */
  if (true == current_row) {
    wattroff(editor.studio.main, A_REVERSE);
  }

  editor.refresh.studio.pos = true;
  editor.refresh.studio.main = true;
}

static void enter_studio_editor_mode()
{
  editor.mode = EDITOR_MODE_STUDIO;
  werase(editor.studio.count);
  werase(editor.studio.header);
  werase(editor.studio.pos);
  werase(editor.studio.main);

  mvwprintw(editor.studio.pos, 0, 0, "%02x", studio_get_devices());
  mvwprintw(editor.studio.header, 0, 0, "Name");
  mvwprintw(editor.studio.header, 32, 0, "Input");
  mvwprintw(editor.studio.header, 64, 0, "Output");

  const device_idx_t devices = studio_get_devices();

  int w, h;
  getmaxyx(editor.studio.main, h, w);

  h = h;

  for (device_idx_t device_idx = 0; device_idx < devices; device_idx++) {
    refresh_studio_row(device_idx, w);
  }

  editor.refresh.studio.count = true;
  editor.refresh.studio.header = true;
  editor.refresh.studio.pos = true;
  editor.refresh.studio.main = true;
  editor_refresh_windows();
}

static void enter_pattern_editor_mode()
{
  editor.mode = EDITOR_MODE_PATTERN;
  editor.refresh.pattern.stats = true;
  editor.refresh.pattern.header = true;
  editor.refresh.pattern.pos = true;
  editor.refresh.pattern.main = true;
  editor_refresh_windows();
}

static void pattern_read_kbd()
{
  const column_idx_t column_idx = columns_get_column_idx();
  const column_type_t column_type = columns_get_column_type(column_idx);

  const project_mode_t mode = project_get_project_mode();

  const bool edit = project_get_edit();
  const track_idx_t track_idx = columns_get_track_idx(column_idx);

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
  const pattern_idx_t pattern_idx =
    project_get_pattern_idx(part_idx, part_pattern_idx);
  const device_idx_t device_idx = project_get_device_idx(track_idx);
  const instrument_idx_t instrument_idx =
    project_get_instrument_idx(pattern_idx, track_idx);
  const row_idx_t rows = project_get_pattern_rows(pattern_idx);
  const row_idx_t row_idx = project_get_row_idx();

  int c = wgetch(editor.pattern.main);

  if (27 == c) {
    c = wgetch(editor.pattern.main);
    debug("1: %d", c);
    if (79 == c) {
      const part_pattern_idx_t part_pattern_idx =
        project_get_part_pattern_idx(part_idx);

      c = wgetch(editor.pattern.main);
      debug("4: %d", c);
      if (100 == c) { // Ctrl + LEFT
        update_pattern_idx(part_idx, part_pattern_idx, pattern_idx - 1);
        c = -1;
      } else if (99 == c) { // Ctrl + RIGHT
        update_pattern_idx(part_idx, part_pattern_idx, pattern_idx + 1);
        c = -1;
      }
    } else if (27 == c) {
      c = wgetch(editor.pattern.main);
      debug("5: %d", c);
      if (79 == c) {
        const song_part_idx_t song_part_idx =
        project_get_song_part_idx(song_idx);
        c = wgetch(editor.pattern.main);
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
      c = wgetch(editor.pattern.main);
      debug("2: %d", c);
      if (49 == c) {
        c = wgetch(editor.pattern.main);
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
    track_idx_t track_idx;
    note_idx_t note_idx;
    device_idx_t device_idx;
    instrument_idx_t instrument_idx;
    key_t key;
  } note_off;

  if (note_off.key != 0) {
    update_instrument_note_off(note_off.track_idx,
                               note_off.note_idx,
                               note_off.device_idx,
                               note_off.instrument_idx,
                               note_off.key,
                               127);
    memset(&note_off, 0, sizeof(note_off));
  }

  switch (c) {
  case KEY_F(1):
    if (editor.pattern.octave > 1) {
      editor_set_octave(editor.pattern.octave - 1);
    }
    break;
  case KEY_F(2):
    if (editor.pattern.octave < 9) {
      editor_set_octave(editor.pattern.octave + 1);
    }
    break;
  case KEY_F(9):
    if (PROJECT_MODE_STOPPED == mode)
      update_project_mode(PROJECT_MODE_PLAY_PATTERN);
    break;
  case KEY_F(10):
    if (PROJECT_MODE_STOPPED == mode)
      update_project_mode(PROJECT_MODE_PLAY_PART);
    break;
  case KEY_F(11):
    if (PROJECT_MODE_STOPPED == mode)
      update_project_mode(PROJECT_MODE_PLAY_SONG);
    break;
  case KEY_F(12):
    if (PROJECT_MODE_STOPPED == mode)
      update_project_mode(PROJECT_MODE_PLAY_PROJECT);
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
    if (PROJECT_MODE_STOPPED != mode) {
      update_project_mode(PROJECT_MODE_STOPPED);
      project_notes_off(pattern_idx);
    }
    update_edit(!project_get_edit());
    break;
  default:
    switch (column_type) {
    case COLUMN_TYPE_NOTE:{
      key_t key = key_to_note[c];
      if (0 != key) {
        const note_idx_t note_idx = columns_get_note_idx(column_idx);
        key += 12 * (editor.pattern.octave - 1);
        if (true == edit) {
          update_key(pattern_idx, row_idx, track_idx, note_idx, key);
          update_velocity(pattern_idx, row_idx, track_idx, note_idx,
                          127);
          update_row_idx(pattern_idx, row_idx + (quantization + 1), rows);
        }

        // Make the instrument play the note
        update_instrument_note_on(track_idx,
                                  note_idx,
                                  device_idx,
                                  instrument_idx,
                                  key,
                                  127);

        // Remember which note to turn off
        note_off.track_idx = track_idx;
        note_off.note_idx = note_idx,
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
    } else if (9 == c) { /* TAB = Studio editor */
      enter_studio_editor_mode();
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

static void studio_read_kbd()
{
  int c = wgetch(editor.pattern.main);
  if (9 == c) { /* TAB = Pattern editor */
    enter_pattern_editor_mode();
  } else if (-1 != c) {
    debug("Unhandled key %d", c);
  }
}

void editor_read_kbd() {
  assert(true == editor_initialized);

  switch (editor.mode) {
  case EDITOR_MODE_PATTERN:
    pattern_read_kbd();
    break;
  case EDITOR_MODE_STUDIO:
    studio_read_kbd();
    break;
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
  REFRESH(pattern.stats);
  REFRESH(pattern.header);
  REFRESH(pattern.pos);
  REFRESH(pattern.main);

  REFRESH(studio.count);
  REFRESH(studio.header);
  REFRESH(studio.pos);
  REFRESH(studio.main);

  switch (editor.mode) {
  case EDITOR_MODE_PATTERN: {
    const column_idx_t column_idx = columns_get_column_idx();
    const row_idx_t row_idx = project_get_row_idx();
    wmove(editor.pattern.main, row_idx - editor.pattern.row_offset, columns_get_column(column_idx) - editor.pattern.col_offset);
    break;
  }
  case EDITOR_MODE_STUDIO: {
    //    const column_idx_t column_idx = 0; // studio_columns_get_column_idx();
    const row_idx_t device_idx = editor.studio.device_idx;
    wmove(editor.studio.main, device_idx - editor.studio.row_offset, 0 /*studio_columns_get_column(column_idx)*/ - editor.studio.col_offset);
    break;
  }
  }
}

#undef REFRESH

void editor_set_octave(int octave)
{
  assert((octave >= 1) && (octave <= 9));
  editor.pattern.octave = octave;
  mvwprintw(editor.pattern.stats, 5, 1, "%x", editor.pattern.octave);
  editor.refresh.pattern.stats = true;
}

void editor_cleanup()
{
  assert(true == editor_initialized);

  echo();
  noraw();
  nocbreak();

  delwin(editor.pattern.stats);
  delwin(editor.pattern.header);
  delwin(editor.pattern.pos);
  delwin(editor.pattern.main);

  delwin(editor.studio.count);
  delwin(editor.studio.header);
  delwin(editor.studio.pos);
  delwin(editor.studio.main);

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

  mvwprintw(editor.pattern.stats, 4, 1, "%c", c[edit]);

  editor.refresh.pattern.stats = true;
}

void editor_set_quantization(const quantization_t quantization)
{
  assert(quantization <= 0xf);

  mvwprintw(editor.pattern.stats, 5, 0, "%x", quantization);

  editor.refresh.pattern.stats = true;
}

void editor_set_project_mode(const project_mode_t mode)
{
  assert(0 == PROJECT_MODE_STOPPED);
  assert(1 == PROJECT_MODE_PLAY_PROJECT);
  assert(2 == PROJECT_MODE_PLAY_SONG);
  assert(3 == PROJECT_MODE_PLAY_PART);
  assert(4 == PROJECT_MODE_PLAY_PATTERN);

  const static char c[6] = {' ', 'a', 's', 'p', '>'};

  mvwprintw(editor.pattern.stats, 4, 1, "%c", c[mode]);

  editor.refresh.pattern.stats = true;
}

static void refresh_song_list(song_idx_t song_idx)
{
  song_idx_t songs = project_get_songs();
  for (song_idx_t idx = 0; idx < songs; idx++) {
    int column = MAX_NAME_LENGTH + idx * 3;
    if (song_idx == idx) {
      wattron(editor.pattern.header, A_REVERSE);
    }
    mvwprintw(editor.pattern.header, 0, column, "%02x", idx);
    if (song_idx == idx) {
      wattroff(editor.pattern.header, A_REVERSE);
    }
  }
  editor.refresh.pattern.header = true;
}

static void refresh_song_part_list(song_idx_t song_idx,
                                  song_part_idx_t song_part_idx)
{
  const song_part_idx_t song_parts = project_get_song_parts(song_idx);

  for (song_part_idx_t idx = 0; idx < song_parts; idx++) {
    const part_idx_t part_idx = project_get_part_idx(song_idx, idx);
    const int column = MAX_NAME_LENGTH + idx * 3;
    if (song_part_idx == idx) {
      wattron(editor.pattern.header, A_REVERSE);
    }
    mvwprintw(editor.pattern.header, 1, column, "%02x", part_idx);
    if (song_part_idx == idx) {
      wattroff(editor.pattern.header, A_REVERSE);
    }
  }
  editor.refresh.pattern.header = true;
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
      wattron(editor.pattern.header, A_REVERSE);
    }
    mvwprintw(editor.pattern.header, 2, column, "%02x", pattern_idx);
    if (part_pattern_idx == idx) {
      wattroff(editor.pattern.header, A_REVERSE);
    }
  }
  editor.refresh.pattern.header = true;
}

void editor_set_song_idx(const song_idx_t song_idx)
{
  mvwprintw(editor.pattern.stats, 0, 0, "%02x", song_idx);
  mvwprintw(editor.pattern.header, 0, 0, "%s", project_get_song_name(song_idx));

  refresh_song_list(song_idx);
  refresh_song_part_list(song_idx, project_get_song_part_idx(song_idx));

  editor.refresh.pattern.stats = true;
  editor.refresh.pattern.header = true;
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
  mvwprintw(editor.pattern.stats, 1, 0, "%02x", part_idx);
  mvwprintw(editor.pattern.header, 1, 0, "%s", project_get_part_name(part_idx));

  editor.refresh.pattern.stats = true;
  editor.refresh.pattern.header = true;
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
  mvwprintw(editor.pattern.stats, 2, 0, "%02x", pattern_idx);
  mvwprintw(editor.pattern.header, 2, 0, "%s", project_get_pattern_name(pattern_idx));

  editor.refresh.pattern.stats = true;
  editor.refresh.pattern.header = true;

  refresh_part_pattern_list(part_idx, part_pattern_idx);
  refresh_pattern(pattern_idx);
}

void editor_set_pattern_rows(const pattern_idx_t pattern_idx,
                             const row_idx_t rows)
{
  const song_idx_t song_idx = project_get_song_idx();
  const song_part_idx_t song_part_idx =
    project_get_song_part_idx(song_idx);
  const part_idx_t part_idx =
    project_get_part_idx(song_idx, song_part_idx);
  const part_pattern_idx_t part_pattern_idx =
    project_get_part_pattern_idx(part_idx);
  /* Only refresh if the change is done to the currently shown pattern */
  if (pattern_idx != project_get_pattern_idx(part_idx, part_pattern_idx)) {
    return;
  }
  mvwprintw(editor.pattern.stats, 6, 0, "%02x", rows);

  editor.refresh.pattern.stats = true;

  refresh_pattern(pattern_idx);
}

static void calculate_vertical_scroll(row_idx_t row_idx,
                                      row_idx_t rows,
                                      int height)
{
  if ((rows > height) && (row_idx >= (height / 2))) {
    editor.pattern.row_offset = (row_idx - (height / 2));
    if (editor.pattern.row_offset > (rows - height)) {
      editor.pattern.row_offset = (rows - height);
    }
  }
  else {
    editor.pattern.row_offset = 0;
  }
}

void editor_set_row_idx(const pattern_idx_t pattern_idx,
                        const row_idx_t row_idx)
{
  int h, w;
  int old_row_offset = editor.pattern.row_offset;
  getmaxyx(editor.pattern.main, h, w);
  h = h;
  calculate_vertical_scroll(row_idx, project_get_pattern_rows(pattern_idx), h);
  row_idx_t old_row_idx = editor.pattern.row_idx;

  int scroll_delta = editor.pattern.row_offset - old_row_offset;

  wscrl(editor.pattern.main, scroll_delta);
  wscrl(editor.pattern.pos, scroll_delta);

  refresh_pattern_row(pattern_idx, old_row_idx, w);
  editor.pattern.row_idx = row_idx;
  refresh_pattern_row(pattern_idx, row_idx, w);

  int start = 0;
  if (scroll_delta > 0) {
    start = h + old_row_offset;
  }
  else if (scroll_delta < 0) {
    scroll_delta *= -1; // abs;
    start = editor.pattern.row_offset;
  }
  for (int i = start ; i < start + scroll_delta; i++) {
    refresh_pattern_row(pattern_idx, i, w);
  }
}

void editor_set_column_idx(const column_idx_t column_idx)
{
  editor.pattern.column = columns_get_column(column_idx);
  /* TODO: Implement horizontal scolling */
  wmove(editor.pattern.main,
        editor.pattern.row_idx - editor.pattern.row_offset,
        editor.pattern.column - editor.pattern.col_offset);
}
