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

using namespace std;

#include "sequencer.hh"
#include "track_client.hh"
#include "pattern_client.hh"
#include "part_client.hh"

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
         class TRACK_ENTRIES,
         class TRACK_ENTRY,
         class NOTES,
         class NOTE>
class PatternEditorTemplate {

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
  /// Internal storage of the current pattern length.
  unsigned int pattern_length;

  /// Internal storage of the current display mode.
  enum DisplayMode display_mode;

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
   * Render a pattern row with the current index at the current cursor
   * position.
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
    }

    if (row_index == this->row_index) {
      render_normal_video();
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
   * @return The offset at which the pattern should start to be rendered from.
   */
  virtual unsigned int calculate_pattern_render_offset() {
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
        if (((row - screen_height) <= half_screen) &&
            (row <= (screen_height - half_screen))) {
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
    unsigned int offset = calculate_pattern_render_offset();
    unsigned int rows_to_print = min(pattern_length,
                                     get_screen_height() + offset);
    for (unsigned int i = offset; i < rows_to_print; i++) {
      render_row(i);
    }
  }

  /**
   * Program main-loop.
   */
  virtual bool main_loop() {
    cout << "DEBUG: Geranemoooooooo!" << endl;
    return true;
  }

public:

  /**
   * Create a new instance of a pattern editor class with the positbility to
   * render/update and edit a pattern.
   *
   * @param sequencer A reference to the sequencer master to use.
   */
  PatternEditorTemplate(SequencerInterface* sequencer)
    : sequencer(sequencer),
    row_index(0),
    track_index(0),
    display_mode(POLYPHONY) {}
  virtual ~PatternEditorTemplate() {}

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(int)
  void set_track_index(unsigned int track_index) {
    this->track_index = track_index;
  }

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(int)
  void set_pattern_index(unsigned int pattern_index) {
    this->pattern_index = pattern_index;
    render_pattern();
  }

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  void set_pattern_row_index(unsigned int row_index) {
    render_row(this->row_index);
    this->row_index = row_index;
    render_row(this->row_index);
  }

  /// @copydoc PatternClientInterface::set_pattern_length(int)
  virtual void set_pattern_length(int pattern_length) {
    this->pattern_length = pattern_length;
    render_pattern();
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
    sequencer->register_client(dynamic_cast<ClientPrimitiveInterface*>(this));
    while (main_loop())
      ;
    sequencer->unregister_client(dynamic_cast<ClientPrimitiveInterface*>(this));
    return 0;
  }

};


/**
 * PatternEditor is a specialized class of the template PatternEditorTemplate
 * for performance and reuse.
 */
typedef PatternEditorTemplate<Sequencer,
                              PatternRow,
                              TrackEntries,
                              TrackEntry,
                              Notes,
                              Note> PatternEditor;

#endif
