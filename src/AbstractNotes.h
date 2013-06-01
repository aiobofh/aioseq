/**
 * @file AbstractNotes.h
 */

#ifndef _ABSTRACT_NOTES_H_
#define _ABSTRACT_NOTES_H_

#include "AbstractNote.h"

#include <list>

using std::list;

class AbstractNotes : public list<AbstractNote*> {
public:
  AbstractNotes() : list<AbstractNote*>() {}
};

#endif
