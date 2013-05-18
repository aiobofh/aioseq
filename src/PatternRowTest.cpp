/**
 * @file PatternRowTest.cpp
 *
 * Test cases for the PatternRow implementation.
 */

#include <gtest/gtest.h>

#include "PatternRow.h"

#include "ChannelRowBuilder.h"

#include "MockAbstractChannelRow.h"

using std::list;

/**
 * @test PatternRow constructor shall keep track of all channels.
 */
TEST(PatternRow, Constructor_shall_create_all_channels) {
  list<AbstractChannelRow*> channels;

  channels.push_back(new MockAbstractChannelRow);

  PatternRow pattern_row(&channels);

  ASSERT_EQ(pattern_row.channels->size(), 1);
  
}
