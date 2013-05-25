/**
 * @file AbstractNoteBuilder.h
 *
 * Interface for a note factory.
 */

#ifndef _ABSTRACT_NOTE_BUILDER_H_
#define _ABSTRACT_NOTE_BUILDER_H_

#include "AbstractNote.h"

// Mock: template<class NOTE_CLASS>
/**
 * Note object factory class.
 */
class AbstractNoteBuilder {

public:

  /**
   * Dummy constructor.
   */
  AbstractNoteBuilder() {};


  /**
   * Create a new note object.
   *
   * @return A pointer to the new object.
   */
  virtual AbstractNote *create() = 0;


  /**
   * Destroy a note object.
   *
   * @param note Pointer to a pointer refrencing the note object to destroy.
   */
  virtual void destroy(AbstractNote **note) = 0;

};

#endif
