/**
 * @file ChannelRowTest.cpp
 *
 * Test casese for the ChannelRow implementation.
 */

#include <gtest/gtest.h>

#include "ChannelRow.h"

#include "AbstractNote.h"
#include "AbstractNotes.h"
#include "AbstractParameter.h"
#include "AbstractParameters.h"

#include "MockAbstractNote.h"
#include "MockAbstractParameter.h"

using std::list;


/**
 * @test The constructor shall set the note and parameters.
 */
TEST(ChannelRow, Constructor_shall_have_set_the_notes_and_parameters) {
  AbstractNotes *notes = NULL;
  AbstractParameters *parameters = NULL;

  ChannelRow channel_row(notes, parameters);

  notes++;
  parameters++;

  ASSERT_EQ(NULL, channel_row.notes);
  ASSERT_EQ(NULL, channel_row.parameters);
}


/**
 * @test The get_notes method shall return the list of the notes in the
 *       channel row.
 */
TEST(ChannelRow, Get_notes_shall_return_the_list_of_notes_in_the_channel_row) {
  AbstractNotes *notes_got;
  AbstractNotes notes;
  AbstractParameters parameters;

  notes.push_back(new MockAbstractNote());
  notes.push_back(new MockAbstractNote());

  ChannelRow channel_row(&notes, &parameters);

  notes_got = channel_row.get_notes();

  ASSERT_EQ(*notes_got, notes);
}


/**
 * @test The get_parameters method shall return the list of the parameters in
 *       the channel row.
 */
TEST(ChannelRow, Get_parameters_shall_return_the_list_of_parameters_in_the_channel_row) {
  AbstractParameters *parameters_got;
  AbstractNotes notes;
  AbstractParameters parameters;

  parameters.push_back(new MockAbstractParameter());
  parameters.push_back(new MockAbstractParameter());

  ChannelRow channel_row(&notes, &parameters);

  parameters_got = channel_row.get_parameters();

  ASSERT_EQ(*parameters_got, parameters);
}
