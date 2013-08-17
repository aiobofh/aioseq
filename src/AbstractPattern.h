/**
 * @file AbstractPattern.h
 *
 * Interface for pattern objects.
 */

#ifndef _ABSTRACT_PATTERN_H_
#define _ABSTRACT_PATTERN_H_

#include <list>

#include "AbstractPatternRow.h"

#include "AbstractPatternRows.h" // Include in mock.

using std::list;

/**
 * Represent a pattern.
 */
class AbstractPattern {

public:

  /**
   * Dummy constructor.
   */
  AbstractPattern() {};


  /**
   * Constructor with argument.
   *
   * @param rows A pointer to a std::list of pointers to PatternRow objects.
   */
  AbstractPattern(AbstractPatternRows *rows) {};


  /**
   * Get a pointer to the list of pattern rows within the pattern object.
   *
   * @return A pointer to a list of pointers to PatternRow objects.
   */
  virtual AbstractPatternRows *get_pattern_rows() = 0;

};

#endif
