/**
 * PartBuilderTest.cpp
 *
 * Test cases for the PartBuilder class.
 */

#include <gtest/gtest.h>

#include "PartBuilder.h"
#include "Part.h"
#include "AbstractPart.h"
#include "MockAbstractPart.h"
#include "MockAbstractPatterns.h"

using ::testing::Mock;
using ::testing::_;

TEST(PartBuilderTest, Create_method_shall_create_a_part) {

  string *name = new string("foobar");

  AbstractPart *part;

  /**
   * Set-up the system under test using mocks inside.
   */
  PartBuilder<MockAbstractPart> part_builder;

  /**
   * Call the design under test.
   */
  part = part_builder.create(name);

  /**
   * A part shall be created.
   */
  ASSERT_FALSE(NULL == part);

}


TEST(PartBuilderTest, Destroy_method_shall_destroy_a_part) {

  /**
   * Set-up the system under test.
   */
  PartBuilder<MockAbstractPart> part_builder;

  /**
   * Create an empty list of patterns (as a mock).
   */
  AbstractPatterns *patterns = new AbstractPatterns();

  /**
   * Create a new part.
   */
  AbstractPart *part = new Part(patterns);

  /**
   * Call the design under test.
   */
  part_builder.destroy(&part);

  /**
   * Make sure that the destroy method sets the referenced part pointer to
   * NULL.
   */
  ASSERT_EQ(NULL, part);

}
