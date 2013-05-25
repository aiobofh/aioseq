/**
 * @file PatternRowTest.cpp
 *
 * Test cases for the PatternRow implementation.
 */

#include <gtest/gtest.h>

#include "PatternRow.h"

#include "AbstractChannelRow.h"

#include "MockAbstractChannelRow.h"

using std::list;


/**
 * @test Constructor shall create a pattern row.
 */
TEST(PatternRow, Constructor_shall_create_a_pattern_row) {
  list<AbstractChannelRow*> channel_rows;

  PatternRow *pattern_row= new PatternRow(&channel_rows);

  ASSERT_FALSE(NULL == pattern_row);
}


/**
 * @test The get_channel_rows method shall return the list of channel rows in
 *       the pattern row.
 */
TEST(PatternRow, Get_channel_rows_shall_return_the_list_of_channel_rows_in_the_pattern_row) {
  list<AbstractChannelRow*> *channel_rows_got = NULL;
  list<AbstractChannelRow*> channel_rows;

  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));
  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));
  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));

  PatternRow pattern_row(&channel_rows);

  channel_rows_got = pattern_row.get_channel_rows();

  ASSERT_FALSE(NULL != channel_rows_got);
}
