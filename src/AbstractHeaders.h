/**
 * @file AbstractHeaders.h
 *
 * List of abstract headers.
 */

#ifndef _ABSTRACT_HEADERS_H_
#define _ABSTRACT_HEADERS_H_

#include "AbstractHeader.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointer to headers.
 */
class AbstractHeaders : public list<AbstractHeader*> {

public:

  AbstractHeaders() : list<AbstractHeader*>() {};

};

#endif
