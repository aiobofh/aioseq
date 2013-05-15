/**
 * @file Note.h
 *
 * The interface declaration for the Note class.
 */

#include <gtest/gtest_prod.h>

/**
 * Represent a note event.
 */
class Note {

private:
  FRIEND_TEST(Note, Constructor_without_arguments_shall_set_note_and_velocity_to_zero);
  FRIEND_TEST(Note, Constructor_shall_set_note_number_and_velocity);
  FRIEND_TEST(Note, Note_setter_shall_set_note);
  FRIEND_TEST(Note, Velocity_setter_shall_set_velocity);
  int note;
  int velocity;

public:
  /**
   * Constructor with no parameters will produce a non-note with no velocity.
   */
  Note();

  /**
   * Constructor with arguments.
   *
   * @param note Note number to initialise the new Note as.
   * @param velocity Velocity at which the note is hit (positive) or released
   *                 (negative).
   */
  Note(int note, int velocity);

  /**
   * Set the note number of a Note.
   *
   * @param note The note number to set the Note as.
   */
  void set_note(int note);

  /**
   * Get the note number of a Note.
   *
   * @return The note number of the Note.
   */
  int get_note();

  /**
   * Set the velocity at which the Note is hit on a keyboard.
   *
   * @param velocity A negative velocity is the velocity at which the note of
   *                 a keyboard was released, a positive velocity is the
   *                 velocity at which the note of a keyboard was hit.
   */
  void set_velocity(int velocity);

  /**
   * Get the velocity at which a Note is hit or released on a keyboard.
   *
   * @return A negative velocity is the velocity at which the note of
   *         a keyboard was released, a positive velocity is the velocity at
   *         which the note of a keyboard was hit.
   */
  int get_velocity();
};
