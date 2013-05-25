/**
 * @file NoteBuilder.h
 *
 * @copydoc AbstractNoteBuilder.h
 */

#ifndef _NOTE_BUILDER_H_
#define _NOTE_BUILDER_H_

#include <cstddef>

#include "Note.h"
#include "AbstractNote.h"

#include "AbstractNoteBuilder.h"

template<class NOTE_CLASS>

/**
 * @copydoc AbstractNoteBuilder
 */
class NoteBuilder : AbstractNoteBuilder {

public:

  /**
   * @copydoc AbstractNoteBuilder::create()
   */
  AbstractNote *create() {
    return new NOTE_CLASS();
  }


  /**
   * @copydoc AbstractNoteBuilder::destroy(AbstractNote*)
   */
  void destroy(AbstractNote **note) {
    delete *note;
    *note = NULL;
  }

};

#endif
