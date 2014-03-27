/**
 * @file pattern_editor_test.cc
 */

#define PATTERN_EDITOR_FRIENDS \
  friend class PatternEditor_Foo_Test;

#include "pattern_editor.hh"

#include "mock_project.hh"
#include "mock_track.hh"
#include "mock_client.hh"
#include "mock_client_primitive.hh"
#include "mock_pattern.hh"
#include "mock_pattern_row.hh"
#include "mock_track_entry.hh"
#include "mock_note.hh"
#include "mock_effect.hh"
#include "mock_song.hh"
#include "mock_part.hh"
#include "mock_sequencer.hh"

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "test.hh"

expose_class_1(PatternEditor, SequencerInterface*, sequencer);

/*
 * PatternEditor class with all referenced classes as mocks.
 */
typedef PatternEditorTemplate<MockSequencer,
                              MockPatternRow,
                              MockTrackEntries,
                              MockTrackEntry,
                              MockNotes,
                              MockNote> PatternEditorMock;

/*
 * Since most of the PatternEditor is implemented in protected methods and
 * members let's create a convenience class to make it easier to stub/mock
 * methods and values making it friend with all test-cases.
 */
class FakePatternEditor : protected PatternEditorMock {

  /*
   * Unfortunetly you'll have to repeat this mecro both here and in the
   * template implementation.
   */
  PATTERN_EDITOR_FRIENDS

protected:
  /*
   * Remember to update these mocks whenever you add a protected method to
   * the template.
   */
  MOCK_METHOD1(render_key, void(unsigned int key)); // TODO: change to int
  MOCK_METHOD2(render_nibble, void(int nibble, int value));
  MOCK_METHOD1(render_velocity, void(unsigned int velocity)); // TODO: int
  MOCK_METHOD2(render_note, void(unsigned int key, unsigned int velocity));
  MOCK_METHOD1(render_row_number, void(unsigned int row_index));
  MOCK_METHOD0(render_reverse_video, void());
  MOCK_METHOD0(render_normal_video, void());
  MOCK_METHOD1(render_row, void(unsigned int row_index));
  MOCK_METHOD0(get_screen_height, int());
  MOCK_METHOD0(get_screen_width, int());
  MOCK_METHOD0(calculate_pattern_render_offset, int());
  MOCK_METHOD0(render_pattern, void());
  MOCK_METHOD0(main_loop, bool());
public:
  FakePatternEditor() : PatternEditorMock(NULL) {}
  FakePatternEditor(SequencerInterface *s) : PatternEditorMock(s) {}
};

test_case(PatternEditor, Foo) {
  MockSequencer s;
  FakePatternEditor p(&s);
  p.PatternEditorMock::main_loop();
}

class FakePatternEditorWithMockedProtectedMethods
  : public PatternEditorTemplate<MockSequencer,
                                 MockPatternRow,
                                 MockTrackEntries,
                                 MockTrackEntry,
                                 MockNotes,
                                 MockNote> {
  friend class PatternEditor_SettersAndGetters_Test;
  friend class PatternEditor_MainWillRenderAPatternAndCallTheMainLoopAndExit_Test;
protected:
  MOCK_METHOD1(render_row, void(unsigned int));
  MOCK_METHOD0(render_pattern, void());
  MOCK_METHOD0(main_loop, bool());
public:
  FakePatternEditorWithMockedProtectedMethods(SequencerInterface *sequencer)
  : PatternEditorTemplate(sequencer) {}
  ~FakePatternEditorWithMockedProtectedMethods() {}
};

class FakePatternEditorWithMockedColumnRenderers :
  public PatternEditorTemplate<MockSequencer,
                               MockPatternRow,
                               MockTrackEntries,
                               MockTrackEntry,
                               MockNotes,
                               MockNote> {
  friend class PatternEditor_RenderRowShallRenderAllVisibleRowColumns_Test;
protected:
  MOCK_METHOD1(render_row_number, void(unsigned int));
  MOCK_METHOD2(render_note, void(unsigned int, unsigned int));
  MOCK_METHOD0(render_reverse_video, void());
  MOCK_METHOD0(render_normal_video, void());
public:
  FakePatternEditorWithMockedColumnRenderers(SequencerInterface *sequencer)
  : PatternEditorTemplate(sequencer) {}
  ~FakePatternEditorWithMockedColumnRenderers() {}
};


class FakePatternEditorWithMockedRowRenderer :
  public PatternEditorTemplate<MockSequencer,
                               MockPatternRow,
                               MockTrackEntries,
                               MockTrackEntry,
                               MockNotes,
                               MockNote> {
  friend class PatternEditor_RenderPatternShallRenderAllVisibleRows_Test;
protected:
  MOCK_METHOD1(render_row, void(unsigned int));
  MOCK_METHOD0(calculate_pattern_render_offset, int());
  MOCK_METHOD0(get_screen_height, int());
public:
  FakePatternEditorWithMockedRowRenderer(SequencerInterface *sequencer)
  : PatternEditorTemplate(sequencer) {}
  ~FakePatternEditorWithMockedRowRenderer() {}
};


test_case(PatternEditor, Constructors) {
  MockSequencer sequencer;
  PatternEditorExposed1 pattern_editor(&sequencer);
  assert_eq(&sequencer, pattern_editor.sequencer);
  assert_eq(0, pattern_editor.row_index);
  assert_eq(0, pattern_editor.track_index);
}

test_case(PatternEditor, SettersAndGetters) {
  MockPatternRow pattern_row;
  MockSequencer sequencer;
  FakePatternEditorWithMockedProtectedMethods pattern_editor(&sequencer);

  // Defailt (0 will be re-rendered in normal video when index 2 is rendered)
  expect_call_times(pattern_editor, render_row(Eq(0)), 1);
  // Row index 2 (2 will be re-rendered in normal if index 5 is rendered)
  expect_call_times(pattern_editor, render_row(Eq(2)), 2);
  // Row index 5
  expect_call_times(pattern_editor, render_row(Eq(5)), 1);
  // Pattern length 2 and 5
  expect_call_times(pattern_editor, render_pattern(), 2);

  pattern_editor.set_pattern_row_index(2);
  assert_eq(2, pattern_editor.row_index);
  pattern_editor.set_pattern_row_index(5);
  assert_eq(5, pattern_editor.row_index);

  pattern_editor.set_pattern_length(2);
  assert_eq(2, pattern_editor.pattern_length);
  pattern_editor.set_pattern_length(5);
  assert_eq(5, pattern_editor.pattern_length);

  pattern_editor.set_track_index(2);
  assert_eq(2, pattern_editor.track_index);
  pattern_editor.set_track_index(5);
  assert_eq(5, pattern_editor.track_index);
}

class PatternEditorExposed : public PatternEditorExposed1 {

  friend class PatternEditor_RenderKeyShallRenderCorrectKey_Test;
  friend class PatternEditor_RenderNibbleShallRenderCorrectByteNibbleInHex_Test;
  friend class PatternEditor_RenderRowNumberShallRenderCorrectRowNumberInHex_Test;
  friend class PatternEditor_RenderNoteShallRenderCorrectNote_Test;
  friend class PatternEditor_GetScreenHeightShallReturnTerminalHeight_Test;
  friend class PatternEditor_GetScreenWidthShallReturnTerminalWidth_Test;
  friend class PatternEditor_CalculatePatternRenderOffsetShallAdaptToScreen_Test;  friend class PatternEditor_RenderReverseVideoShallOutputEscapeCodeToStdout_Test;
  friend class PatternEditor_RenderNormalVideoShallOutputEscapeCodeToStdout_Test;

public:

  PatternEditorExposed() : PatternEditorExposed1(NULL) {}
  PatternEditorExposed(SequencerInterface *sequencer) :
  PatternEditorExposed1(sequencer) {}

};

test_case(PatternEditor, RenderKeyShallRenderCorrectKey) {
  PatternEditorExposed pattern_editor;

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

test_case(PatternEditor, RenderRowNumberShallRenderCorrectRowNumberInHex) {
  PatternEditorExposed pattern_editor;
  assert_stdout_eq("00 ", pattern_editor.render_row_number(0));
  assert_stdout_eq("0f ", pattern_editor.render_row_number(15));
  assert_stdout_eq("ff ", pattern_editor.render_row_number(255));
}

test_case(PatternEditor, RenderNibbleShallRenderCorrectByteNibbleInHex) {
  PatternEditorExposed pattern_editor;
  assert_stdout_eq("2", pattern_editor.render_nibble(0, 0x12));
  assert_stdout_eq("1", pattern_editor.render_nibble(1, 0x12));
}

test_case(PatternEditor, RenderVelocityShallRenderTwoNibblesInHex) {
}

test_case(PatternEditor, RenderNoteShallRenderCorrectNote) {
  PatternEditorExposed pattern_editor;

  assert_stdout_eq("--- 00 ", pattern_editor.render_note(0, 0));
  assert_stdout_eq("C-1 01 ", pattern_editor.render_note(1, 1));
}

test_case(PatternEditor, RenderRowShallRenderAllVisibleRowColumns) {
  MockNote note;
  MockNotes notes;
  MockTrackEntry track_entry;
  MockTrackEntries track_entries;
  MockPatternRow pattern_row;
  MockSequencer sequencer;
  FakePatternEditorWithMockedColumnRenderers pattern_editor(&sequencer);

  track_entries.push_back(&track_entry);
  notes.push_back(&note);
  notes.push_back(&note);
  notes.push_back(&note);

  expect_call_times(pattern_editor, render_row_number(_), 2);
  expect_call_times_will_return(sequencer, get_row(_), 2, &pattern_row);
  expect_call_times_will_return(pattern_row, get_track_entries(), 2,
                                &track_entries);
  expect_call_times_will_return(track_entry, get_notes(), 2, &notes);
  expect_call_times_will_return(note, get_key(), 6, 0);
  expect_call_times_will_return(note, get_velocity(), 6, 0);
  expect_call_times(pattern_editor, render_note(Eq(0),Eq(0)), 6);

  pattern_editor.row_index = 1; // Make another row selected

  pattern_editor.render_row(0);

  /*
   * Since one of the rows must be selected.
   */
  expect_call_times(pattern_editor, render_reverse_video(), 1);
  expect_call_times(pattern_editor, render_normal_video(), 1);

  pattern_editor.row_index = 0; // Make the row to be rendered selected

  pattern_editor.render_row(0);

}

test_case(PatternEditor, RenderReverseVideoShallOutputEscapeCodeToStdout) {
  PatternEditorExposed pattern_editor;
  assert_stdout_eq("\x1B[7m", pattern_editor.render_reverse_video());
}

test_case(PatternEditor, RenderNormalVideoShallOutputEscapeCodeToStdout) {
  PatternEditorExposed pattern_editor;
  assert_stdout_eq("\x1B[0m", pattern_editor.render_normal_video());
}

test_case(PatternEditor, GetScreenHeightShallReturnTerminalHeight) {
  PatternEditorExposed pattern_editor;

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  int lines = w.ws_row;

  assert_eq(lines, pattern_editor.get_screen_height());
}

test_case(PatternEditor, GetScreenWidthShallReturnTerminalWidth) {
  PatternEditorExposed pattern_editor;

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  int columns = w.ws_col;

  assert_eq(columns, pattern_editor.get_screen_width());
}

test_case(PatternEditor, CalculatePatternRenderOffsetShallAdaptToScreen) {
  MockSequencer sequencer;
  PatternEditorExposed pattern_editor(&sequencer);

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
    assert_eq(0, pattern_editor.calculate_pattern_render_offset());
  }

  rows = lines + 3;

  /*
   * Mock the system to beleive that the pattern is longer than the number
   * of lines available on the screen.
   */
  pattern_editor.pattern_length = rows;
  //expect_call_times_will_return(sequencer, get_row_count(), rows, rows);

  /*
   * A pattern fitting on a terminal should offset two lines, emulate scolling
   * through it and reach the end of the pattern.
   */
  for (int i = 0; i < rows; i++) {
    pattern_editor.row_index = i;
    int offset = pattern_editor.calculate_pattern_render_offset();
    if (i <= (lines / 2)) {
      // Selected row has not reached half screen yet.
      assert_eq(0, offset);
    }
    else if ((i - lines) <= (lines / 2) && (i <= (lines - lines / 2))) {
      // Selected row has reached half screen but last row is not yet printed.
      assert_eq((i - (lines / 2)), offset);
    }
    else {
      // Last line is printed, no need to dynamically offset anymore.
      assert_eq(rows - lines, offset);
    }
  }

}

test_case(PatternEditor, RenderPatternShallRenderAllVisibleRows) {
  MockPattern pattern;
  MockPatternRow pattern_row;
  MockPatternRows pattern_rows;
  MockSequencer sequencer;

  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);
  pattern_rows.push_back(&pattern_row);

  FakePatternEditorWithMockedRowRenderer pattern_editor(&sequencer);

  /*
   * Rendering a pattern of four rows on a screen of 5 rows shall render
   * four rows.
   */
  pattern_editor.pattern_length = pattern_rows.size();
  //  expect_call_times_will_return(sequencer, get_row_count(), 1, pattern_rows.size());
  expect_call_times_will_return(pattern_editor, get_screen_height(), 1, 5);
  expect_call_times_will_return(pattern_editor, calculate_pattern_render_offset(), 1, 0);

  expect_call_times(pattern_editor, render_row(Eq(0)), 1);
  expect_call_times(pattern_editor, render_row(Eq(1)), 1);
  expect_call_times(pattern_editor, render_row(Eq(2)), 1);
  expect_call_times(pattern_editor, render_row(Eq(3)), 1);

  /*
   * Call the design under testing.
   */
  pattern_editor.render_pattern();

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


  expect_call_times(pattern_editor, render_row(Eq(0)), 1);
  expect_call_times(pattern_editor, render_row(Eq(1)), 1);
  expect_call_times(pattern_editor, render_row(Eq(2)), 1);
  expect_call_times(pattern_editor, render_row(Eq(3)), 1);
  expect_call_times(pattern_editor, render_row(Eq(4)), 1);

  /*
   * Call the design under testing.
   */
  pattern_editor.render_pattern();

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


  expect_call_times(pattern_editor, render_row(Eq(2)), 1);
  expect_call_times(pattern_editor, render_row(Eq(3)), 1);
  expect_call_times(pattern_editor, render_row(Eq(4)), 1);
  expect_call_times(pattern_editor, render_row(Eq(5)), 1);
  expect_call_times(pattern_editor, render_row(Eq(6)), 1);

  /*
   * Call the design under testing.
   */
  pattern_editor.render_pattern();
}

class FakePatternEditorWithMockedMainLoop
  : public PatternEditorTemplate<MockSequencer,
                                 MockPatternRow,
                                 MockTrackEntries,
                                 MockTrackEntry,
                                 MockNotes,
                                 MockNote> {
  friend class PatternEditor_SettersAndGetters_Test;
  friend class PatternEditor_MainWillRenderAPatternAndCallTheMainLoopAndExit_Test;
protected:
  MOCK_METHOD0(main_loop, bool());
public:
  FakePatternEditorWithMockedMainLoop(SequencerInterface *sequencer)
  : PatternEditorTemplate(sequencer) {}
};

test_case(PatternEditor, MainWillRenderAPatternAndCallTheMainLoopAndExit) {
  MockSequencer sequencer;
  FakePatternEditorWithMockedMainLoop pattern_editor(&sequencer);

  FakePatternEditorWithMockedMainLoop* pe_ptr = &pattern_editor;

  PatternEditorInterface* pattern_editor_ptr =
    dynamic_cast<PatternEditorInterface*>(pe_ptr);

  ClientPrimitiveInterface* client =
    dynamic_cast<ClientPrimitiveInterface*>(pattern_editor_ptr);

  expect_call_times(sequencer, register_client(Eq(client)), 1);
  expect_call_times(sequencer, unregister_client(Eq(client)), 1);

  expect_call_times_will_return(pattern_editor, main_loop(), 1, false);

  assert_eq(0, pattern_editor.main(0, NULL));
}
