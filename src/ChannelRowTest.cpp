/**
 * @file ChannelRowTest.cpp
 *
 * Test casese for the ChannelRow implementation.
 */

#include <gtest/gtest.h>

#include "ChannelRow.h"

#include "MockAbstractNote.h"
#include "MockAbstractParameter.h"

using std::list;

/**
 * @test The constructor shall create lists with correct size.
 */
TEST(ChannelRow, Constructor_shall_have_set_the_notes_and_parameters) {
  list<AbstractNote*> *notes = NULL;
  list<AbstractParameter*> *parameters = NULL;

  ChannelRow channel_row(notes, parameters);

  notes++;
  parameters++;

  ASSERT_EQ(NULL, channel_row.notes);
  ASSERT_EQ(NULL, channel_row.parameters);
}

