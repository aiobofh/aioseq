/**
 * @file Pattern.h
 */

#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <gtest/gtest_prod.h>

#include <list>

#include "AbstractPattern.h"

using std::list;

/**
 * Represent a pattern.
 */
class Pattern : public AbstractPattern {
private:
  FRIEND_TEST(Pattern, Constructor_shall_have_set_the_rows);
  /**
   * Pointer to a std::list of pointers to PatternRow objects.
   */
  list<AbstractPatternRow*> *rows;
public:
  /**
   * Constructor which just assigns rows to the new object.
   *
   * @param rows Pointer to a std::list of PatternRow objects.
   */
  Pattern(list<AbstractPatternRow*> *rows);
};

#endif
