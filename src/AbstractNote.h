/**
 * @file AbstractNote.h
 *
 * Interface for note objects.
 *
 * A note is a musical note representation of some sort all notes within this
 * design it contains to properties: @copydoc AbstractNote::note And the
 * second one: @copydoc AbstractNote::velocity
 *
 */

#ifndef _ABSTRACT_NOTE_H_
#define _ABSTRACT_NOTE_H_


/**
 * Represent a note event.
 */
class AbstractNote {

private:

  /**
   * A note number, where 0 (zero) is a C on the first octave (16.35Hz), and
   * so on.
   */
  int note;


  /**
   * A velocity at which the key representing the note on a musical instrument
   * is hit (positivt value) or released (negative value).
   */
  int velocity;

public:

  /**
   * Constructor with no parameters will produce a non-note (0) with no
   * velocity (0).
   */
  AbstractNote() : note(0), velocity(0) {};


  /**
   * Constructor with arguments that construct a note object with the
   * specified note number and velocity.
   *
   * @param note : @copydoc AbstractNote::note
   * @param velocity : Bar @copydoc AbstractNote::velocity
   */
  AbstractNote(int note, int velocity) : note(note), velocity(velocity) {};


  /**
   * Set the note number of a note object.
   *
   * @param note : @copydoc AbstractNote::note
   */
  virtual void set_note(int note) = 0;


  /**
   * Get the note number of a note object.
   *
   * @return : @copydoc AbstractNote::note
   */
  virtual int get_note() = 0;


  /**
   * Set the velocity at which the Note is hit on a keyboard.
   *
   * @param velocity : @copydoc AbstractNote::velocity
   */
  virtual void set_velocity(int velocity) = 0;


  /**
   * Get the velocity at which a Note is hit or released on a keyboard.
   *
   * @return : @copydoc AbstractNote::velocity
   */
  virtual int get_velocity() = 0;
};

#endif
