/**
 * @file Pattern.h
 *
 * @copydoc AbstractPattern.h
 */

#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <gtest/gtest_prod.h>

#include <list>

#include "AbstractPattern.h"

#include "AbstractPatternRow.h"

#include "AbstractPatternRows.h"

using std::list;

/**
 * @copydoc AbstractPattern
 */
class Pattern : public AbstractPattern {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Pattern, Constructor_shall_have_set_the_rows);

private:

  /**
   * A pointer to a std::list of pointers to PatternRow objects.
   */
  AbstractPatternRows *rows;

public:

  /**
   * @copydoc AbstractPattern::AbstractPattern(AbstractPatternRows*)
   */
  Pattern(AbstractPatternRows *rows);


  /**
   * @copydoc AbstractPattern::get_pattern_rows()
   */
  AbstractPatternRows *get_pattern_rows();

};

#endif
