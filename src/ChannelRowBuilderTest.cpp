/**
 * @file ChannelRowBuilderTest.cpp
 *
 * Test cases for the ChannelRowBuilder class.
 */

#include <gtest/gtest.h>

#include "MockAbstractNote.h"
#include "AbstractNote.h"
#include "AbstractNotes.h"
#include "AbstractParameter.h"
#include "MockAbstractParameter.h"
#include "AbstractParameters.h"
#include "MockAbstractNoteBuilder.h"
#include "MockAbstractParameterBuilder.h"
#include "AbstractChannelRow.h"
#include "MockAbstractChannelRow.h"

#include "ChannelRow.h"
#include "ChannelRowBuilder.h"

using ::testing::Mock;
using ::testing::_;

/**
 * @test Create method shall create a channel row.
 */
TEST(ChannelRowBuilder, Create_method_shall_create_a_channel_row) {
  MockAbstractNoteBuilder<MockAbstractNote> note_builder;
  MockAbstractParameterBuilder<MockAbstractParameter> parameter_builder;

  ChannelRowBuilder<MockAbstractChannelRow>
    channel_row_builder(&note_builder, &parameter_builder);

  AbstractChannelRow *channel_row;

  /**
   * Since the note builder and the parameter builder will be deleted at the
   * end of the test case...
   */
  EXPECT_CALL(note_builder, Die()).Times(1);
  EXPECT_CALL(parameter_builder, Die()).Times(1);

  EXPECT_CALL(note_builder, create()).Times(4);
  EXPECT_CALL(parameter_builder, create()).Times(5);

  channel_row = channel_row_builder.create(4, 5);

  ASSERT_FALSE(NULL == channel_row);
}


/**
 * @test Destroy method shall destroy a channel row.
 *
 * This test emulates the create method part of the object factory, using
 * mocks instead of real objects and the calls the destroy method as design
 * under test. Validation is done for calls to the injected mocks to make
 * sure that the design under test is destroying all instantiated objects
 * in the correct way.
 */
TEST(ChannelRowBuilder, Destroy_method_shall_destroy_a_channel_row) {
  MockAbstractNoteBuilder<MockAbstractNote> note_builder;
  MockAbstractParameterBuilder<MockAbstractParameter> parameter_builder;

  AbstractNotes *notes = new AbstractNotes();
  AbstractParameters *parameters = new AbstractParameters();

  /**
   * Instantiate the system under test, providing the two lists.
   */
  ChannelRowBuilder<MockAbstractChannelRow>
    channel_row_builder(&note_builder,
                        &parameter_builder);

  /**
   * An existing channel row "always" have some notes and some parametrs.
   */
  notes->push_back(new MockAbstractNote());
  parameters->push_back(new MockAbstractParameter());

  /**
   * Unfortunately the channel row can not be mocked in this test, since
   * the destroy method make use of the _Real_ lists inside the actual
   * channel row object, hence an implementation of the channel row must
   * be instantiated.
   */
  AbstractChannelRow *channel_row = new ChannelRow(notes, parameters);
  AbstractChannelRow *abstract_channel_row = channel_row;

  /**
   * The note and the parameter created earlier in this test set-up shall be
   * destroyed by the channel row builder's destroy method.
   */
  EXPECT_CALL(note_builder, destroy(_)).Times(1);
  EXPECT_CALL(parameter_builder, destroy(_)).Times(1);

  /**
   * Since the note builder and the parameter builder are stack static
   * instantiated objects they are supposed to be deleted after the test case
   * so lets expect calls to their destructors.
   */
  EXPECT_CALL(note_builder, Die()).Times(1);
  EXPECT_CALL(parameter_builder, Die()).Times(1);

  /**
   * Call the design under test.
   */
  channel_row_builder.destroy(&abstract_channel_row);

  /**
   * Make sure that the object was deleted and NULL:ed.
   */
  ASSERT_EQ(NULL, abstract_channel_row);

  /**
   * Since the object is deleted out of context for Google Mock we need to
   * allow the memory leakage of the note mock.
   */
  Mock::AllowLeak(channel_row);
}
