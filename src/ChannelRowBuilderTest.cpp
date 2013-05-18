/**
 * @file ChannelRowBuilderTest.cpp
 *
 * Test cases for the ChannelRow object factory ChannelRowBuilder.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractNote.h"
#include "MockAbstractParameter.h"
#include "MockAbstractChannelRow.h"

#include "ChannelRowBuilder.h"

/**
 * This class is used to make the mock accept constructor arguments correctly.
 */
class MockAbstractChannelRowCompatibleConstructor : MockAbstractChannelRow {
public:
  /**
   * ChannelRow compatible constructor in this test context.
   *
   * @param notes Pointer to a list of MockAbstractNote objects.
   * @param parameters Pointer to a list of MockAbstractParameter objects.
   */
  MockAbstractChannelRowCompatibleConstructor(list<MockAbstractNote*> *notes,
					      list<MockAbstractParameter*> *parameters) : MockAbstractChannelRow() {};
};

/**
 * ChannelRowBuild instance with testable properties.
 */
static ChannelRowBuilder<MockAbstractChannelRowCompatibleConstructor,
			 MockAbstractNote,
			 MockAbstractParameter> channel_row_mock_builder;
/**
 * ChannelRowBuild instance with testable properties.
 */
static ChannelRowBuilder<MockAbstractChannelRow,
			 MockAbstractNote,
			 MockAbstractParameter> channel_row_builder;

/**
 * @test Create method shall create a ChannelRow object.
 */
TEST(ChannelRowBuilder, Create_method_shall_create_a_channel_row) {
  /**
   * Declare a pointer to the specialised mock.
   */
  MockAbstractChannelRowCompatibleConstructor *channel_row;

  /**
   * Use the factory to create a new ChannelRow object variant.
   */
  channel_row = channel_row_mock_builder.create(4, 5);

  ASSERT_FALSE(NULL == channel_row);
}

/**
 * @test Destroy method shall destroy a ChannelRow object.
 */
TEST(ChannelRowBuilder, Destroy_method_shall_destroy_a_channel_row) {
  MockAbstractChannelRow *channel_row = new MockAbstractChannelRow();
  EXPECT_CALL(*channel_row, Die()).Times(1);
  channel_row_builder.destroy(channel_row);
}
