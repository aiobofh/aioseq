/**
 * @file ParameterBuilderTest.cpp
 *
 * Test cases for the ParameterBuilder object factory.
 */

#include <gtest/gtest.h>

#include "MockAbstractParameter.h"

#include "ParameterBuilder.h"

static ParameterBuilder<MockAbstractParameter> parameter_builder;

/**
 * @test Create method shall create a Parameter object.
 */
TEST(ParameterBuilder, Create_method_shall_create_a_parameter_object) {
  MockAbstractParameter *parameter;
  parameter = parameter_builder.create();
  ASSERT_FALSE(NULL == parameter);
}
