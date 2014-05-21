/**
 * @file pattern_editor.hh
 *
 * The pattern editor of AiOSeq is created in a very minimalist way. The idea
 * is to mimic the old 80's and 90's sample based trackers in layout and
 * functionality.
 *
 */
#ifndef _PATTERN_EDITOR_HH_
#define _PATTERN_EDITOR_HH_

#ifndef PATTERN_EDITOR_FRIENDS
#define PATTERN_EDITOR_FRIENDS
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

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


/**
 * The cursor column mode identifies each column of the pattern editor as a
 * point and type of data storage.
 */
enum CursorColumnMode {
  NOTE_KEY,
  NOTE_VELOCITY_HIGH_NIBBLE,
  NOTE_VELOCITY_LOW_NIBBLE,
  EFFECT_COMMAND_HIGH_NIBBLE,
  EFFECT_COMMAND_LOW_NIBBLE,
  EFFECT_VALUE_HIGH_NIBBLE,
  EFFECT_VALUE_LOW_NIBBLE
};

/**
 * Column identifier entry data type.
 */
struct CursorColumn_s {
  CursorColumnMode mode; ///< The column data entry mode
  unsigned int track;    ///< The track index of a column.
  unsigned int index;    ///< The note or effect index within the track.
  int column;            ///< The absolute console output column mapped.
};

/**
 * Convenience type naming for a struct CursorColumn_s.
 */
typedef struct CursorColumn_s CursorColumn;

/**
 * Convenience type naming for a vector of CursorColumn entries.
 */
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

  /// The number of lines that the heading takes up.
  const int HEADING_LINES = 2;

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


  /**
   * Editing mode state where true tells the editor to accept editing,
   * otherwise only navigation and display is allowed.
   */
  bool edit_mode;


  /**
   * Current column of the pattern editor cursor.
   */
  int column;


  /**
   * The total number of columns currently available, this is updated in the
   * update_cursor_columns() method which need to be called whenever a new
   * pattern layout is set (number of tracks changed, or number of notes or
   * effects in a track).
   */
  int columns;


  /**
   * A vector to keep track of the column mapping towards the actual console
   * column and which track and note/effect this column corresponds to.
   */
  CursorColumns cursor_column_list;


  /**
   * A look-up table for mapping the computer keyboard to notes, see the
   * constructor documentation for more information.
   */
  int scancode_to_note_key[256];


  /**
   * A look-up table for mapping the keyboard for hexadecimal nibble input.
   */
  int scancode_to_hex_value[256];


  /**
   * Read a character from the terminal standard input.
   *
   * @return A character pressed on the keyboard.
   *
   * @todo Remove this with a non-blocking propper one.
   *
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
  virtual void render_key(stringstream* out, unsigned int key) {
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
    *out << note[key] << " ";
  }


  /**
   * Render one hexadecimal (4 bits) value (a nibble of a byte).
   *
   * @param nibble Which of the 4 bits to render of the provided value.
   * @param value  The value to extract 4 bits and render in hex.
   */
  virtual void render_nibble(stringstream* out, unsigned int nibble, unsigned int value) {
    *out << hex << ((value >> (nibble << 2)) & 0xf);
  }


  /**
   * Render the velocity of a musical note in hexadecimal.
   *
   * @param velocity Velocity value to render.
   */
  virtual void render_velocity(stringstream* out, unsigned int velocity) {
    render_nibble(out, 1, velocity);
    render_nibble(out, 0, velocity);
    *out << " ";
  }


  /**
   * Render a musical note and its velocity value in hexadecimal.
   *
   * @param key      Musical note key to render.
   * @param velocity Velocity to render for the musical note.
   */
  virtual void render_note(stringstream* out, unsigned int key, unsigned int velocity) {
    render_key(out, key);
    render_velocity(out, velocity);
  }


  /**
   * Render an effect event with its command and vale in hexadecimal.
   *
   * @param command Command number to render.
   * @param value   Command value to render.
   */
  virtual void render_effect(stringstream* out, unsigned int command, unsigned int value) {
    render_nibble(out, 1, command);
    render_nibble(out, 0, command);
    render_nibble(out, 1, value);
    render_nibble(out, 0, value);
    *out << " ";
  }


  /**
   * Render the pattern row number.
   *
   * @param row_index Pattern row index to render.
   */
  virtual void render_row_number(stringstream* out, unsigned int row_index) {
    *out << setfill('0') << setw(2) << hex << row_index;
    *out << " ";
  }


  /**
   * Reverse the video, setting the foreground color as background and the
   * background color as foreground at the current cursor position.
   */
  virtual void render_reverse_video(stringstream* out) {
    *out << dec << (char)27 << "[7m";
  }


  /**
   * Set the video to its normal mode where foreground is foreground color
   * and background is background color at the current cursor position.
   */
  virtual void render_normal_video(stringstream* out) {
    *out << dec << (char)27 << "[0m";
  }


  /**
   * Clear the screen and move the cursor to the upper left corner.
   */
  virtual void clear_screen(stringstream* out) {
    *out << dec << (char)27 << "[2J";
    move_cursor_to_row(out, 0);
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
  virtual void move_cursor_to_row(stringstream* out, int row) {
    *out << dec << (char)27 << "[" << row + 1 << ";0H";
  }


  /**
   * Move the cursor to the specified column.
   *
   * @param column Column to move the cursor to.
   */
  virtual void move_cursor_to_column(stringstream* out, int column) {
    *out << dec << (char)27 << "[" << column << "G" << flush;
  }


  /**
   * Render a pattern row with the current index at the current cursor
   * position.
   *
   * @param row_index The index of the row within the pattern to render.
   */
  virtual void render_row(unsigned int row_index) {
    stringstream out;
    render_row(&out, row_index);
    cout << out.str() << flush;
  }

  virtual void render_row(stringstream* out, unsigned int row_index) {

    if (row_index == this->row_index) {
      render_reverse_video(out);
    }

    render_row_number(out, row_index);

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
        render_note(out, note->get_key(), note->get_velocity());
      }
      EFFECTS* effects = dynamic_cast<EFFECTS*>(track_entry->get_effects());
      for (unsigned int i = 0; i < effects->size(); i++) {
        EFFECT* effect = dynamic_cast<EFFECT*>(effects->at(i));
        render_effect(out, effect->get_command(), effect->get_value());
      }
    }

    if (row_index == this->row_index) {
      render_normal_video(out);
    }
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
   * @param row_index The row to use as selected row when calculating the
   *                  scrolling offset.
   *
   * @return The offset at which the pattern should start to be rendered from.
   */
  virtual unsigned int calculate_pattern_render_offset(int row_index) {
    int row = static_cast<int>(row_index);
    int row_count = static_cast<int>(pattern_length);
    int screen_height = static_cast<int>(get_screen_height()) - HEADING_LINES;
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
  virtual void render_pattern(stringstream* out) {
    if (true == block_render) {
      return;
    }
    unsigned int offset = calculate_pattern_render_offset(this->row_index);
    unsigned int rows_to_print = min(pattern_length,
                                     get_screen_height() - HEADING_LINES + offset);
    for (unsigned int i = offset; i < rows_to_print; i++) {
      render_row(out, i);
      if (i < rows_to_print - 1) {
        *out << endl;
      }
    }
  }


  /**
   * Render the heading of the pattern editor, containing the pattern number,
   * name and the track headings.
   */
  virtual void render_heading(stringstream* out) {
    move_cursor_to_row(out, 0);
    move_cursor_to_column(out, 0);
    render_reverse_video(out);
    string pattern_name = "New pattern";
    stringstream title_row;

    /* Render title row. */
    title_row << " AiOSeq Pattern Editor - ";
    title_row << setfill('0') << setw(2) << hex << pattern_index;
    title_row << " ";
    title_row << pattern_name;
    title_row << " ";

    *out << title_row.str();
    *out << setfill(' ') << setw(get_screen_width() - title_row.str().size());
    *out << "" << endl;

    /* Render track headings. */
    render_normal_video(out);
    *out << "   ";

    TRACKS* tracks = dynamic_cast<TRACKS*>(sequencer->get_tracks());

    for (unsigned int i = 0; i < tracks->size(); i++) {
      TRACK* track = dynamic_cast<TRACK*>(tracks->at(i));
      unsigned int notes = track->get_notes();
      unsigned int effects = track->get_effects();
      int columns = notes * 6 + notes + effects * 4 + effects;
      string track_name = "New track";
      /*
      if (track_index == i) {
        render_reverse_video(out);
      }
      */
      *out << setfill('0') << setw(2) << hex << i;
      *out << " ";

      int track_width = columns - 3 - 1;

      track_name.resize(track_width, ' ');

      *out << track_name;
      /*
      if (track_index == i) {
        render_normal_video(out);
      }
      */
      *out << " ";
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

    stringstream out;

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
        move_cursor_to_column(&out, get_console_column(column));
      }
    }
    else if ('D' == c) {
      if (0 < column) {
        column--;
        move_cursor_to_column(&out, get_console_column(column));
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

    cout << out.str() << flush;

    return retval;
  }


  /**
   * Set a nibble and re-rerender it.
   *
   * @param value       The value to set (0-15)
   * @param high_nibble True = Hich nibble, False = Low nibble.
   */
  virtual void set_nibble(int value, bool high_nibble) {
    stringstream out;

    move_cursor_to_column(&out, get_console_column(column));
    render_reverse_video(&out);
    render_nibble(&out, 0, value);
    move_cursor_to_column(&out, get_console_column(column));
    render_normal_video(&out);
    column++;
    move_cursor_to_column(&out, get_console_column(column));
    cout << out.str() << flush;
  }


public:

  /**
   * Create a new instance of a pattern editor class with the positbility to
   * render/update and edit a pattern.
   *
   * This constructor set-up a keyboard mapping for using a computer keyboard
   * as a musical keyboard.
   *
   * @param sequencer A reference to the sequencer master to use.
   *
   */
  PatternEditorTemplate(SequencerInterface* sequencer) : sequencer(sequencer), row_index(0), track_index(0), pattern_index(0), pattern_length(0), display_mode(POLYPHONY), block_render(false), edit_mode(true), column(0), columns(0) {
    for (int i = 0; i < 256; i++) {
      scancode_to_note_key[i] = 0xffff;
      scancode_to_hex_value[i] = 0xffff;
    }

    /**
     * Set-up a scancode to note mapping table.
     *
       @verbatim
       .---.---.---.---.---.---.---.---.---.---.---.---.---.-------.
       |   |   |C#y|D#y|   |F#y|G#y|H#y|   |C#z|D#z|   |F#z|       |
       +---'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-----+
       |     |C-y|D-y|E-y|F-y|G-y|H-y|A-y|C-z|D-z|E-z|F-z|G-z|     |
       +-----'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.---.-'-.-'-.  ||
       |       |C#x|D#x|   |F#x|G#x|H#x|   |C#y|D#y|   |F#y|   |<-'|
       +----.--'.--'.--'.--'.--'.--'.--'.--'.--'.--'.--'.--'---'---+
       | ^  |C-x|D-x|E-x|F-x|G-x|H-x|A-x|C-y|D-y|E-y|F-y|        ^ |
       '----+---'.--'-.-'---'---'---'---'---'--.'---+---'.----.----+
       |    |    |    |                        |    |    |    |    |
       '----'----'----'------------------------'----'----'----'----'
       @endverbatim
    */
    scancode_to_note_key['<'] = 1;
    scancode_to_note_key['a'] = 2;
    scancode_to_note_key['z'] = 3;
    scancode_to_note_key['s'] = 4;
    scancode_to_note_key['x'] = 5;
    scancode_to_note_key['c'] = 6;
    scancode_to_note_key['f'] = 7;
    scancode_to_note_key['v'] = 8;
    scancode_to_note_key['g'] = 9;
    scancode_to_note_key['b'] = 10;
    scancode_to_note_key['h'] = 11;
    scancode_to_note_key['n'] = 12;
    scancode_to_note_key['m'] = 13;
    scancode_to_note_key['k'] = 14;
    scancode_to_note_key[','] = 15;
    scancode_to_note_key['l'] = 16;
    scancode_to_note_key['.'] = 17;
    scancode_to_note_key['-'] = 18;

    scancode_to_note_key['q'] = 13;
    scancode_to_note_key['2'] = 14;
    scancode_to_note_key['w'] = 15;
    scancode_to_note_key['3'] = 16;
    scancode_to_note_key['e'] = 17;
    scancode_to_note_key['r'] = 18;
    scancode_to_note_key['5'] = 19;
    scancode_to_note_key['t'] = 20;
    scancode_to_note_key['6'] = 21;
    scancode_to_note_key['y'] = 22;
    scancode_to_note_key['7'] = 23;
    scancode_to_note_key['u'] = 24;
    scancode_to_note_key['i'] = 25;
    scancode_to_note_key['9'] = 26;
    scancode_to_note_key['o'] = 27;
    scancode_to_note_key['0'] = 28;
    scancode_to_note_key['p'] = 29;
    scancode_to_note_key['+'] = 30;


    /**
     * Set-up a scancode to hexadecimal nibble mapping table.
     */


    scancode_to_hex_value['0'] = 0x0;
    scancode_to_hex_value['1'] = 0x1;
    scancode_to_hex_value['2'] = 0x2;
    scancode_to_hex_value['3'] = 0x3;
    scancode_to_hex_value['4'] = 0x4;
    scancode_to_hex_value['5'] = 0x5;
    scancode_to_hex_value['6'] = 0x6;
    scancode_to_hex_value['7'] = 0x7;
    scancode_to_hex_value['8'] = 0x8;
    scancode_to_hex_value['9'] = 0x9;
    scancode_to_hex_value['a'] = 0xa;
    scancode_to_hex_value['b'] = 0xb;
    scancode_to_hex_value['c'] = 0xc;
    scancode_to_hex_value['d'] = 0xd;
    scancode_to_hex_value['e'] = 0xe;
    scancode_to_hex_value['f'] = 0xf;

  }
  ~PatternEditorTemplate() {}

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(unsigned int)
  void set_track_index(unsigned int track_index) {
    if (this->track_index != track_index) {
      stringstream out;
      this->track_index = track_index;
      move_cursor_to_row(&out, HEADING_LINES);
      move_cursor_to_column(&out, 0);
      render_pattern(&out);
      render_heading(&out);
      cout << out.str();
      set_pattern_row_index(row_index);
    }
  }

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(unsigned int)
  void set_pattern_index(unsigned int pattern_index) {
    if (pattern_index != this->pattern_index) {
      stringstream out;
      this->pattern_index = pattern_index;
      update_cursor_columns();
      render_pattern(&out);
      render_heading(&out);
      cout << out.str();
    }
  }

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  void set_pattern_row_index(int pattern_row_index) {
    stringstream out;
    int row = this->row_index;
    int prev_offset = calculate_pattern_render_offset(row);
    this->row_index = static_cast<unsigned int>(pattern_row_index);

    int curr_offset = calculate_pattern_render_offset(this->row_index);

    int screen_height = get_screen_height() - HEADING_LINES;

    if ((curr_offset < prev_offset) ||
        ((row == 0) && (pattern_row_index == pattern_length - 1))) {
      /*
       * Wrapping from last row to first row should end up in re-rendering
       * the whole pattern.
       */
      move_cursor_to_row(&out, HEADING_LINES);
      render_pattern(&out);
      render_heading(&out);
      move_cursor_to_row(&out, this->row_index - curr_offset + HEADING_LINES);
      move_cursor_to_column(&out, get_console_column(this->column));
    } else if (curr_offset > prev_offset) {
      /*
       * If the offset has increased the pattern shall scroll down.
       */
      move_cursor_to_row(&out, screen_height + HEADING_LINES - 1);
      out << endl;
      render_row(&out, screen_height + curr_offset - 1);
      render_heading(&out);

      // Re-render the previously selected row.
      move_cursor_to_row(&out, row - prev_offset + HEADING_LINES - 1);
      render_row(&out, row);

      // And render the selected row.
      move_cursor_to_row(&out, this->row_index - curr_offset + HEADING_LINES);
      render_row(&out, this->row_index);
      move_cursor_to_column(&out, get_console_column(this->column));
    } else {
      /*
       * Move the selected line.
       */
      // Re-render the previously selected row.
      move_cursor_to_row(&out, row - prev_offset + HEADING_LINES);
      render_row(&out, row);

      // And render the selected row.
      move_cursor_to_row(&out, this->row_index - curr_offset + HEADING_LINES);
      render_row(&out, this->row_index);
      move_cursor_to_column(&out, get_console_column(this->column));
    }
    cout << out.str() << flush;
  }


  /// @copydoc PatternClientInterface::set_pattern_length(unsigned int)
  virtual void set_pattern_length(unsigned int pattern_length) {
    /**
     * @todo Only render if the newly added bit of lenght is currently fitting
     *       in the display terminal. Otherwize don't re-render.
     */
    stringstream out;
    this->pattern_length = pattern_length;
    render_pattern(&out);
    render_heading(&out);
  }


  /// @copydoc PatternClientInterface::set_key(unsigned int, unsigned int, int)
  virtual void set_key(unsigned int track_index, unsigned int note_index, int key) {
    stringstream out;
    render_reverse_video(&out);
    render_key(&out, key);
    move_cursor_to_column(&out, get_console_column(this->column));
    render_normal_video(&out);

    cout << out.str();

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

    stringstream out;

    column = 0;
    block_render = true;
    sequencer->register_client(dynamic_cast<ClientPrimitiveInterface*>(this));
    block_render = false;
    clear_screen(&out);
    update_cursor_columns();
    move_cursor_to_row(&out, HEADING_LINES);
    render_pattern(&out);
    render_heading(&out);
    move_cursor_to_row(&out, HEADING_LINES);
    move_cursor_to_column(&out, get_console_column(this->column));

    cout << out.str() << flush;

    while (true == main_loop())
      ;
    sequencer->unregister_client(dynamic_cast<ClientPrimitiveInterface*>(this));

    stringstream out_late;

    clear_screen(&out_late);
    clear_cursor_columns();

    cout << out_late.str() << flush;

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
