/**
 * @file AbstractPatterns.h
 *
 * List of abstract patterns.
 */

#ifndef _ABSTRACT_PATTERNS_H_
#define _ABSTRACT_PATTERNS_H_

#include "AbstractPattern.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointers to patterns.
 */
class AbstractPatterns : public list<AbstractPattern*> {
public:
  AbstractPatterns() : list<AbstractPattern*>() {};
};

#endif
