/**
 * @file note_test.cc
 *
 * Test-suite for the Note class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Note class to be able to
 * inspect the protected or private variables.
 */
#define NOTE_FRIENDS                    \
  friend_test(Note, Constructors)       \
  friend_test(Note, SettersAndGetters)  \
  friend_test(Note, Factory)            \

#include "note.hh"
#include "mock_note.hh"


/**
 * @test Note constructors.
 *
 * Make sure that the constructor sets the note and velocity to 0 if no
 * arguments are provided, otherwise it shall pass the note and velocity to the
 * internal storage untouched.
 */
test_case(Note, Constructors) {
  Note note1;

  assert_eq(0, note1.key);
  assert_eq(0, note1.velocity);

  Note note2(1, 2);

  assert_eq(1, note2.key);
  assert_eq(2, note2.velocity);
}


/**
 * @test Note setters and getters.
 *
 * Make sure that the note and velocity ar possible to be set or fetched
 * correctly.
 */
test_case(Note, SettersAndGetters) {
  Note note;

  assert_setter_and_getter_twice_eq(1, 3, note, key);
  assert_setter_and_getter_twice_eq(2, 4, note, velocity);
}


/**
 * @test NoteFactory creates valid notes and destroy them.
 *
 * Make sure that the NoteFactory is able to create a valid note, both with
 * no arguments (setting default values) and with arguments, setting them
 * to the note.
 */
test_case(Note, Factory) {
  Note* note1 = NoteFactory::New();
  assert_eq(0, note1->key);
  assert_eq(0, note1->velocity);
  Note* note2 = NoteFactory::New(1, 2);
  assert_eq(1, note2->key);
  assert_eq(2, note2->velocity);

  NoteFactory::Delete(&note1);
  assert_eq(NULL, note1);
  NoteFactory::Delete(&note2);
  assert_eq(NULL, note2);
}
