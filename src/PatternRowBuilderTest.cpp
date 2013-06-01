/**
 * @file PatternRowBuilder.cpp
 *
 * Test cases for the PatternRowBuilder class.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractChannelRowBuilder.h"
#include "MockAbstractChannelRow.h"
#include "MockAbstractPatternRow.h"
#include "MockAbstractNoteBuilder.h"
#include "MockAbstractNote.h"
#include "MockAbstractParameterBuilder.h"
#include "MockAbstractParameter.h"
#include "PatternRowBuilder.h"
#include "PatternRow.h"
#include "AbstractPatternRow.h"
#include "AbstractNotes.h"
#include "AbstractParameters.h"
#include "AbstractChannelRows.h"

using ::testing::Mock;
using ::testing::_;

/**
 * Create method shall create a pattern row.
 */
TEST(PatternRowBuilder, Create_method_shall_create_a_pattern_row) {
  /**
   * The note builder and parameter builder are sent as parameters to the
   * channel row builder constructor.
   */
  //  MockAbstractNoteBuilder<MockAbstractNote> note_builder;
  //  MockAbstractParameterBuilder<MockAbstractParameter> parameter_builder;

  /**
   * The channel row builder is sent as parameter to the pattern row
   * constructor.
   */
  //  MockAbstractChannelRowBuilder<MockAbstractChannelRow> channel_row_builder(&note_builder, &parameter_builder);
  MockAbstractChannelRowBuilder<MockAbstractChannelRow> channel_row_builder;

  /**
   * Instantiate the system under test.
   */
  PatternRowBuilder<MockAbstractPatternRow> pattern_row_builder(&channel_row_builder);

  AbstractPatternRow *pattern_row;

  /**
   * Since these objects are mocks the destructor shall be called at the end
   * of the test case.
   */
  //  EXPECT_CALL(note_builder, Die()).Times(1);
  //  EXPECT_CALL(parameter_builder, Die()).Times(1);
  EXPECT_CALL(channel_row_builder, Die()).Times(1);

  /**
   * The real logic inside the create method consiss of a call to the
   * create method of the channel row builder mock.
   */
  EXPECT_CALL(channel_row_builder, create(_,_)).Times(8);

  int foo[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  /**
   * Call the design under test.
   */
  pattern_row = pattern_row_builder.create(8, foo, foo);

  /**
   * Make sure that a pointer to the new object is returned.
   */
  ASSERT_FALSE(NULL == pattern_row);
}

TEST(PatternRowBuilder, Destroy_method_shall_destroy_a_pattern_row) {
  //  MockAbstractNoteBuilder<MockAbstractNote> note_builder;
  //  MockAbstractParameterBuilder<MockAbstractParameter> parameter_builder;
  //  MockAbstractChannelRowBuilder<MockAbstractChannelRow> channel_row_builder(&note_builder, &parameter_builder);
  MockAbstractChannelRowBuilder<MockAbstractChannelRow> channel_row_builder;

  AbstractNotes *notes = new AbstractNotes;
  AbstractParameters *parameters = new AbstractParameters;
  AbstractChannelRows *channel_rows = new AbstractChannelRows;

  /**
   * Instantiate the system under test, providing the list of channel rows.
   */
  PatternRowBuilder<MockAbstractPatternRow>
    pattern_row_builder(&channel_row_builder);

  /**
   * An existing pattern row "always" have some channel rows.
   */
  channel_rows->push_back(new MockAbstractChannelRow(notes, parameters));

  /**
   * Unfortunetly the pattern row can not be mocked in this test, cince the
   * destroy method makes use of the _real_ list inside the actual
   * pattern row object, hence an implementation of the pattern row must be
   * instantiated.
   */
  AbstractPatternRow *pattern_row = new PatternRow(channel_rows);
  AbstractPatternRow *abstract_pattern_row = pattern_row;

  /**
   * The channel row builder created earlier in this test set-up shall be
   * destroyed by the pattern row builder's destroy method.
   */
  EXPECT_CALL(channel_row_builder, destroy(_)).Times(1);

  /**
   * Since the note builder and the parameter builder are stack static
   * instantiated objects they are supposed to be deleted after the test case
   * so lets expect calls to their destructors.
   */
  EXPECT_CALL(channel_row_builder, Die()).Times(1);
  //  EXPECT_CALL(note_builder, Die()).Times(1);
  // EXPECT_CALL(parameter_builder, Die()).Times(1);

  /**
   * Call the design under test.
   */
  pattern_row_builder.destroy(&abstract_pattern_row);

  /**
   * Make sure that the object was deleted and NULL:ed
   */
  ASSERT_EQ(NULL, abstract_pattern_row);

  /**
   * Since the object is deleted out of context for Google Mock we need to
   * allow the memory leakage of the note mock.
   */
  Mock::AllowLeak(pattern_row);
}
