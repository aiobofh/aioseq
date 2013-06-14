/**
 * @file PatternTest.cpp
 *
 * Test sutie for the Pattern class.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Pattern.h"
#include "AbstractChannelRow.h"
#include "AbstractPatternRow.h"
#include "MockAbstractPatternRow.h"

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

/**
 * @test The get pattern rows shall return a reference to a list of pointers
 *       to pattern row objects.
 */
TEST(Pattern, Get_pattern_rows_shall_return_a_reference_to_a_list_of_pointers_to_patern_row_objects) {
  list<AbstractChannelRow*> *channel_rows; // Really not used for anything...
  list<AbstractPatternRow*> rows;

  list<AbstractPatternRow*> *rows_got;

  list<AbstractPatternRow*> *rows_before_break = &rows;

  Pattern pattern(&rows);

  ASSERT_EQ(rows_before_break, pattern.get_pattern_rows());
}

