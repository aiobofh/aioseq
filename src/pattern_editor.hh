/**
 * @pattern_editor.hh
 */
#ifndef _PATTERN_EDITOR_HH_
#define _PATTERN_EDITOR_HH_

#ifndef PATTERN_EDITOR_FRIENDS
#define PATTERN_EDITOR_FRIENDS
#endif

#include <iostream>
#include <iomanip>
#include <string>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

using namespace std;

#include "sequencer.hh"
#include "track_client.hh"
#include "pattern_client.hh"
#include "part_client.hh"


/// Cursor column mode
enum CursorColumnMode {
  NOTE_KEY,
  NOTE_VELOCITY_HIGH_NIBBLE,
  NOTE_VELOCITY_LOW_NIBBLE,
  EFFECT_COMMAND_HIGH_NIBBLE,
  EFFECT_COMMAND_LOW_NIBBLE,
  EFFECT_VALUE_HIGH_NIBBLE,
  EFFECT_VALUE_LOW_NIBBLE
};

struct CursorColumn_s {
  CursorColumnMode mode;
  unsigned int track;
  unsigned int index;
  int column;
};

typedef struct CursorColumn_s CursorColumn;

typedef vector<CursorColumn*> CursorColumns;

/**
 * Pure virtual interface class for a pattern editor.
 *
 * A pattern editor is a slave object to the SequencerInterface API for
 * editing musical patterns and displaying the same.
 *
 * This specific interface implements three more interfaces.
 *
 * @copydoc TrackClientInterface
 * @copydoc PatternClientInterface
 * @copydoc PartClientInterface
 *
 */
class PatternEditorInterface : public virtual TrackClientInterface,
                               public virtual PatternClientInterface,
                               public virtual PartClientInterface {

public:

  /**
   * Default constructor.
   */
  PatternEditorInterface() {}

  /**
   * Default destructor.
   */
  virtual ~PatternEditorInterface() {}

};


/**
 * Template class with optimizable handling of a pattern editor.
 *
 * @copydetails PatternEditorInterface
 */
template<class SEQUENCER,
         class PATTERN_ROW,
         class TRACKS,
         class TRACK,
         class TRACK_ENTRIES,
         class TRACK_ENTRY,
         class NOTES,
         class NOTE,
         class EFFECTS,
         class EFFECT>
class PatternEditorTemplate : virtual public PatternEditorInterface {

  PATTERN_EDITOR_FRIENDS //  Unfortunatly needed for good unit-testing.

protected:

  /// Internal storage of the pointer to the sequencer master instance.
  SequencerInterface* sequencer;

  /// Display modes for the console output.
  enum DisplayMode {
    POLYPHONY,
    PERCUSSION
  };

  /// Internal storage of the current row index.
  unsigned int row_index;
  /// Internal storage of the current track index.
  unsigned int track_index;
  /// Internal storage of the current pattern index.
  unsigned int pattern_index;
  /// Internal storage of the current pattern length.
  unsigned int pattern_length;

  /// Internal storage of the current display mode.
  enum DisplayMode display_mode;

  /**
   * Block rendering - used upon connection when the sequencer will inform
   * the pattern editor about many things.
   */
  bool block_render;

  bool edit_mode;

  int column;
  int columns;
  CursorColumns cursor_column_list;

  int scancode_to_note_key[256];
  int scancode_to_hex_value[256];

  /**
   * Read a character from the terminal standard input.
   *
   * @todo Remove this with a non-blocking propper one.
   * @note This method will not have any code coverage.
   */
  virtual char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
      perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
      perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
      perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
      perror ("tcsetattr ~ICANON");
    return (buf);
  }


  /**
   * Render a three character representation of a musical note key.
   *
   * @param key The musical note key to render.
   */
  virtual void render_key(unsigned int key) {
    static string note[1 + 10 * 12 + 7] = {
      "---",
      "C-1","C#1","D-1","D#1","E-1","F-1","F#1","G-1","G#1","H-1","H#1","A-1",
      "C-2","C#2","D-2","D#2","E-2","F-2","F#2","G-2","G#2","H-2","H#2","A-2",
      "C-3","C#3","D-3","D#3","E-3","F-3","F#3","G-3","G#3","H-3","H#3","A-3",
      "C-4","C#4","D-4","D#4","E-4","F-4","F#4","G-4","G#4","H-4","H#4","A-4",
      "C-5","C#5","D-5","D#5","E-5","F-5","F#5","G-5","G#5","H-5","H#5","A-5",
      "C-6","C#6","D-6","D#6","E-6","F-6","F#6","G-6","G#6","H-6","H#6","A-6",
      "C-7","C#7","D-7","D#7","E-7","F-7","F#7","G-7","G#7","H-7","H#7","A-7",
      "C-8","C#8","D-8","D#8","E-8","F-8","F#8","G-8","G#8","H-8","H#8","A-8",
      "C-9","C#9","D-9","D#9","E-9","F-9","F#9","G-9","G#9","H-9","H#9","A-9",
      "C-a","C#a","D-a","D#a","E-a","F-a","F#a","G-a","G#a","H-a","H#a","A-a",
      "C-b","C#b","D-b","D#b","E-b","F-b","F#b"
    };
    cout << note[key] << " ";
  }

  /**
   * Render one hexadecimal (4 bits) value (a nibble of a byte).
   *
   * @param nibble Which of the 4 bits to render of the provided value.
   * @param value  The value to extract 4 bits and render in hex.
   */
  virtual void render_nibble(unsigned int nibble, unsigned int value) {
    cout << hex << ((value >> (nibble << 2)) & 0xf);
  }

  /**
   * Render the velocity of a musical note in hexadecimal.
   *
   * @param velocity Velocity value to render.
   */
  virtual void render_velocity(unsigned int velocity) {
    render_nibble(1, velocity);
    render_nibble(0, velocity);
    cout << " ";
  }

  /**
   * Render a musical note and its velocity value in hexadecimal.
   *
   * @param key      Musical note key to render.
   * @param velocity Velocity to render for the musical note.
   */
  virtual void render_note(unsigned int key, unsigned int velocity) {
    render_key(key);
    render_velocity(velocity);
  }

  /**
   * Render an effect event with its command and vale in hexadecimal.
   *
   * @param command Command number to render.
   * @param value   Command value to render.
   */
  virtual void render_effect(unsigned int command, unsigned int value) {
    render_nibble(1, command);
    render_nibble(0, command);
    render_nibble(1, value);
    render_nibble(0, value);
    cout << " ";
  }

  /**
   * Render the pattern row number.
   *
   * @param row_index Pattern row index to render.
   */
  virtual void render_row_number(unsigned int row_index) {
    cout << setfill('0') << setw(2) << hex << row_index;
    cout << " ";
  }

  /**
   * Reverse the video, setting the foreground color as background and the
   * background color as foreground at the current cursor position.
   */
  virtual void render_reverse_video() {
    cout << dec << (char)27 << "[7m";
  }

  /**
   * Set the video to its normal mode where foreground is foreground color
   * and background is background color at the current cursor position.
   */
  virtual void render_normal_video() {
    cout << dec << (char)27 << "[0m";
  }

  /**
   * Clear the screen and move the cursor to the upper left corner.
   */
  virtual void clear_screen() {
    cout << dec << (char)27 << "[2J";
    move_cursor_to_row(0);
  }

  /**
   * Clear the list of column v.s. console columns mapping.
   */
  virtual void clear_cursor_columns() {
    for (unsigned int i = 0; i < cursor_column_list.size(); i++) {
      CursorColumn *cc =
        dynamic_cast<CursorColumn*>(cursor_column_list.at(i));
      delete cc;
      cursor_column_list.pop_back();
    }
    cursor_column_list.clear();
  }


  /**
   * Add columns for a note entry.
   *
   * @param offset Console column offset.
   * @param track  Track index for the columns.
   * @param note   Note index for the columns.
   *
   * @return The new console column offset.
   */
  virtual unsigned int add_note_columns(unsigned int offset, unsigned int track, unsigned int note) {
    CursorColumn *key = new CursorColumn();
    CursorColumn *velocity1 = new CursorColumn();
    CursorColumn *velocity2 = new CursorColumn();

    /* Remember which track and which note the columns are on. */
    key->track = velocity1->track = velocity2->track = track;
    key->index = velocity1->index = velocity2->index = note;

    /* Keep a record for every key, and both velocity nibbles. */
    key->mode = NOTE_KEY;
    velocity1->mode = NOTE_VELOCITY_HIGH_NIBBLE;
    velocity2->mode = NOTE_VELOCITY_LOW_NIBBLE;

    /* Calculate console cursor offsets. */
    key->column = offset;
    offset += 3;

    /* Separator column between key and velocity. */
    offset += 1;

    velocity1->column = offset++;
    velocity2->column = offset++;

    /* Add the columns (key, high nibble, low nibble) to the list. */
    cursor_column_list.push_back(key);
    cursor_column_list.push_back(velocity1);
    cursor_column_list.push_back(velocity2);

    /* Separator column between note entries. */
    offset++;
    return offset;
  }


  /**
   * Add columns for an effect entry.
   *
   * @param offset Console column offset.
   * @param track  Track index for the columns.
   * @param effect Effect index for the columns.
   *
   * @return The new console column offset.
   */
  virtual unsigned int add_effect_columns(unsigned int offset, unsigned int track, unsigned int effect) {
    CursorColumn *command1 = new CursorColumn();
    CursorColumn *command2 = new CursorColumn();
    CursorColumn *value1 = new CursorColumn();
    CursorColumn *value2 = new CursorColumn();

    /* Remember which track and which effect the columns are on. */
    command1->track = command2->track = value1->track = value2->track = track;
    command1->index = command2->index = effect;
    value1->index = value2->index = effect;

    /* Keep a record for every command and value nibbles. */
    command1->mode = EFFECT_COMMAND_HIGH_NIBBLE;
    command2->mode = EFFECT_COMMAND_LOW_NIBBLE;
    value1->mode = EFFECT_VALUE_HIGH_NIBBLE;
    value2->mode = EFFECT_VALUE_LOW_NIBBLE;

    command1->column = offset++;
    command2->column = offset++;
    value1->column = offset++;
    value2->column = offset++;

    cursor_column_list.push_back(command1);
    cursor_column_list.push_back(command2);
    cursor_column_list.push_back(value1);
    cursor_column_list.push_back(value2);

    /* Separator column between effect entries. */
    offset++;
    return offset;
  }

  /**
   * Update the translation table from "columns" to real console columns.
   */
  virtual void update_cursor_columns() {

    // Skip the pattern row column, not editable.
    int cur = 4;

    columns = 0;

    clear_cursor_columns();

    /*
     * Push new entries to the list with the graphical offsets and indices to
     * whatever data the column represents.
     */
    TRACKS* tracks = dynamic_cast<TRACKS*>(sequencer->get_tracks());
    for (unsigned int track_index = 0; track_index < tracks->size(); track_index++) {
      TRACK* track = dynamic_cast<TRACK*>(tracks->at(track_index));
      unsigned int notes = track->get_notes();
      columns += notes * 3;
      unsigned int effects = track->get_effects();
      columns += effects * 4;
      for (unsigned int note = 0; note < notes; note++) {
        cur = add_note_columns(cur, track_index, note);
      }
      for (unsigned int effect = 0; effect < effects; effect++) {
        cur = add_effect_columns(cur, track_index, effect);
      }
    }

  }

  /**
   * Get the graphical console column from a data storage column.
   *
   * @param column The data storage column.
   *
   * @return The console column.
   */
  virtual unsigned int get_console_column(unsigned int column) {
    return cursor_column_list[column]->column;
  }

  /**
   * Move the cursor to left of the specified row.
   *
   * @param row The row to move to.
   */
  virtual void move_cursor_to_row(int row) {
    cout << dec << (char)27 << "[" << row << ";0H";
  }

  /**
   * Move the cursor to the specified column.
   *
   * @param column Column to move the cursor to.
   */
  virtual void move_cursor_to_column(int column) {
    cout << dec << (char)27 << "[" << column << "G" << flush;
  }

  /**
   * Render a pattern row with the current index at the current cursor
   * position.
   *
   * @param row_index The index of the row within the pattern to render.
   */
  virtual void render_row(unsigned int row_index) {
    //    int column = 0; // This will be passed on to each sub-renderer

    if (row_index == this->row_index) {
      render_reverse_video();
    }

    render_row_number(row_index);

    PATTERN_ROW* pattern_row =
      dynamic_cast<PATTERN_ROW*>(sequencer->get_row(row_index));
    TRACK_ENTRIES* track_entries =
      dynamic_cast<TRACK_ENTRIES*>(pattern_row->get_track_entries());

    for (unsigned int i = 0; i < track_entries->size(); i++) {
      TRACK_ENTRY* track_entry =
        dynamic_cast<TRACK_ENTRY*>(track_entries->at(i));
      NOTES* notes = dynamic_cast<NOTES*>(track_entry->get_notes());
      for (unsigned int i = 0; i < notes->size(); i++) {
        NOTE* note = dynamic_cast<NOTE*>(notes->at(i));
        render_note(note->get_key(), note->get_velocity());
      }
      EFFECTS* effects = dynamic_cast<EFFECTS*>(track_entry->get_effects());
      for (unsigned int i = 0; i < effects->size(); i++) {
        EFFECT* effect = dynamic_cast<EFFECT*>(effects->at(i));
        render_effect(effect->get_command(), effect->get_value());
      }
    }

    if (row_index == this->row_index) {
      render_normal_video();
    }

    cout << flush;
  }


  /**
   * Get the current console height.
   *
   * @return The console height in rows.
   */
  virtual unsigned int get_screen_height() {
    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);
    return win.ws_row;
  }


  /**
   * Get the current console width.
   *
   * @return The console width in columns.
   */
  virtual unsigned int get_screen_width() {
    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);
    return win.ws_col;
  }


  /**
   * Calculate the offset at which to start rendering the pattern rows,
   * depending on how much the pattern has scrolled.
   *
   * @return The offset at which the pattern should start to be rendered from.
   */
  virtual unsigned int calculate_pattern_render_offset(int row_index) {
    int row = static_cast<int>(row_index);
    int row_count = static_cast<int>(pattern_length);
    int screen_height = static_cast<int>(get_screen_height());
    int offset = 0;

    /*
     * Make pattern scroll if needed, with the currently selected row in the
     * middle of the screen.
     */
    if (row_count > screen_height) {
      int half_screen = (screen_height / 2);
      if (row > half_screen) {
        /*
        if (((row - screen_height) <= half_screen) &&
            (row <= (screen_height - half_screen))) {
          offset = row - half_screen;
        }
        */
        if (((row - screen_height) <= half_screen) &&
            (row < (row_count - half_screen))) {
          offset = row - half_screen;
        }
        else {
          offset = row_count - screen_height;
        }
      }
    }
    return static_cast<unsigned int>(offset);
  }


  /**
   * Render a complete pattern.
   */
  virtual void render_pattern() {
    if (true == block_render) {
      return;
    }
    unsigned int offset = calculate_pattern_render_offset(this->row_index);
    unsigned int rows_to_print = min(pattern_length,
                                     get_screen_height() + offset);
    for (unsigned int i = offset; i < rows_to_print; i++) {
      render_row(i);
      if (i < rows_to_print - 1) {
        cout << endl;
      }
    }
  }

  /**
   * Program main-loop.
   *
   * @return True if normal exit, false if not.
   */
  virtual bool main_loop() {
    unsigned char c = this->getch();
    bool retval = true;

    /*
     * Arrow keys
     */
    if ('B' == c) {
      sequencer->set_pattern_row_index(row_index + 1);
    }
    else if ('A' == c) {
      sequencer->set_pattern_row_index(row_index - 1);
    }
    if ('C' == c) {
      if (columns - 1 > column) {
        column++;
        move_cursor_to_column(get_console_column(column));
      }
    }
    else if ('D' == c) {
      if (0 < column) {
        column--;
        move_cursor_to_column(get_console_column(column));
      }
    }
    else if ('Q' == c) {
      retval = false;
    }
    else {
      if (true == edit_mode) {

        unsigned int track = cursor_column_list[column]->track;
        unsigned int index = cursor_column_list[column]->index;
        unsigned int mode = cursor_column_list[column]->mode;

        if (NOTE_KEY == mode) {
          int v = -1;
          v = scancode_to_note_key[c];
          if (0xffff != v) {
            sequencer->set_key(track, index, v);
          }
        }
        else {
          int v = -1;
          v = scancode_to_hex_value[c];
          if (0xffff != v) {
            switch (mode) {
            case NOTE_VELOCITY_HIGH_NIBBLE:
              sequencer->set_velocity(track, index, v, true);
              break;
            case NOTE_VELOCITY_LOW_NIBBLE:
              sequencer->set_velocity(track, index, v, false);
              break;
            case EFFECT_COMMAND_HIGH_NIBBLE:
              sequencer->set_command(track, index, v, true);
              break;
            case EFFECT_COMMAND_LOW_NIBBLE:
              sequencer->set_command(track, index, v, false);
              break;
            case EFFECT_VALUE_HIGH_NIBBLE:
              sequencer->set_value(track, index, v, true);
              break;
            case EFFECT_VALUE_LOW_NIBBLE:
              sequencer->set_value(track, index, v, false);
              break;
            }
          }
        }
      }
    }

    return retval;
  }

  /**
   * Set a nibble and re-rerender it.
   *
   * @param value       The value to set (0-15)
   * @param high_nibble True = Hich nibble, False = Low nibble.
   */
  virtual void set_nibble(int value, bool high_nibble) {
    move_cursor_to_column(get_console_column(column));
    render_reverse_video();
    render_nibble(0, value);
    move_cursor_to_column(get_console_column(column));
    render_normal_video();
    column++;
    move_cursor_to_column(get_console_column(column));
    cout << flush;
  }


public:

  /**
   * Create a new instance of a pattern editor class with the positbility to
   * render/update and edit a pattern.
   *
   * @param sequencer A reference to the sequencer master to use.
   */
  PatternEditorTemplate(SequencerInterface* sequencer) : sequencer(sequencer), row_index(0), track_index(0), display_mode(POLYPHONY), block_render(false), edit_mode(true) {
    int *s = &scancode_to_note_key[0];
    /*
     * Set-up a scancode to note mapping table.
     */
    fill_n(s, 256, 0xffff);

          s['9']=26; s['0']=28;
    s['i']=25; s['o']=27; s['p']=29; s['å']=30;

          s['2']=14; s['3']=16;            s['5']=19; s['6']=21; s['7']=23;
    s['q']=13; s['w']=15; s['e']=17; s['r']=18; s['t']=20; s['y']=22; s['u']=24;
          s['k']=14; s['l']=16;
    s['m']=13; s[',']=15; s['.']=17; s['-']=18;

          s['a']=2;  s['s']=4;             s['f']=7;  s['g']=9; s['h']=11;
    s['<']=1;  s['z']=3;  s['x']=5;  s['c']=6;  s['v']=8;  s['b']=10; s['n']=12;
    /*
     * Set-up a scancode to hexadecimal nibble mapping table.
     */
    s = &scancode_to_hex_value[0];

    fill_n(s, 256, 0xffff);

    s['0'] = 0x0;
    s['1'] = 0x1;
    s['2'] = 0x2;
    s['3'] = 0x3;
    s['4'] = 0x4;
    s['5'] = 0x5;
    s['6'] = 0x6;
    s['7'] = 0x7;
    s['8'] = 0x8;
    s['9'] = 0x9;
    s['a'] = 0xa;
    s['b'] = 0xb;
    s['c'] = 0xc;
    s['d'] = 0xd;
    s['e'] = 0xe;
    s['f'] = 0xf;

  }
  virtual ~PatternEditorTemplate() {}

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(int)
  void set_track_index(unsigned int track_index) {
    if (this->track_index != track_index) {
      this->track_index = track_index;
      render_pattern();
    }
  }

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(int)
  void set_pattern_index(unsigned int pattern_index) {
    if (pattern_index != this->pattern_index) {
      this->pattern_index = pattern_index;
      update_cursor_columns();
      render_pattern();
    }
  }

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  void set_pattern_row_index(int pattern_row_index) {
    int row = this->row_index;
    int prev_offset = calculate_pattern_render_offset(row);
    this->row_index = static_cast<unsigned int>(pattern_row_index);

    int curr_offset = calculate_pattern_render_offset(this->row_index);
    int screen_height = get_screen_height();

    if ((curr_offset < prev_offset) ||
        ((row == 0) && (pattern_row_index == pattern_length - 1))) {
      move_cursor_to_row(0);
      render_pattern();
      move_cursor_to_row(this->row_index + 1 - curr_offset);
      move_cursor_to_column(get_console_column(this->column));
    } else if (curr_offset > prev_offset) {
      move_cursor_to_row(screen_height);
      cout << endl;
      render_row(screen_height + curr_offset - 1);

      // Re-render the previously selected row.
      move_cursor_to_row(row - prev_offset);
      render_row(row);

      // And render the selected row.
      move_cursor_to_row(this->row_index + 1 - curr_offset);
      render_row(this->row_index);
      move_cursor_to_column(get_console_column(this->column));
    } else {
      // Re-render the previously selected row.
      move_cursor_to_row(row + 1 - prev_offset);
      render_row(row);

      // And render the selected row.
      move_cursor_to_row(this->row_index + 1 - curr_offset);
      render_row(this->row_index);
      move_cursor_to_column(get_console_column(this->column));
    }
    cout << flush;
  }

  /// @copydoc PatternClientInterface::set_pattern_length(unsigned int)
  virtual void set_pattern_length(unsigned int pattern_length) {
    /**
     * @todo Only render if the newly added bit of lenght is currently fitting
     *       in the display terminal. Otherwize don't re-render.
     */
    this->pattern_length = pattern_length;
    render_pattern();
  }

  /// @copydoc PatternClientInterface::set_key(unsigned int, unsigned int, int)
  virtual void set_key(unsigned int track_index, unsigned int note_index, int key) {
    render_reverse_video();
    render_key(key);
    move_cursor_to_column(get_console_column(this->column));
    render_normal_video();
    sequencer->set_pattern_row_index(row_index + 1);
  }

  /// @copydoc PatternClientInterface::set_velocity(unsigned int, unsigned int, int, bool)
  virtual void set_velocity(unsigned int track_index, unsigned int note_index, int velocity, bool high_nibble) {
    set_nibble(velocity, high_nibble);
  }

  /// @copydoc PatternClientInterface::set_command(unsigned int, unsigned int, int, bool)
  virtual void set_command(unsigned int track_index, unsigned int effect_index, int command, bool high_nibble) {
    set_nibble(command, high_nibble);
  }

  /// @copydoc PatternClientInterface::set_value(unsigned int, unsigned int, int, bool)
  virtual void set_value(unsigned int track_index, unsigned int effect_index, int value, bool high_nibble) {
    set_nibble(value, high_nibble);
  }

  /**
   * The main program for the pattern editor.
   *
   * @param argc Command arguments count.
   * @param argv A pointer to an array of character-strings, one for each
   *             argument.
   *
   * @return Program exit status.
   */
  int main(int argc, char* argv[]) {
    column = 0;
    block_render = true;
    sequencer->register_client(dynamic_cast<ClientPrimitiveInterface*>(this));
    block_render = false;
    clear_screen();
    update_cursor_columns();
    render_pattern();
    move_cursor_to_row(0);
    move_cursor_to_column(get_console_column(this->column));
    while (true == main_loop())
      ;
    sequencer->unregister_client(dynamic_cast<ClientPrimitiveInterface*>(this));
    clear_screen();
    clear_cursor_columns();
    return 0;
  }

};


/**
 * PatternEditor is a specialized class of the template PatternEditorTemplate
 * for performance and reuse.
 */
typedef PatternEditorTemplate<Sequencer,
                              PatternRow,
                              Tracks,
                              Track,
                              TrackEntries,
                              TrackEntry,
                              Notes,
                              Note,
                              Effects,
                              Effect> PatternEditor;

#endif
