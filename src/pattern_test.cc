/**
 * @file pattern_test.cc
 *
 * Test-suite for Pattern class.
 *
 */
#include "mock_pattern_row.hh"

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_ROW_FACTORY_NEW(tracks) \
  new MockPatternRow()

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_ROW_FACTORY_DELETE(pattern_row) delete *(pattern_row); \
  *pattern_row = NULL

#include "test.hh"

/**
 * Make all relevant test cases friends with the Pattern class to be able to
 * inspect the protected or private variables.
 */
#define PATTERN_FRIENDS                         \
  friend_test(Pattern, Constructors)            \
  friend_test(Pattern, SettersAndGetters)       \
  friend_test(Pattern, Factory)                 \


#include "pattern.hh"

#include "mock_track_entry.hh"

#include "mock_pattern.hh"
#include "mock_track.hh"


/**
 * @test Pattern constructor.
 *
 * Make sure that the pattern rows list reference is passed to the internal
 * storage untouched.
 */
test_case(Pattern, Constructors) {
  MockPatternRow *foo = new MockPatternRow();
  foo = foo;
  MockPatternRows pattern_rows;
  Pattern pattern(&pattern_rows);
  assert_eq(&pattern_rows, pattern.pattern_rows);
}


/**
 * @test Pattern setters and getters.
 *
 * Make sure that the pattern rows reference is possible to be set of fetched
 * correctly.
 */
test_case(Pattern, SettersAndGetters) {
  MockPatternRows pattern_rows;
  Pattern pattern(&pattern_rows);
  assert_eq(&pattern_rows, pattern.get_pattern_rows());
}


/**
 * @test PatternFactory creates valid pattern.
 *
 * Make sure that the PatternFactory is able to create a valid pattern both
 * with pattern rows-list reference argument and without, always matching
 * the provided configuration. Also make sure that the user is informed if
 * something went wrong via the standard error output.
 */
test_case(Pattern, Factory) {
  const unsigned int ROWS = 64;
  const unsigned int TRACKS = 8;

  // Create a mock-up of the tracks list.
  MockTracks tracks;
  for (unsigned int i = 0; i < TRACKS; i++) {
    tracks.push_back(new MockTrack());
  }

  /*
   * Normal operations for creating a new pattern based on the track list and
   * the number of rows.
   */
  Pattern *pattern = PatternFactoryMock::New(&tracks, ROWS);
  assert_eq(ROWS,
            dynamic_cast<MockPatternRows*>(pattern->pattern_rows)->size());

  // Destroy the pattern again.
  PatternFactoryMock::Delete(&pattern);
  assert_eq(NULL, pattern);

  // Try again but with mismatching rows-param and pattern_rows contents.
  MockPatternRows pattern_rows;
  for (unsigned int i = 0; i < ROWS - 1; i++) {
    pattern_rows.push_back(new MockPatternRow());
  }
  assert_stderr_eq("ERROR: The number of pattern rows provided does not "
                   "match the number of pattern rows configruationprovided "
                   "to this factory.\n",
                   pattern = PatternFactoryMock::New(&tracks,
                                                     ROWS,
                                                     &pattern_rows));
  assert_eq(NULL, pattern);

  // Clean-up the mocked tracks
  for (unsigned int i = 0; i < TRACKS; i++) {
    delete tracks.back(); tracks.pop_back();
  }
}


/**
 * @test Pattern - Factory adding rows shall add rows.
 *
 * Make sure that rows are added when the factory method AddRows is called.
 */
test_case(Pattern, Factory_adding_rows_shall_add_rows) {
  const unsigned int ROWS= 2;
  const unsigned int TRACKS = 8;

  MockTracks tracks;
  MockTrack track;
  Pattern *pattern = PatternFactoryMock::New(&tracks, ROWS);

  for (unsigned int i = 0; i < TRACKS; i++) { tracks.push_back(&track); }

  PatternFactoryMock::AddRows(&tracks, pattern, 6);

  MockPatternRows *rows =
    dynamic_cast<MockPatternRows*>(pattern->get_pattern_rows());

  assert_eq(static_cast<unsigned int>(8), rows->size());
}


/**
 * @test Pattern - Factory removing rows shall remove rows.
 *
 * Make sure that rows are removed when the factory method RemoveRows is
 * called.
 */
test_case(Pattern, Factory_removing_rows_shall_remove_rows) {
  const unsigned int ROWS= 8;
  const unsigned int TRACKS = 8;

  MockTracks tracks;
  MockTrack track;
  Pattern *pattern = PatternFactoryMock::New(&tracks, ROWS);

  for (unsigned int i = 0; i < TRACKS; i++) { tracks.push_back(&track); }

  PatternFactoryMock::RemoveRows(pattern, 2);

  MockPatternRows *rows =
    dynamic_cast<MockPatternRows*>(pattern->get_pattern_rows());

  assert_eq(static_cast<unsigned int>(6), rows->size());
}
