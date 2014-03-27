/**
 * @file track_entry_test.cc
 *
 * Test-suite for the TrackEntry class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the TrackEntry class to be able to
 * inspect the protected or private variables.
 */
#define TRACK_ENTRY_FRIENDS                    \
  friend_test(TrackEntry, Constructors)        \
  friend_test(TrackEntry, Getters)             \
  friend_test(TrackEntry, Factory)             \

#include "mock_note.hh"
#include "mock_effect.hh"

/// Override the macro defined in track_entry.hh for stubbed functionallity
#define NOTE_FACTORY_NEW new MockNote
/// Override the macro defined in track_entry.hh for stubbed functionallity
#define NOTE_FACTORY_DELETE(note) delete *(note)

/// Override the macro defined in track_entry.hh for stubbed functionallity
#define EFFECT_FACTORY_NEW new MockEffect
/// Override the macro defined in track_entry.hh for stubbed functionallity
#define EFFECT_FACTORY_DELETE(effect) delete *(effect)

#include "track_entry.hh"

#include "mock_track_entry.hh"
#include "mock_track.hh"


/**
 * @test TrackEntry constructors
 *
 * Make sure that the constructor passes the track-, notes- and effects
 * objects to internal storage untouched.
 */
test_case(TrackEntry, Constructors) {
  MockTrack track;
  MockNotes notes;
  MockEffects effects;
  TrackEntry track_entry(&notes, &effects, &track);

  assert_eq(&notes, track_entry.notes);
  assert_eq(&effects, track_entry.effects);
  assert_eq(&track, track_entry.track);
}


/**
 * @test TrackEntry getters.
 *
 * Make sure that the notes-, effects- and track-object references ar possible
 * to be fetched correctly.
 */
test_case(TrackEntry, Getters) {
  MockTrack track;
  MockNotes notes;
  MockEffects effects;
  TrackEntry track_entry(&notes, &effects, &track);

  assert_eq(&notes, track_entry.get_notes());
  assert_eq(&effects, track_entry.get_effects());
  assert_eq(&track, track_entry.get_track());
}


/**
 * @test TrackEntryFactory creates valid track and delete it.
 *
 * Make sure that the TrackEntryFactory is able to create a valid track entry,
 * both with notes- and effects-list reference arguments and without, always
 * matching the track template. Also make sure that the user is informed if
 * something went wrong via the standard error output.
 */
test_case(TrackEntry, Factory) {
  const unsigned int NOTES = 2;
  const unsigned int EFFECTS = 3;

  // Create a mock-up of the track making it return known values.
  MockTrack track;
  expect_call_times_will_return(track, get_notes(), 3, NOTES);
  expect_call_times_will_return(track, get_effects(), 3, EFFECTS);

  // Normal operations for createing new entries based on the track.
  TrackEntry* track_entry = TrackEntryFactoryMock::New(&track);
  assert_eq(NOTES, dynamic_cast<MockNotes*>(track_entry->notes)->size());
  assert_eq(EFFECTS, dynamic_cast<MockEffects*>(track_entry->effects)->size());

  // Destroy the track entry again.
  TrackEntryFactoryMock::Delete(&track_entry);
  assert_eq(NULL, track_entry);

  // Try again but with mismatching track and notes length.
  MockNotes notes;
  for (unsigned int i = 0; i < NOTES - 1; i++) { notes.push_back(new MockNote()); }

  assert_stderr_eq("ERROR: The notes list is not the same length as track specifies.\n", track_entry = TrackEntryFactoryMock::New(&track, &notes));
  assert_eq(NULL, track_entry);

  // Try again but with mismatching track and effects length.
  notes.push_back(new MockNote()); // Add the missing note.
  MockEffects effects;
  for (unsigned int i = 0; i < EFFECTS - 1; i++) { effects.push_back(new MockEffect()); }

  assert_stderr_eq("ERROR: The effects list is not the same length as track specifies.\n", track_entry = TrackEntryFactoryMock::New(&track, &notes, &effects));
  assert_eq(NULL, track_entry);

  // Clean-up mocks.
  for (unsigned int i = 0; i < NOTES; i++) { delete notes.back(); notes.pop_back(); }
  for (unsigned int i = 0; i < EFFECTS - 1; i++) {
    delete effects.back();
    effects.pop_back();
  }
}
