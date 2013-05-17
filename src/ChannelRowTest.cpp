/**
 * @file ChannelRowTest.cpp
 *
 * Test casese for the ChannelRow implementation.
 */

#include <gtest/gtest.h>

#include "ChannelRow.h"

#include "NoteBuilder.h"
#include "ParameterBuilder.h"
#include "MockAbstractNote.h"
#include "MockAbstractParameter.h"

using std::list;

/**
 * @test The constructor shall create lists with correct size.
 */
TEST(ChannelRow, Constructor_shall_create_lists_with_correct_size) {
  list<AbstractNote*> notes;
  list<AbstractParameter*> parameters;

  NoteBuilder<MockAbstractNote> note_builder;
  ParameterBuilder<MockAbstractParameter> parameter_builder;

  notes.push_back(note_builder.create());
  parameters.push_back(parameter_builder.create());

  ChannelRow channel_row(&notes, &parameters);

  ASSERT_EQ(channel_row.notes->size(), 1);
  ASSERT_EQ(channel_row.parameters->size(), 1);
}
