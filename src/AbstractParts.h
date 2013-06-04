/**
 * AbstractParts.h
 *
 * List of abstract parts.
 */

#ifndef _ABSTRACT_PARTS_H_
#define _ABSTRACT_PARTS_H_

#include "AbstractPart.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointers to parts.
 */
class AbstractParts : public list<AbstractPart*> {
public:
  AbstractParts() : list<AbstractPart*>() {};
};

#endif
