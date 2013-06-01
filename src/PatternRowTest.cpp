/**
 * @file PatternRowTest.cpp
 *
 * Test cases for the PatternRow implementation.
 */

#include <gtest/gtest.h>

#include "PatternRow.h"

#include "AbstractChannelRows.h"

#include "MockAbstractChannelRow.h"

/**
 * @test Constructor shall create a pattern row.
 */
TEST(PatternRow, Constructor_shall_create_a_pattern_row) {
  AbstractChannelRows channel_rows;

  PatternRow *pattern_row= new PatternRow(&channel_rows);

  ASSERT_FALSE(NULL == pattern_row);
}


/**
 * @test The get_channel_rows method shall return the list of channel rows in
 *       the pattern row.
 */
TEST(PatternRow, Get_channel_rows_shall_return_the_list_of_channel_rows_in_the_pattern_row) {
  AbstractChannelRows *channel_rows_got = NULL;
  AbstractChannelRows channel_rows;

  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));
  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));
  channel_rows.push_back(new MockAbstractChannelRow(NULL, NULL));

  PatternRow pattern_row(&channel_rows);

  channel_rows_got = pattern_row.get_channel_rows();

  ASSERT_TRUE(NULL != channel_rows_got);
}
