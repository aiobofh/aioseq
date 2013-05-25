/**
 * @file AbstractPattern.h
 *
 * Interface for pattern objects.
 */

#ifndef _ABSTRACT_PATTERN_H_
#define _ABSTRACT_PATTERN_H_

#include <list>

#include "AbstractPatternRow.h"

using std::list;

/**
 * Represent a pattern.
 */
class AbstractPattern {

protected:

  /**
   * A pointer to a std::list of pointers to PatternRow objects.
   */
  list<AbstractPatternRow*> *rows;

public:

  /**
   * Dummy constructor.
   */
  AbstractPattern() {};


  /**
   * Constructor with argument.
   *
   * @param rows @copydoc AbstractPattern::rows
   */
  AbstractPattern(list<AbstractPatternRow*> *rows) {};


  /**
   * Get a pointer to the list of pattern rows within the pattern object.
   *
   * @return @copydoc AbstractPattern::rows
   */
  virtual list<AbstractPatternRow*> *get_pattern_rows() = 0;

};

#endif
