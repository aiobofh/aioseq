/**
 * @file AbstractNote.h
 *
 * Pure virtual interface for Note classes.
 */

#ifndef _ABSTRACT_NOTE_H_
#define _ABSTRACT_NOTE_H_

/**
 * Represent a note event in a pure virtual fashion.
 */
class AbstractNote {
public:
  /**
   * Dummy constructor.
   */
  AbstractNote() {};

  /**
   * Dummy constructor.
   *
   * @param note Note value.
   * @param velocity Velocity valye.
   */
  AbstractNote(int note, int velocity) {};

  /**
   * Virtual method to set a note value.
   *
   * @param note Note value to set.
   */
  virtual void set_note(int note) = 0;

  /**
   * Virtual method to get a note value.
   *
   * @return Note value.
   */
  virtual int get_note() = 0;

  /**
   * Virtual method to set the velocity of the Note.
   *
   * @param velocity Velocity value.
   */
  virtual void set_velocity(int velocity) = 0;

  /**
   * Virtual method to get the velocity of the Note.
   *
   * @return The velocity value.
   */
  virtual int get_velocity() = 0;
};

#endif
