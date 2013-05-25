/**
 * @file ParameterBuilderTest.cpp
 *
 * Test cases for the ParameterBuilder class.
 */

#include <gtest/gtest.h>

#include "ParameterBuilder.h"
#include "AbstractParameter.h"
#include "MockAbstractParameter.h"

using ::testing::Mock;

/**
 * @test Create method shall create a parameter object.
 */
TEST(ParameterBuilder, Create_method_shall_create_a_parameter_object) {
  ParameterBuilder<MockAbstractParameter> parameter_builder;
  AbstractParameter *parameter;

  parameter = parameter_builder.create();

  ASSERT_FALSE(NULL == parameter);
}


/**
 * @test Destroy method shall destroy a parameter object.
 */
TEST(ParameterBuilder, Destroy_method_shall_destroy_a_parameter_object) {
  ParameterBuilder<MockAbstractParameter> parameter_builder;
  MockAbstractParameter *parameter = new MockAbstractParameter();
  AbstractParameter *abstract_parameter = parameter;

  EXPECT_CALL(*parameter, Die()).Times(1);

  parameter_builder.destroy(&abstract_parameter);

  ASSERT_EQ(NULL, abstract_parameter);

  /**
   * Since the object is deleted out of context for Google Mock we need to
   * allow the memory leakage of the parameter mock.
   */
  Mock::AllowLeak(parameter);
}

