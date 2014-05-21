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
  friend_test(PatternEditor, Set_pattern_row_index_shall_select_correct_rendering_method) \
  friend_test(PatternEditor, Render_key) \
  friend_test(PatternEditor, Render_row_number) \
  friend_test(PatternEditor, Render_nibble) \
  friend_test(PatternEditor, Render_velocity) \
  friend_test(PatternEditor, Render_note) \
  friend_test(PatternEditor, Render_effect) \
  friend_test(PatternEditor, Render_row) \
  friend_test(PatternEditor, Render_reverse_video) \
  friend_test(PatternEditor, Render_normal_video) \
  friend_test(PatternEditor, Get_screen_height) \
  friend_test(PatternEditor, Get_screen_width) \
  friend_test(PatternEditor, Clear_screen) \
  friend_test(PatternEditor, Clear_cursor_columns) \
  friend_test(PatternEditor, Add_note_columns) \
  friend_test(PatternEditor, Add_effect_columns) \
  friend_test(PatternEditor, Update_cursor_columns) \
  friend_test(PatternEditor, Get_console_column) \
  friend_test(PatternEditor, Move_cursor_to_row) \
  friend_test(PatternEditor, Move_cursor_to_column) \
  friend_test(PatternEditor, Calculate_pattern_render_offset) \
  friend_test(PatternEditor, Render_pattern_visible_rows) \
  friend_test(PatternEditor, Main_will_render_a_pattern_call_the_main_loop_and_exit) \
  friend_test(PatternEditor, Main_loop_read_keyboard_and_return_true) \
  friend_test(PatternEditor, Getting_a_key_shall_get_a_key) \
  friend_test(PatternEditor, Key_down_will_select_next_pattern_row) \
  friend_test(PatternEditor, Key_up_will_select_previous_pattern_row) \
  friend_test(PatternEditor, Key_left_will_select_previous_pattern_column) \
  friend_test(PatternEditor, Key_right_will_select_next_pattern_column) \
  friend_test(PatternEditor, Q_will_quit_main_loop) \
  friend_test(PatternEditor, Keyboard_will_send_notes_to_sequencer) \
  friend_test(PatternEditor, Keyboard_will_send_velocity_nibbles_to_the_sequencer) \
  friend_test(PatternEditor, Keyboard_will_send_command_nibbles_to_the_sequencer) \
  friend_test(PatternEditor, Keyboard_will_send_value_nibbles_to_the_sequencer) \
  friend_test(PatternEditor, Set_nibble) \
  friend_test(PatternEditor, Set_pattern_length) \
  friend_test(PatternEditor, Set_key) \
  friend_test(PatternEditor, Set_velocity_rely_on_set_nibble) \
  friend_test(PatternEditor, Set_command_rely_on_set_nibble) \
  friend_test(PatternEditor, Set_value_rely_on_set_nibble)


#include "pattern_editor.hh"
#include "mock_pattern_editor.hh"

#include "mock_sequencer.hh"
#include "mock_track.hh"

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
  assert_eq(static_cast<unsigned int>(0), pattern_editor.pattern_index);
  assert_eq(0, pattern_editor.column);
  assert_eq(0, pattern_editor.columns);
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

  /**
   * @todo Number of calls are irrelevant, make a better macro.
   */
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(_), 8, 0);
  expect_call_times_will_return(pattern_editor,
                                get_screen_height(), 4, 24);

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
 * @test PatternEditor - Set pattern row index shall select correct
 *                       rendering method.
 */
test_case(PatternEditor, Set_pattern_row_index_shall_select_correct_rendering_method) {
  NiceMock<MockPatternRow> pattern_row;
  NiceMock<MockSequencer> sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  /**
   * Redraw the whole pattern when on last row, wrapping over to first row.
   */
  expect_call_times(pattern_editor, render_pattern(_), 1);
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(Eq(20)),
                                1,
                                10);
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(Eq(0)),
                                1,
                                0);
  pattern_editor.PatternEditor::row_index = 20;
  pattern_editor.PatternEditor::set_pattern_row_index(0);

  /**
   * Make sure that scrolling down works.
   */
  expect_call_times(pattern_editor, render_row(_,_), 3);
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(Eq(0)),
                                1,
                                0);
  expect_call_times_will_return(pattern_editor,
                                calculate_pattern_render_offset(Eq(1)),
                                1,
                                1);
  pattern_editor.PatternEditor::row_index = 0;
  pattern_editor.PatternEditor::set_pattern_row_index(1);
}

/**
 * @test PatternEditor - Render key
 *
 * Make sure that a key is rendered correctly to the standard output.
 */
test_case(PatternEditor, Render_key) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  unsigned int k = 0;

  static string note[12] =
    {"C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "H-", "H#", "A-"};

  pattern_editor.render_key(&out, k++);
  assert_eq("--- ", out.str());

  for (unsigned int o = 1; o < 11; o++) {
    for (unsigned int n = 0; n < 12; n++) {
      stringstream real;
      stringstream expect;
      expect << note[n] << hex << o << " ";
      pattern_editor.render_key(&real, k++);
      assert_eq(expect.str(), real.str());
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
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  pattern_editor.render_row_number(&out, 0);
  assert_eq("00 ", out.str());
  out.str("");
  pattern_editor.render_row_number(&out, 15);
  assert_eq("0f ", out.str());
  out.str("");
  pattern_editor.render_row_number(&out, 255);
  assert_eq("ff ", out.str());
  out.str("");
}


/**
 * @test PatternEditor - Render nibble
 *
 * Make sure that a nibble (4 bits) of a byte is outputed in hex correctly
 * to the standard output.
 */
test_case(PatternEditor, Render_nibble) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  pattern_editor.render_nibble(&out, 0, 0x12);
  assert_eq("2", out.str());
  out.str("");
  pattern_editor.render_nibble(&out, 1, 0x12);
  assert_eq("1", out.str());
}


/**
 * @test PatternEditor - Render note
 *
 * Make sure that a complete note (key and velocity) is outputed correctly to
 * the standard output.
 */
test_case(PatternEditor, Render_note) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  pattern_editor.render_note(&out, 0, 0);
  assert_eq("--- 00 ", out.str());
  out.str("");
  pattern_editor.render_note(&out, 1, 1);
  assert_eq("C-1 01 ", out.str());
  out.str("");
  pattern_editor.render_note(&out, 2, 16);
  assert_eq("C#1 10 ", out.str());
}


/**
 * @test PatternEditor - Render effect
 *
 * Make sure that a complete effect (command and value) is outputed
 * correctly toe the standard output.
 */
test_case(PatternEditor, Render_effect) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.render_effect(&out, 0, 0);
  assert_eq("0000 ", out.str());
  out.str("");
  pattern_editor.render_effect(&out, 1, 1);
  assert_eq("0101 ", out.str());
  out.str("");
  pattern_editor.render_effect(&out, 16, 16);
  assert_eq("1010 ", out.str());
}


/**
 * @test PatternEditor - Render reverse video
 *
 * Make sure that the correct ANSI escape code is outputed to the standard
 * output.
 */
test_case(PatternEditor, Render_reverse_video) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  pattern_editor.render_reverse_video(&out);
  assert_eq("\x1B[7m", out.str());
}


/**
 * @test PatternEditor - Render normal video
 *
 * Make sure that the correct ANSI escape code is outputed to the standard
 * output.
 */
test_case(PatternEditor, Render_normal_video) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  pattern_editor.render_normal_video(&out);
  assert_eq("\x1B[0m", out.str());
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
  stringstream out;
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  expect_call_times(pattern_editor, move_cursor_to_row(_,0), 1);

  pattern_editor.PatternEditor::clear_screen(&out);
  assert_eq("\x1B[2J", out.str());
}


/**
 * @test PatternEditor - Clear cursor columns.
 *
 * Make sure that the clear_cursor_columns() method leaves the list of
 * cursor columns empty.
 */
test_case(PatternEditor, Clear_cursor_columns) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);
  CursorColumn *cc1 = new CursorColumn();
  CursorColumn *cc2 = new CursorColumn();
  CursorColumn *cc3 = new CursorColumn();

  /**
   * It should be empty by default, but lets make sure it can be cleared
   * anyway.
   */
  pattern_editor.clear_cursor_columns();
  assert_eq(0, pattern_editor.cursor_column_list.size());

  /**
   * Add some entries to the list.
   */
  pattern_editor.cursor_column_list.push_back(cc1);
  pattern_editor.cursor_column_list.push_back(cc2);
  pattern_editor.cursor_column_list.push_back(cc3);

  /**
   * Just make sure that they actually ended up in the list.
   */
  assert_eq(3, pattern_editor.cursor_column_list.size());

  /**
   * Make sure that the list can be cleared again.
   */
  pattern_editor.clear_cursor_columns();
  assert_eq(0, pattern_editor.cursor_column_list.size());

}


/**
 * @test PatternEditor - Add note columns.
 *
 * Make sure that note columns can be added to the mapping table for columns
 * and actual console columns.
 */
test_case(PatternEditor, Add_note_columns) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.add_note_columns(1, 2, 3);

  assert_eq(3, pattern_editor.cursor_column_list.size());

  pattern_editor.add_note_columns(4, 5, 6);

  assert_eq(6, pattern_editor.cursor_column_list.size());

  CursorColumn *note = pattern_editor.cursor_column_list[0];
  CursorColumn *velocity1 = pattern_editor.cursor_column_list[1];
  CursorColumn *velocity2 = pattern_editor.cursor_column_list[2];

  assert_eq(NOTE_KEY, note->mode);
  assert_eq(2, note->track);
  assert_eq(3, note->index);
  assert_eq(1, note->column);

  assert_eq(NOTE_VELOCITY_HIGH_NIBBLE, velocity1->mode);
  assert_eq(2, velocity1->track);
  assert_eq(3, velocity1->index);
  assert_eq(1 + 4, velocity1->column);

  assert_eq(NOTE_VELOCITY_LOW_NIBBLE, velocity2->mode);
  assert_eq(2, velocity2->track);
  assert_eq(3, velocity2->index);
  assert_eq(1 + 5, velocity2->column);

  note = pattern_editor.cursor_column_list[3];
  velocity1 = pattern_editor.cursor_column_list[4];
  velocity2 = pattern_editor.cursor_column_list[5];

  assert_eq(NOTE_KEY, note->mode);
  assert_eq(5, note->track);
  assert_eq(6, note->index);
  assert_eq(4, note->column);

  assert_eq(NOTE_VELOCITY_HIGH_NIBBLE, velocity1->mode);
  assert_eq(5, velocity1->track);
  assert_eq(6, velocity1->index);
  assert_eq(4 + 4, velocity1->column);

  assert_eq(NOTE_VELOCITY_LOW_NIBBLE, velocity2->mode);
  assert_eq(5, velocity2->track);
  assert_eq(6, velocity2->index);
  assert_eq(4 + 5, velocity2->column);
}


/**
 * @test PatternEditor - Add effect columns.
 *
 * Make sure that effect columns can be added to the mapping table for columns
 * and actual console columns.
 */
test_case(PatternEditor, Add_effect_columns) {
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.add_effect_columns(1, 2, 3);

  assert_eq(4, pattern_editor.cursor_column_list.size());

  pattern_editor.add_effect_columns(4, 5, 6);

  assert_eq(8, pattern_editor.cursor_column_list.size());

  CursorColumn *command1 = pattern_editor.cursor_column_list[0];
  CursorColumn *command2 = pattern_editor.cursor_column_list[1];
  CursorColumn *velocity1 = pattern_editor.cursor_column_list[2];
  CursorColumn *velocity2 = pattern_editor.cursor_column_list[3];

  assert_eq(EFFECT_COMMAND_HIGH_NIBBLE, command1->mode);
  assert_eq(2, command1->track);
  assert_eq(3, command1->index);
  assert_eq(1, command1->column);

  assert_eq(EFFECT_COMMAND_LOW_NIBBLE, command2->mode);
  assert_eq(2, command2->track);
  assert_eq(3, command2->index);
  assert_eq(2, command2->column);

  assert_eq(EFFECT_VALUE_HIGH_NIBBLE, velocity1->mode);
  assert_eq(2, velocity1->track);
  assert_eq(3, velocity1->index);
  assert_eq(3, velocity1->column);

  assert_eq(EFFECT_VALUE_LOW_NIBBLE, velocity2->mode);
  assert_eq(2, velocity2->track);
  assert_eq(3, velocity2->index);
  assert_eq(4, velocity2->column);

  command1 = pattern_editor.cursor_column_list[4];
  command2 = pattern_editor.cursor_column_list[5];
  velocity1 = pattern_editor.cursor_column_list[6];
  velocity2 = pattern_editor.cursor_column_list[7];

  assert_eq(EFFECT_COMMAND_HIGH_NIBBLE, command1->mode);
  assert_eq(5, command1->track);
  assert_eq(6, command1->index);
  assert_eq(4, command1->column);

  assert_eq(EFFECT_COMMAND_LOW_NIBBLE, command2->mode);
  assert_eq(5, command2->track);
  assert_eq(6, command2->index);
  assert_eq(5, command2->column);

  assert_eq(EFFECT_VALUE_HIGH_NIBBLE, velocity1->mode);
  assert_eq(5, velocity1->track);
  assert_eq(6, velocity1->index);
  assert_eq(6, velocity1->column);

  assert_eq(EFFECT_VALUE_LOW_NIBBLE, velocity2->mode);
  assert_eq(5, velocity2->track);
  assert_eq(6, velocity2->index);
  assert_eq(7, velocity2->column);
}


/**
 * @test PatternEditor - Update cursor columns.
 *
 * Make sure that the current cursor column translation table is uppdated
 * correctly.
 */
test_case(PatternEditor, Update_cursor_columns) {
  const int NOTES = 2;
  const int EFFECTS = 3;
  MockTrack track;
  MockTracks tracks;
  MockSequencer sequencer;
  FakePatternEditorMock pattern_editor(&sequencer);

  tracks.push_back(&track);
  tracks.push_back(&track);

  expect_call_times_will_return(sequencer, get_tracks(), 1, &tracks);
  expect_call_times_will_return(track, get_notes(), tracks.size(), NOTES);
  expect_call_times_will_return(track, get_effects(), tracks.size(), EFFECTS);

  expect_call_times(pattern_editor, clear_cursor_columns(), 1);

  // Make sure that the columns are added in the correct order.
  int o = 0;
  for (unsigned int t = 0; t < tracks.size(); t++) {
    for (unsigned int n = 0; n < NOTES; n++) {
      expect_call_times_will_return(pattern_editor,
                                    add_note_columns(Eq(4 + o), Eq(t), Eq(n)),
                                    1, 4 + o + 1);
      o++;
    }
    for (unsigned int e = 0; e < EFFECTS; e++) {
      expect_call_times_will_return(pattern_editor,
                                    add_effect_columns(Eq(4 + o), Eq(t), Eq(e)),
                                    1, 4 + o + 1);
      o++;
    }
  }

  pattern_editor.PatternEditorMock::update_cursor_columns();

  assert_eq(2 * (NOTES * 3 + EFFECTS * 4), pattern_editor.PatternEditorMock::columns);
}


/**
 * @test PatternEditor - Get console column.
 *
 * Make sure that the correct console column number is provided.
 */
test_case(PatternEditor, Get_console_column) {
  CursorColumn *cc1 = new CursorColumn();
  CursorColumn *cc2 = new CursorColumn();

  cc1->column = 99;
  cc2->column = 98;

  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.cursor_column_list.push_back(cc1);
  pattern_editor.cursor_column_list.push_back(cc2);

  assert_eq(99, pattern_editor.get_console_column(0));
  assert_eq(98, pattern_editor.get_console_column(1));
}


/**
 * @test PatternEditor - Move cursor to row
 *
 * Make sure that the cursor is moved to the correct row using escape codes.
 */
test_case(PatternEditor, Move_cursor_to_row) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.move_cursor_to_row(&out, 2);
  assert_eq("\x1B[3;0H", out.str());
}


/**
 * @test PatternEditor - Move cursor to column
 *
 * Make sure that the cursor is moved to the correct column using escape
 * codes.
 */
test_case(PatternEditor, Move_cursor_to_column) {
  stringstream out;
  MockSequencer sequencer;
  PatternEditor pattern_editor(&sequencer);

  pattern_editor.move_cursor_to_column(&out, 2);
  assert_eq("\x1B[2G", out.str());
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

  stringstream out;

  track_entries.push_back(&track_entry);

  notes.push_back(&note);
  notes.push_back(&note);
  notes.push_back(&note);

  effects.push_back(&effect);
  effects.push_back(&effect);
  effects.push_back(&effect);

  expect_call_times(pattern_editor, render_row_number(&out,_), 2);
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
                    render_note(_,
                                Eq(static_cast<unsigned int>(0)),
                                Eq(static_cast<unsigned int>(0))), 6);

  // Make another row selected
  pattern_editor.PatternEditorTemplate::row_index = 1;

  pattern_editor.PatternEditorTemplate::render_row(&out, 0);

  /*
   * Since one of the rows must be selected.
   */
  expect_call_times(pattern_editor, render_reverse_video(&out), 1);
  expect_call_times(pattern_editor, render_normal_video(&out), 1);

  // Make the row to be rendered selected
  pattern_editor.PatternEditorTemplate::row_index = 0;

  pattern_editor.PatternEditorTemplate::render_row(&out, 0);

  /**
   * Calling the stream wrapper method shall call the generic method and
   * output things to stdout.
   */
  expect_call_times(pattern_editor, render_row(_,12), 1);
  assert_stdout_eq("", pattern_editor.PatternEditorTemplate::render_row(12));

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

  int lines = w.ws_row - 2; // Remove header rows
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
    //    pattern_editor.row_index = i;
    assert_eq(static_cast<unsigned int>(0),
              pattern_editor.calculate_pattern_render_offset(i));
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
    int offset = pattern_editor.calculate_pattern_render_offset(i);

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

  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  stringstream out;

  /*
   * Rendering a pattern of four rows on a screen of 7 (2 in the heading)
   * rows shall render four rows.
   */
  pattern_editor.pattern_length = pattern_rows.size();

  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 7);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(_), 1, 0);

  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(0))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(1))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(3))), 1);

  /*
   * Call the design under testing.
   */
  pattern_editor.PatternEditor::render_pattern(&out);
  assert_eq("\n\n\n", out.str());
  out.str("");

  verify_and_clear(pattern_editor);

  /*
   * Adding a couple of rows to the pattern will result in rendering 5
   * (screen height - heading) rows.
   */

  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  pattern_editor.pattern_length = pattern_rows.size();

  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 7);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(_), 1, 0);


  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(0))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(1))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(3))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(4))), 1);

  /*
   * Call the design under testing.
   */
  pattern_editor.PatternEditor::render_pattern(&out);
  assert_eq("\n\n\n\n", out.str());
  out.str("");

  verify_and_clear(pattern_editor);

  /*
   * Adding a couple of more rows to the pattern will result in rendering 5
   * (screen height - heading) rows but with the specified offset
   */
  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  pattern_editor.pattern_length = pattern_rows.size();

  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 7);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(_), 1, 2);


  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(2))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(3))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(4))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(5))), 1);
  expect_call_times(pattern_editor,
                    render_row(Eq(&out),Eq(static_cast<unsigned int>(6))), 1);
  /*
   * Call the design under testing.
   */
  pattern_editor.PatternEditor::render_pattern(&out);
  assert_eq("\n\n\n\n", out.str());
  out.str("");
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
 * @test PatternEditor - Key left will select previous pattern column.
 *
 * Make sure that the keyboard input of the left-arrow has the expected
 * effect.
 */
test_case(PatternEditor, Key_left_will_select_previous_pattern_column) {
  const unsigned int COLUMN = 2;
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;
  pattern_editor.PatternEditor::column = COLUMN;

  expect_call_times_will_return(pattern_editor,
                                get_console_column(Eq(COLUMN - 1)),
                                1,
                                99);

  expect_call_times(pattern_editor, move_cursor_to_column(_,Eq(99)), 1);
  expect_call_times_will_return(pattern_editor, getch(), 1, 'D');

  assert_eq(true, pattern_editor.PatternEditor::main_loop());
}


/**
 * @test PatternEditor - Key right will select next pattern column.
 *
 * Make sure that the keyboard input of the right-arrow has the expected
 * effect.
 */
test_case(PatternEditor, Key_right_will_select_next_pattern_column) {
  const unsigned int COLUMN = 2;
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);

  pattern_editor.edit_mode = false;
  pattern_editor.columns = COLUMN + 2;
  pattern_editor.PatternEditor::column = COLUMN;

  expect_call_times_will_return(pattern_editor,
                                get_console_column(Eq(COLUMN + 1)),
                                1,
                                99);

  expect_call_times(pattern_editor, move_cursor_to_column(_,Eq(99)), 1);
  expect_call_times_will_return(pattern_editor, getch(), 1, 'C');

  assert_eq(true, pattern_editor.PatternEditor::main_loop());
}


/**
 * @test PatternEditor - Q will quite main loop.
 *
 * Make sure that the keyboard input of the Q has the expected
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
 * @test PatternEditor - Keyboard will send notes to sequencer.
 *
 * Make sure that the computer keyboard is mapped correctly to notes sent to
 * the sequencer.
 */
test_case(PatternEditor, Keyboard_will_send_notes_to_sequencer) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);
  pattern_editor.edit_mode = true;

  CursorColumn *cc1 = new CursorColumn();
  CursorColumn *cc2 = new CursorColumn();
  CursorColumn *cc3 = new CursorColumn();

  /**
   * Set-up some different note columns on a couple of tracks.
   */
  cc1->column = cc2->column = cc3->column = 0; // Does not matter.
  cc1->mode = cc2->mode = cc3->mode = NOTE_KEY;
  cc1->track = 0;
  cc1->index = 0;
  cc2->track = 0;
  cc2->index = 1;
  cc3->track = 1;
  cc3->index = 0;

  pattern_editor.cursor_column_list.push_back(cc1);
  pattern_editor.cursor_column_list.push_back(cc2);
  pattern_editor.cursor_column_list.push_back(cc3);

  // C-2 in track 0 in the first note column.
  pattern_editor.column = 0;
  expect_call_times_will_return(pattern_editor, getch(), 1, 'q');
  expect_call_times(sequencer, set_key(Eq(0), Eq(0), Eq(13)), 1);
  assert_true(pattern_editor.PatternEditor::main_loop());

  // C#2 in track 0 in the second note column.
  pattern_editor.column = 1;
  expect_call_times_will_return(pattern_editor, getch(), 1, '2');
  expect_call_times(sequencer, set_key(Eq(0), Eq(1), Eq(14)), 1);
  assert_true(pattern_editor.PatternEditor::main_loop());

  // D-2 in track 1 in the first note column.
  pattern_editor.column = 2;
  expect_call_times_will_return(pattern_editor, getch(), 1, 'w');
  expect_call_times(sequencer, set_key(Eq(1), Eq(0), Eq(15)), 1);
  assert_true(pattern_editor.PatternEditor::main_loop());

  /**
   * Traverse the whole keyboard and make sure that the mapping is correct.
   *
   * @todo Fix this for scan codes when the pattern editor accepts them
   *       instead of actually localized key maps.
   *
   * @todo Extend with octave selection.
   *
   * @todo Break-out this test when the above is implemented.
   */
  const char *keys = "<azsxdcfvgbhnjmk,l.ö-ä1q2w3e4r5t6y7u8i9o0p+å";

  int values[44];

  values[0] = 1;   // < -> C-1
  values[1] = 2;   // a -> C#1
  values[2] = 3;   // z -> D-1
  values[3] = 4;   // s -> D#1
  values[4] = 5;   // x -> E-1
  values[5] = 0;   // d
  values[6] = 6;   // c -> F-1
  values[7] = 7;   // f -> F#1
  values[8] = 8;   // v -> G-1
  values[9] = 9;   // g -> G#1
  values[10] = 10; // b -> A-1
  values[11] = 11; // h -> A#1
  values[12] = 12; // n -> H-1
  values[13] = 0;  // j
  values[14] = 13; // m -> C-2
  values[15] = 14; // k -> C#2
  values[16] = 15; // , -> D-2
  values[17] = 16; // l -> D#2
  values[18] = 17; // . -> E-2
  values[19] = 0;  // ö
  values[20] = 18; // - -> F-2
  values[21] = 0;  // ä

  values[22] = 0;  // 1
  values[23] = 13; // q -> C-2
  values[24] = 14; // 2 -> C#2
  values[25] = 15; // w -> D-2
  values[26] = 16; // 3 -> D#2
  values[27] = 17; // e -> E-2
  values[28] = 0;  // 4
  values[29] = 18; // r -> F-2
  values[30] = 19; // 5 -> F#2
  values[31] = 20; // t -> G-2
  values[32] = 21; // 6 -> G#2
  values[33] = 22; // y -> A-2
  values[34] = 23; // 7 -> A#2
  values[35] = 24; // u -> H-2
  values[36] = 0;  // 8
  values[37] = 25; // i -> C-3
  values[38] = 26; // 9 -> C#3
  values[39] = 27; // o -> D-3
  values[40] = 28; // 0 -> D#3
  values[41] = 29; // p -> E-3
  values[42] = 0;  // +
  values[43] = 30; // å -> F-3

  pattern_editor.column = 0;

  for (unsigned int i = 0; i < 42; i++) {
    expect_call_times_will_return(pattern_editor, getch(), 1, keys[i]);
    expect_call_times(sequencer, set_key(Eq(0), Eq(0), Eq(values[i])), (values[i] != 0));
    assert_true(pattern_editor.PatternEditor::main_loop());
    verify_and_clear(sequencer);
  }
}


/**
 * @test PatternEditor - Keyboard will send velocity nibbles to the
 *                       sequencer.
 */
test_case(PatternEditor, Keyboard_will_send_velocity_nibbles_to_the_sequencer) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);
  pattern_editor.edit_mode = true;

  CursorColumn *cc = new CursorColumn();

  /**
   * Set-up some different note columns on a couple of tracks.
   */
  cc->column = 0;
  cc->track = 0;
  cc->index = 0;

  pattern_editor.cursor_column_list.push_back(cc);

  pattern_editor.column = 0;
  expect_call_times_will_return(pattern_editor, getch(), 1, '0');
  expect_call_times(sequencer, set_velocity(Eq(0), Eq(0), Eq(0), Eq(true)), 1);
  cc->mode = NOTE_VELOCITY_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'f');
  expect_call_times(sequencer, set_velocity(Eq(0), Eq(0), Eq(15), Eq(true)), 1);
  cc->mode = NOTE_VELOCITY_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, '1');
  expect_call_times(sequencer, set_velocity(Eq(0), Eq(0), Eq(1), Eq(false)), 1);
  cc->mode = NOTE_VELOCITY_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'e');
  expect_call_times(sequencer, set_velocity(Eq(0), Eq(0), Eq(14), Eq(false)), 1);
  cc->mode = NOTE_VELOCITY_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);
}


/**
 * @test PatternEditor - Keyboard will send command nibbles to the
 *                       sequencer.
 */
test_case(PatternEditor, Keyboard_will_send_command_nibbles_to_the_sequencer) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);
  pattern_editor.edit_mode = true;

  CursorColumn *cc = new CursorColumn();

  /**
   * Set-up some different effect columns on a couple of tracks.
   */
  cc->column = 0;
  cc->track = 0;
  cc->index = 0;

  pattern_editor.cursor_column_list.push_back(cc);

  pattern_editor.column = 0;
  expect_call_times_will_return(pattern_editor, getch(), 1, '2');
  expect_call_times(sequencer, set_command(Eq(0), Eq(0), Eq(2), Eq(true)), 1);
  cc->mode = EFFECT_COMMAND_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'd');
  expect_call_times(sequencer, set_command(Eq(0), Eq(0), Eq(13), Eq(true)), 1);
  cc->mode = EFFECT_COMMAND_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, '3');
  expect_call_times(sequencer, set_command(Eq(0), Eq(0), Eq(3), Eq(false)), 1);
  cc->mode = EFFECT_COMMAND_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'c');
  expect_call_times(sequencer, set_command(Eq(0), Eq(0), Eq(12), Eq(false)), 1);
  cc->mode = EFFECT_COMMAND_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);
}


/**
 * @test PatternEditor - Keyboard will send value nibbles to the
 *                       sequencer.
 */
test_case(PatternEditor, Keyboard_will_send_value_nibbles_to_the_sequencer) {
  MockSequencer sequencer;
  FakePatternEditor pattern_editor(&sequencer);
  pattern_editor.edit_mode = true;

  CursorColumn *cc = new CursorColumn();

  /**
   * Set-up some different effect columns on a couple of tracks.
   */
  cc->column = 0;
  cc->track = 0;
  cc->index = 0;

  pattern_editor.cursor_column_list.push_back(cc);

  pattern_editor.column = 0;
  expect_call_times_will_return(pattern_editor, getch(), 1, '4');
  expect_call_times(sequencer, set_value(Eq(0), Eq(0), Eq(4), Eq(true)), 1);
  cc->mode = EFFECT_VALUE_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'b');
  expect_call_times(sequencer, set_value(Eq(0), Eq(0), Eq(11), Eq(true)), 1);
  cc->mode = EFFECT_VALUE_HIGH_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, '5');
  expect_call_times(sequencer, set_value(Eq(0), Eq(0), Eq(5), Eq(false)), 1);
  cc->mode = EFFECT_VALUE_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);

  expect_call_times_will_return(pattern_editor, getch(), 1, 'a');
  expect_call_times(sequencer, set_value(Eq(0), Eq(0), Eq(10), Eq(false)), 1);
  cc->mode = EFFECT_VALUE_LOW_NIBBLE;
  assert_true(pattern_editor.PatternEditor::main_loop());

  verify_and_clear(sequencer);
}


/**
 * @test PatternEditor - Set nibble.
 *
 * Make sure that the nibble is rendered and the cursor moved if possible.
 */
test_case(PatternEditor, Set_nibble) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);
  pattern_editor.PatternEditor::column = 10;
  expect_call_times(pattern_editor, render_nibble(_, Eq(0), Eq(1)), 1);
  expect_call_times(pattern_editor, render_nibble(_, Eq(0), Eq(2)), 1);
  pattern_editor.PatternEditor::set_nibble(1, true);
  assert_eq(11, pattern_editor.PatternEditor::column);
  pattern_editor.PatternEditor::set_nibble(2, false);
  assert_eq(12, pattern_editor.PatternEditor::column);
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


/**
 * @test PatternEditor - Set pattern length.
 *
 * Make sure that the pattern length is updated and the pattern is
 * re-rendered.
 */
test_case(PatternEditor, Set_pattern_length) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  expect_call_times(pattern_editor, render_pattern(_), 2);

  pattern_editor.PatternEditor::set_pattern_length(10);
  assert_eq(10, pattern_editor.pattern_length);

  pattern_editor.PatternEditor::set_pattern_length(1);
  assert_eq(1, pattern_editor.pattern_length);
}

/**
 * @test PatternEditor - Set key
 *
 * Make sure that the note key is set correctly on the current cursor
 * position.
 */
test_case(PatternEditor, Set_key) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  pattern_editor.PatternEditor::row_index = 5;
  expect_call_times(sequencer, set_pattern_row_index(6), 1);

  pattern_editor.PatternEditor::set_key(0, 0, 0);
}

/**
 * @test PatternEditor - Set velocity rely on set_nibble.
 */
test_case(PatternEditor, Set_velocity_rely_on_set_nibble) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  expect_call_times(pattern_editor, set_nibble(Eq(0), Eq(false)), 1);
  expect_call_times(pattern_editor, set_nibble(Eq(15), Eq(true)), 1);

  pattern_editor.PatternEditor::set_velocity(0, 0, 0, false);
  pattern_editor.PatternEditor::set_velocity(0, 0, 15, true);
}


/**
 * @test PatternEditor - Set command rely on set_nibble.
 */
test_case(PatternEditor, Set_command_rely_on_set_nibble) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  expect_call_times(pattern_editor, set_nibble(Eq(0), Eq(false)), 1);
  expect_call_times(pattern_editor, set_nibble(Eq(15), Eq(true)), 1);

  pattern_editor.PatternEditor::set_command(0, 0, 0, false);
  pattern_editor.PatternEditor::set_command(0, 0, 15, true);
}



/**
 * @test PatternEditor - Set value rely on set_nibble.
 */
test_case(PatternEditor, Set_value_rely_on_set_nibble) {
  MockSequencer sequencer;
  NiceMock<FakePatternEditor> pattern_editor(&sequencer);

  expect_call_times(pattern_editor, set_nibble(Eq(0), Eq(false)), 1);
  expect_call_times(pattern_editor, set_nibble(Eq(15), Eq(true)), 1);

  pattern_editor.PatternEditor::set_value(0, 0, 0, false);
  pattern_editor.PatternEditor::set_value(0, 0, 15, true);
}
