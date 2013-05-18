/**
 * @file PatternRowBuilderTest.cpp
 *
 * Test sutie for the PatternRowBuilder factory.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractPatternRow.h"
#include "MockAbstractChannelRow.h"
#include "MockAbstractNote.h"
#include "MockAbstractParameter.h"

#include "PatternRowBuilder.h"

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
 * Thjis class is used to make the mock accept construcot arguments correctly.
 */
class MockAbstractPatternRowCompatibleConstructor : MockAbstractPatternRow {
public:
  /**
   * PatternRow comptaible constructor in this test context.
   *
   * @param channels Pointer to a list of MockAbstractChannelRowCompatibleConstruct
   *                 objects.
   */
  MockAbstractPatternRowCompatibleConstructor(list<MockAbstractChannelRowCompatibleConstructor*> *channels) {};
};


/**
 * PaternRowBuild instance with testable properties.
 */
static PatternRowBuilder<MockAbstractPatternRowCompatibleConstructor,
			 MockAbstractChannelRowCompatibleConstructor,
			 MockAbstractNote,
			 MockAbstractParameter> pattern_row_builder;

/**
 * @test The create method of the PatternRowBuilder shall create a PatternRow
 *       object.
 */
TEST(PatternRowBuilder, Create_method_shall_create_a_pattern_row) {
  MockAbstractPatternRowCompatibleConstructor *pattern_row;

  pattern_row = pattern_row_builder.create(8);

  ASSERT_FALSE(NULL == pattern_row);
}
