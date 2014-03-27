/**
 * @file track_test.cc
 *
 * Test-suite for the Track class.
 *
 */

#include "test.hh"

/**
 * Make all relevant test cases friends with the TrackEntry class to be able to
 * inspect the protected or private variables.
 */
#define TRACK_FRIENDS                           \
  friend_test(Track, Constructor)               \
  friend_test(Track, SettersAndGetters)         \
  friend_test(Track, Factory)                   \

#include "track.hh"

#include "mock_track.hh"


/**
 * @test Track constructor.
 *
 * Make sure that the constructor pass number of notes, effect, and the
 * track name string are passed to the internal storage untouched.
 */
test_case(Track, Constructor) {
  string name("Foobar");
  Track track(1, 1, name);
  assert_eq(1, track.notes);
  assert_eq(1, track.effects);
  assert_eq(name, track.name);
}


/**
 * @test Track setters and getters.
 *
 * Make sure that the number of notes, effects, and the name string are
 * possible to be set or fetched correctly.
 */
test_case(Track, SettersAndGetters) {
  string name("Foobar");
  Track track(1, 1, name);
  string name_foo("Foo");
  string name_bar("Bar");

  assert_setter_and_getter_twice_eq(1, 2, track, notes);
  assert_setter_and_getter_twice_eq(3, 4, track, effects);
  assert_setter_and_getter_twice_eq(name_foo, name_bar, track, name);
}


/**
 * @test TrackFactory creates valid track.
 *
 * Make sure that the TrackFactory is able to create a valid track, both with
 * no arguments (setting default values) and with arguments, setting them
 * to the track.
 */
test_case(Track, Factory) {
  Track *track = TrackFactoryMock::New();
  assert_eq(1, track->notes);
  assert_eq(1, track->effects);
  assert_eq("New track", track->name);

  TrackFactoryMock::Delete(&track);
  assert_eq(NULL, track);
}
