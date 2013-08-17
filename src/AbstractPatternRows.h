/**
 * @file AbstractPatternRows.h
 */

#ifndef _ABSTRACT_PATTERN_ROWS_H_
#define _ABSTRACT_PATTERN_ROWS_H_

#include "AbstractPatternRow.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointer to pattern rows.
 */
class AbstractPatternRows : public list<AbstractPatternRow*> {
public:
  AbstractPatternRows() : list<AbstractPatternRow*>() {};
};

#endif
