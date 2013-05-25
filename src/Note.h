/**
 * @file Note.h
 *
 * The interface declaration for the Note class.
 *
 * @copydetails AbstractNote.h
 *
 */

#ifndef _NOTE_H_
#define _NOTE_H_

#include <gtest/gtest_prod.h>

#include "AbstractNote.h"


/**
 * Represent a note event according to the constraints of AbstractNote.
 */
class Note : public AbstractNote {

private:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Note, Constructor_without_arguments_shall_set_note_and_velocity_to_zero);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Note, Constructor_shall_set_note_number_and_velocity);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Note, Note_setter_shall_set_note);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Note, Velocity_setter_shall_set_velocity);


  /**
   * @copydoc AbstractNote::note
   */
  int note;


  /**
   * @copydoc AbstractNote::velocity
   */
  int velocity;

public:

  /**
   * @copydoc AbstractNote::AbstractNote()
   */
  Note();


  /**
   * @copydoc AbstractNote::AbstractNote(int,int)
   */
  Note(int note, int velocity);


  /**
   * @copydoc AbstractNote::set_note(int)
   */
  void set_note(int note);


  /**
   * @copydoc AbstractNote::get_note()
   */
  int get_note();


  /**
   * @copydoc AbstractNote::set_velocity(int)
   */
  void set_velocity(int velocity);


  /**
   * @copydoc AbstractNote::get_velocity()
   */
  int get_velocity();
};

#endif
