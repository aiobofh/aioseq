/**
 * @file pattern_editor_test.cc
 */

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sstream>
#include <thread>
#include "test.hh"

using ::testing::NiceMock;

/**
 * Make all relevant test cases friends with the PatternEditor class to be
 * able to inspect the protected and private variables.
 */
#define PATTERN_EDITOR_FRIENDS \
  friend_test(PatternEditor, Constructor) \
  friend_test(PatternEditor, Setters_and_getters) \
  friend_test(PatternEditor, Render_key) \
  friend_test(PatternEditor, Render_row_number) \
  friend_test(PatternEditor, Render_nibble) \
  friend_test(PatternEditor, Render_velocity) \
  friend_test(PatternEditor, Render_note) \
  friend_test(PatternEditor, Render_row) \
  friend_test(PatternEditor, Render_reverse_video) \
  friend_test(PatternEditor, Render_normal_video) \
  friend_test(PatternEditor, Get_screen_height) \
  friend_test(PatternEditor, Get_screen_width) \
  friend_test(PatternEditor, Clear_screen) \
  friend_test(PatternEditor, Move_cursor_to_row) \
  friend_test(PatternEditor, Calculate_pattern_render_offset) \
  friend_test(PatternEditor, Render_pattern_visible_rows) \
  friend_test(PatternEditor, Main_will_render_a_pattern_call_the_main_loop_and_exit) \
  friend_test(PatternEditor, Main_loop_read_keyboard_and_return_true) \
  friend_test(PatternEditor, Getting_a_key_shall_get_a_key) \
  friend_test(PatternEditor, Key_down_will_select_next_pattern_row) \
  friend_test(PatternEditor, Key_up_will_select_previous_pattern_row) \
  friend_test(PatternEditor, Q_will_quit_main_loop)


#include "pattern_editor.hh"
#include "mock_pattern_editor.hh"

#include "mock_sequencer.hh"

/**
 * @test PatternEditor - Constructor
 *
 * Make sure that the constructor passes the sequencer reference to internal
 * storage untouched and set-up default pattern editor.
 */
test_case(PatternEditor, Constructor) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  assert_eq(&sequencer, pattern_editor.sequencer);
  assert_eq(static_cast<unsigned int>(0), pattern_editor.row_index);
  assert_eq(static_cast<unsigned int>(0), pattern_editor.track_index);
}


/**
 * @test PatternEditor - Getters
 *
 * Make sure that all getters are getting the correct internal content.
 */
test_case(PatternEditor, Setters_and_getters) {
  NiceMock<MockPatternRow> pattern_row;
  NiceMock<MockSequencer> sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);
  /*
  // Defailt (0 will be re-rendered in normal video when index 2 is rendered)
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(0))), 1);
  // Row index 2 (2 will be re-rendered in normal if index 5 is rendered)
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(2))), 2);
  // Row index 5
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(5))), 1);
  // Pattern length 2 and 5
  expect_call_times(pattern_editor, render_pattern(), 6);

  // Expect the console cursor to be moved around.
  expect_call_times(pattern_editor, move_cursor_to_row(Eq(1)), 1);
  expect_call_times(pattern_editor, move_cursor_to_row(Eq(3)), 2);
  expect_call_times(pattern_editor, move_cursor_to_row(Eq(6)), 1);
  */
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(), 4, 0);
  expect_call_times_will_return(pattern_editor,
                                get_screen_height(), 2, 24);

  pattern_editor.PatternEditor::set_pattern_row_index(2);
  assert_eq(static_cast<unsigned int>(2), pattern_editor.row_index);
  pattern_editor.PatternEditor::set_pattern_row_index(5);
  assert_eq(static_cast<unsigned int>(5), pattern_editor.row_index);

  pattern_editor.PatternEditor::set_pattern_length(2);
  assert_eq(static_cast<unsigned int>(2), pattern_editor.pattern_length);
  pattern_editor.PatternEditor::set_pattern_length(5);
  assert_eq(static_cast<unsigned int>(5), pattern_editor.pattern_length);

  pattern_editor.PatternEditor::set_track_index(2);
  assert_eq(static_cast<unsigned int>(2), pattern_editor.track_index);
  pattern_editor.PatternEditor::set_track_index(5);
  assert_eq(static_cast<unsigned int>(5), pattern_editor.track_index);

  pattern_editor.PatternEditor::set_pattern_index(2);
  assert_eq(static_cast<unsigned int>(2), pattern_editor.pattern_index);
  pattern_editor.PatternEditor::set_pattern_index(5);
  assert_eq(static_cast<unsigned int>(5), pattern_editor.pattern_index);
}


/**
 * @test PatternEditor - Render key
 *
 * Make sure that a key is rendered correctly to the standard output.
 */
test_case(PatternEditor, Render_key) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  unsigned int k = 0;

  static string note[12] =
    {"C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "H-", "H#", "A-"};

  assert_stdout_eq("--- ", pattern_editor.render_key(k++));

  for (unsigned int o = 1; o < 11; o++) {
    for (unsigned int n = 0; n < 12; n++) {
      stringstream expect;
      expect << note[n] << hex << o << " ";
      assert_stdout_eq(expect.str(), pattern_editor.render_key(k++));
      if (127 == k) {
        break;
      }
    }
  }
}


/**
 * @test PatternEditor - Render row number
 *
 * Make sure that the row number of the pattern is outputed in hex correctly
 * to the standard output.
 */
test_case(PatternEditor, Render_row_number) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  assert_stdout_eq("00 ", pattern_editor.render_row_number(0));
  assert_stdout_eq("0f ", pattern_editor.render_row_number(15));
  assert_stdout_eq("ff ", pattern_editor.render_row_number(255));
}


/**
 * @test PatternEditor - Render nibble
 *
 * Make sure that a nibble (4 bits) of a byte is outputed in hex correctly
 * to the standard output.
 */
test_case(PatternEditor, Render_nibble) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  assert_stdout_eq("2", pattern_editor.render_nibble(0, 0x12));
  assert_stdout_eq("1", pattern_editor.render_nibble(1, 0x12));
}


/**
 * @test PatternEditor - Render note
 *
 * Make sure that a complete note (key and velocity) is outputed correctly to
 * the standard output.
 */
test_case(PatternEditor, Render_note) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  assert_stdout_eq("--- 00 ", pattern_editor.render_note(0, 0));
  assert_stdout_eq("C-1 01 ", pattern_editor.render_note(1, 1));
}


/**
 * @test PatternEditor - Render reverse video
 *
 * Make sure that the correct ANSI escape code is outputed to the standard
 * output.
 */
test_case(PatternEditor, Render_reverse_video) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  assert_stdout_eq("\x1B[7m", pattern_editor.render_reverse_video());
}


/**
 * @test PatternEditor - Render normal video
 *
 * Make sure that the correct ANSI escape code is outputed to the standard
 * output.
 */
test_case(PatternEditor, Render_normal_video) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  assert_stdout_eq("\x1B[0m", pattern_editor.render_normal_video());
}


/**
 * @test PatternEditor - Get screen height
 *
 * Make sure that the correct screen height is picked up from the current
 * terminal height.
 */
test_case(PatternEditor, Get_screen_height) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  unsigned int lines = w.ws_row;

  assert_eq(lines, pattern_editor.get_screen_height());
}


/**
 * @test PatternEditor - Get screen width
 *
 * Make sure that the correct screen width is pciked up from the current
 * terminal width.
 */
test_case(PatternEditor, Get_screen_width) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  unsigned int columns = w.ws_col;

  assert_eq(columns, pattern_editor.get_screen_width());
}


/**
 * @test PatternEditor - Clear screen
 *
 * Make sure that the screen is cleared and the cursors is moved to the upper
 * left corner.
 */
test_case(PatternEditor, Clear_screen) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  expect_call_times(pattern_editor, move_cursor_to_row(0), 1);

  assert_stdout_eq("\x1B[2J", pattern_editor.PatternEditor::clear_screen());
}


/**
 * @test PatternEditor - Move cursor to row
 *
 * Make sure that the cursor is moved to the correct row using escape codes.
 */
test_case(PatternEditor, Move_cursor_to_row) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  assert_stdout_eq("\x1B[2;0H", pattern_editor.move_cursor_to_row(2));
}


/**
 * @test PatternEditor - Render row
 *
 * Make sure that a correctly call chain is performed to output a row of a
 * pattern.
 */
test_case(PatternEditor, Render_row) {
  MockEffect effect;
  MockEffects effects;
  MockNote note;
  MockNotes notes;
  MockTrackEntry track_entry;
  MockTrackEntries track_entries;
  MockPatternRow pattern_row;
  NiceMock<MockSequencer> sequencer;
  NiceMock<FakePatternEditorMock> pattern_editor(&sequencer);

  track_entries.push_back(&track_entry);
  notes.push_back(&note);
  notes.push_back(&note);
  notes.push_back(&note);

  effects.push_back(&effect);
  effects.push_back(&effect);
  effects.push_back(&effect);

  expect_call_times(pattern_editor, render_row_number(_), 2);
  expect_call_times_will_return(sequencer, get_row(_), 2, &pattern_row);
  expect_call_times_will_return(pattern_row, get_track_entries(), 2,
                                &track_entries);
  expect_call_times_will_return(track_entry, get_notes(), 2, &notes);
  expect_call_times_will_return(track_entry, get_effects(), 2, &effects);
  expect_call_times_will_return(note, get_key(), 6, 0);
  expect_call_times_will_return(note, get_velocity(), 6, 0);
  expect_call_times_will_return(effect, get_command(), 6, 0);
  expect_call_times_will_return(effect, get_value(), 6, 0);
  expect_call_times(pattern_editor,
                    render_note(Eq(static_cast<unsigned int>(0)),
                                Eq(static_cast<unsigned int>(0))), 6);

  // Make another row selected
  pattern_editor.PatternEditorTemplate::row_index = 1;

  pattern_editor.PatternEditorTemplate::render_row(0);

  /*
   * Since one of the rows must be selected.
   */
  expect_call_times(pattern_editor, render_reverse_video(), 1);
  expect_call_times(pattern_editor, render_normal_video(), 1);

  // Make the row to be rendered selected
  pattern_editor.PatternEditorTemplate::row_index = 0;

  pattern_editor.PatternEditorTemplate::render_row(0);

}


/**
 * @test PatternEdigor - Calculate pattern render offset
 *
 * Make sure that the render-offset of a pattern is correct by inspecting the
 * current console height and pattern length, producing a scrolling effect
 * if it does not fit.
 */
test_case(PatternEditor, Calculate_pattern_render_offset) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  int lines = w.ws_row;
  int rows = lines - 2;

  /*
   * Mock the system to beleive that the pattern is shorter than the number
   * of lines available on the screen.
   */
  pattern_editor.pattern_length = rows;
  //  expect_call_times_will_return(sequencer, get_row_count(), rows, rows);

  /*
   * A pattern fitting on a terminal should never offset, emulate scolling
   * through it.
   */
  for (int i = 0; i < rows; i++) {
    pattern_editor.row_index = i;
    assert_eq(static_cast<unsigned int>(0),
              pattern_editor.calculate_pattern_render_offset());
  }

  rows = lines + 3;

  /*
   * Mock the system to beleive that the pattern is longer than the number
   * of lines available on the screen.
   */
  pattern_editor.pattern_length = rows;

  /*
   * A pattern fitting on a terminal should offset two lines, emulate scolling
   * through it and reach the end of the pattern.
   */
  for (int i = 0; i < rows; i++) {

    pattern_editor.row_index = i;
    int offset = pattern_editor.calculate_pattern_render_offset();

    if (i <= (lines / 2)) {
      // Selected row has not reached half screen yet.
      assert_eq(static_cast<int>(0), offset);
    }
    else if ((i - lines) <= (lines / 2) && (i < (rows - lines / 2))) {
      // Selected row has reached half screen but last row is not yet printed.
      assert_eq(static_cast<int>(i - (lines / 2)), offset);
    }
    else {
      // Last line is printed, no need to dynamically offset anymore.
      assert_eq(static_cast<int>(rows - lines), offset);
    }
  }
}


/**
 * @test PatternEditor - Render pattern visible rows
 *
 * Make sure that the pattern has the correct call-chain for rendering all
 * visible rows.
 */
test_case(PatternEditor, Render_pattern_visible_rows) {
  MockPattern pattern;
  MockPatternRow pattern_row;
  MockPatternRows pattern_rows;
  MockSequencer sequencer;

  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  FakePatternEditor pattern_editor(&sequencer);

  /*
   * Rendering a pattern of four rows on a screen of 5 rows shall render
   * four rows.
   */
  pattern_editor.pattern_length = pattern_rows.size();
  //  expect_call_times_will_return(sequencer, get_row_count(), 1, pattern_rows.size());
  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 5);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(), 1, 0);

  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(0))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(1))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(3))), 1);

  /*
   * Call the design under testing.
   */
  assert_stdout_eq("\n\n\n",pattern_editor.PatternEditor::render_pattern());

  /*
   * Adding a couple of rows to the pattern will result in rendering 5
   * (screen height) rows.
   */

  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  pattern_editor.pattern_length = pattern_rows.size();
  //  expect_call_times_will_return(sequencer, get_row_count(), 1, pattern_rows.size());
  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 5);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(), 1, 0);


  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(0))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(1))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(3))), 1);
  expect_call_times(pattern_editor
                    ,render_row(Eq(static_cast<unsigned int>(4))), 1);

  /*
   * Call the design under testing.
   */
  assert_stdout_eq("\n\n\n\n", pattern_editor.PatternEditor::render_pattern());

  /*
   * Adding a couple of more rows to the pattern will result in rendering 5
   * (screen height) rows but with the specified offset
   */

  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  pattern_editor.pattern_length = pattern_rows.size();
  //  expect_call_times_will_return(sequencer, get_row_count(), 1, pattern_rows.size());
  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 5);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(), 1, 2);


  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(3))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(4))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(5))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(static_cast<unsigned int>(6))), 1);

  /*
   * Call the design under testing.
   */
  assert_stdout_eq("\n\n\n\n", pattern_editor.PatternEditor::render_pattern());
}


void keyboard_sim(char c) {
  cout << "Keyboard sim started" << endl;
  sleep(1);
  std::cin.putback(c);
  std::cin.putback('\n');
  cout << "Keyboard sim ended" << endl;
}

/**
 * @test PatternEditor - Getting a key shall get a key.
 *
 * Make sure that keyboard (cin) is read and interpreted correctly.
 */
test_case(PatternEditor, Getting_a_key_shall_get_a_key) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  //  thread ksim (keyboard_sim,'g');
  // ksim.detach();

  // cout << "Calling DUT" << endl;
  // assert_eq('g', pattern_editor.getch());
  //cout << "Done" << endl;

  //sleep(2);
}


/**
 * @test PatternEditor - Main loop read keyboard and return true.
 *
 * This is onlye a code-coverage test.
 */
test_case(PatternEditor, Main_loop_read_keyboard_and_return_true) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;

  expect_call_times_will_return(pattern_editor, getch(), 1, 'Å');

  assert_eq(true, pattern_editor.PatternEditor::main_loop());
}


/**
 * @test PatternEditor - Key down will select next pattern row.
 *
 * Make sure that the keyboard input of the down-arrow has the expected
 * effect.
 */
test_case(PatternEditor, Key_down_will_select_next_pattern_row) {
  const int ROWS = 1;
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;
  pattern_editor.PatternEditor::row_index = ROWS;

  expect_call_times(sequencer, set_pattern_row_index(Eq(ROWS + 1)), 1);
  expect_call_times_will_return(pattern_editor, getch(), 1, 'B');

  assert_eq(true, pattern_editor.PatternEditor::main_loop());
}

/**
 * @test PatternEditor - Key up will select previous pattern row.
 *
 * Make sure that the keyboard input of the down-arrow has the expected
 * effect.
 */
test_case(PatternEditor, Key_up_will_select_previous_pattern_row) {
  const unsigned int ROWS = 1;
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;
  pattern_editor.PatternEditor::row_index = ROWS;

  expect_call_times(sequencer, set_pattern_row_index(Eq(ROWS - 1)), 1);
  expect_call_times_will_return(pattern_editor, getch(), 1, 'A');

  assert_eq(true, pattern_editor.PatternEditor::main_loop());
}


/**
 * @test PatternEditor - Key up will select previous pattern row.
 *
 * Make sure that the keyboard input of the down-arrow has the expected
 * effect.
 */
test_case(PatternEditor, Q_will_quit_main_loop) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;
  expect_call_times_will_return(pattern_editor, getch(), 1, 'Q');

  assert_false(pattern_editor.PatternEditor::main_loop());
}

/**
 * @test PatternEditor - Main will render a pattern, call the main lopp and
 *                       exit.
 * Make sure that the main function handles arguments and parameters
 * correctly and that the main loop is called.
 *
 * @todo Implementation missing.
 */
test_case(PatternEditor, Main_will_render_a_pattern_call_the_main_loop_and_exit) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  FakePatternEditor* pe_ptr = &pattern_editor;

  PatternEditorInterface* pattern_editor_ptr =
    dynamic_cast<PatternEditorInterface*>(pe_ptr);

  ClientPrimitiveInterface* client =
    dynamic_cast<ClientPrimitiveInterface*>(pattern_editor_ptr);

  expect_call_times(sequencer, register_client(Eq(client)), 1);
  expect_call_times(sequencer, unregister_client(Eq(client)), 1);

  expect_call_times_will_return(pattern_editor, main_loop(), 1, false);

  assert_eq(static_cast<int>(0), pattern_editor.PatternEditor::main(0, NULL));
}
