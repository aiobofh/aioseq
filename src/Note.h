/**
 * @file Note.h
 *
 * The interface declaration for the Note class.
 */

#ifndef _NOTE_H_
#define _NOTE_H_

#include <gtest/gtest_prod.h>

#include "AbstractNote.h"

/**
 * Represent a note event.
 */
class Note : public AbstractNote {

private:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Note, Constructor_without_arguments_shall_set_note_and_velocity_to_zero);
  FRIEND_TEST(Note, Constructor_shall_set_note_number_and_velocity);
  FRIEND_TEST(Note, Note_setter_shall_set_note);
  FRIEND_TEST(Note, Velocity_setter_shall_set_velocity);

  /**
   * Note number for the Note, a 0 is no note.
   */
  int note;

  /**
   * Velocity of which the note is hit or released on a keyboard.
   */
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

#endif
