/**
 * @file NoteBuilder.h
 *
 * Interface for a Note factory.
 */

#ifndef _NOTE_BUILDER_H_
#define _NOTE_BUILDER_H_

#include "AbstractNote.h"

template<class NOTE_CLASS>
/**
 * Note object factory class.
 */
class NoteBuilder {
public:
  /**
   * Create a new Note object.
   *
   * @return A pointer to the new object.
   */
  AbstractNote *create() {
    return new NOTE_CLASS();
  }

  /**
   * Destrou a Note object.
   *
   * @param note Pointer to the Note object to destroy.
   */
  void destroy(AbstractNote *note) {
    delete note;
  }
};

#endif
