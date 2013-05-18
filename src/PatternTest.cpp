/**
 * @file PatternTest.cpp
 *
 * Test sutie for the Pattern class.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Pattern.h"
#include "AbstractPatternRow.h"

using std::list;

/**
 * @test The constructor shall set the membervariable to keep track of rows.
 */
TEST(Pattern, Constructor_shall_have_set_the_rows) {
  list<AbstractPatternRow*> *rows = NULL;

  Pattern pattern(rows);

  rows++;

  ASSERT_EQ(NULL, pattern.rows);
}
