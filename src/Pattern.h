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

using std::list;

/**
 * @copydoc AbstractPattern
 */
class Pattern : public AbstractPattern {

protected:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Pattern, Constructor_shall_have_set_the_rows);

public:

  /**
   * @copydoc AbstractPattern::AbstractPattern(list<AbstractPatternRow*>*)
   */
  Pattern(list<AbstractPatternRow*> *rows);


  /**
   * @copydoc AbstractPattern::get_pattern_rows()
   */
  list<AbstractPatternRow*> *get_pattern_rows();

};

#endif
