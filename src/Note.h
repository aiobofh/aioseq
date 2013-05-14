/**
 * @file Note.h
 *
 * The interface declaration for the Note class.
 *
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
   * Constructor wit no arguments will result in a C-0 with velocity 0.
   */
  Note();
  Note(int note, int velocity);
  void set_note(int velocity);
  int get_note();
  void set_velocity(int velocity);
  int get_velocity();
};
