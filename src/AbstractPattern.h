/**
 * @file AbstractPattern.h
 *
 * Pure vitual class interface for representing a pattern.
 */

#ifndef _ABSTRACT_PATTERN_H_
#define _ABSTRACT_PATTERN_H_

#include <list>

#include "AbstractPatternRow.h"

using std::list;

/**
 * Pure virtual class to represent a Pattern.
 */
class AbstractPattern {
public:
  /**
   * Dummy constructor.
   */
  AbstractPattern() {};

  /**
   * Dummy constructor.
   *
   * @param rows Pointer to a std::list of pointers to PatternRow objects.
   */
  AbstractPattern(list<AbstractPatternRow*> *rows) {};
};

#endif
