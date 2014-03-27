/**
 * @file pattern_row_test.cc
 *
 * Test-suite for the PatternRow class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the PatternRow class to be able to
 * inspect the protected or private variables.
 */
#define PATTERN_ROW_FRIENDS                     \
  friend_test(PatternRow, Constructors)         \
  friend_test(PatternRow, SettersAndGetters)    \
  friend_test(PatternRow, Factory)              \

#include "mock_track_entry.hh"

/// Override the macro defined in pattern_row.hh for stubbed functionallity
#define TRACK_ENTRY_FACTORY_NEW(track) new MockTrackEntry(); track = track
/// Override the macro defined in pattern_row.hh for stubbed functionallity
#define TRACK_ENTRY_FACTORY_DELETE(track_entry) delete *(track_entry); \
  *track_entry = NULL

#include "pattern_row.hh"

#include "mock_pattern_row.hh"
#include "mock_track.hh"


/**
 * @test PatternRow constructor.
 *
 * Make sure that the track entries reference is transfered unchanged to the
 * internal storage.
 */
test_case(PatternRow, Constructors) {
  MockTrackEntries track_entries;
  PatternRow pattern_row(&track_entries);
  assert_eq(&track_entries, pattern_row.track_entries);
}


/**
 * @test PatternRow setters and getters.
 *
 * Make sure that the track entries are possible to be sot or fetched
 * correctly.
 */
test_case(PatternRow, SettersAndGetters) {
  MockTrackEntries track_entries;
  PatternRow pattern_row(&track_entries);
  assert_eq(&track_entries, pattern_row.get_track_entries());
}


/**
 * @test PatternRowFactory creates valid pattern row.
 *
 * Make sure that the PatternEntryFactory is able to create a valid pattern row
 * both with or without track entries-list reference argument, always matching
 * the configuration of the tracks in the referenced track-list.
 */
test_case(PatternRow, Factory) {
  const int TRACKS = 8;

  // Create a mock-up of the tracks list.
  MockTracks tracks;
  for (int i = 0; i < TRACKS; i++) { tracks.push_back(new MockTrack()); }

  // Normal operations for creating a new pattern row based on the track list.
  PatternRow* pattern_row = PatternRowFactoryMock::New(&tracks);
  assert_eq(TRACKS,
            dynamic_cast<MockTrackEntries*>(pattern_row->track_entries)->size());

  // Destroy the pattern row again.
  PatternRowFactoryMock::Delete(&pattern_row);
  assert_eq(NULL, pattern_row);

  // Try again but but with mismatching tracks-length and track_entries length.
  MockTrackEntries track_entries;
  for (int i = 0; i < TRACKS - 1; i++) {
    track_entries.push_back(new MockTrackEntry());
  }
  //  pattern_row = PatternRowFactoryMock::New(&tracks, &track_entries);
  assert_stderr_eq("ERROR: The number of track entity entries provided does not match the number of track entries.\n", pattern_row = PatternRowFactoryMock::New(&tracks, &track_entries));
  assert_eq(NULL, pattern_row);

  // Clean-up the mocked tracks
  for (int i = 0; i < TRACKS; i++) { delete tracks.back(); tracks.pop_back(); }

  for (int i = 0; i < TRACKS - 1; i++) {
    delete track_entries.back();
    track_entries.pop_back();
  }
}
