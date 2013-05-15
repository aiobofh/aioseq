/**
 * @file ParameterTest.cpp
 *
 * Test cases for Patameter.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Parameter.h"

/**
 * @test A new Parameter without constructor parameters shall instantiate a
 *       Parameter with the type set to 0 and the value set to 0.
 */
TEST(Parameter, Constructor_without_arguments_shall_set_type_and_value_to_zero) {
  Parameter *parameter = new Parameter();
  ASSERT_EQ(parameter->type, 0);
  ASSERT_EQ(parameter->value, 0);
  delete parameter;
}

/**
 * @test A new Parameter with constructor arguments shall set a parameter type
 *       and parameter value accordingly.
 */
TEST(Parameter, Constructor_shall_set_type_and_value) {
  Parameter *parameter = new Parameter(10, 14);
  ASSERT_EQ(parameter->type, 10);
  ASSERT_EQ(parameter->value, 14);
}

/**
 * @test set_type shall set the parameter type of a Parameter.
 */
TEST(Paramater, Type_setter_shall_set_type) {
  Parameter *parameter = new Parameter();
  parameter->set_type(10);
  ASSERT_EQ(parameter->type, 10);
  delete parameter;
}

/**
 * @test set_value shall set the parameter value of a Parameter.
 */
TEST(Parameter, Value_setter_shall_set_value) {
  Parameter *parameter = new Parameter();
  parameter->set_value(10);
  ASSERT_EQ(parameter->value, 10);
  delete parameter;
}

/**
 * @test get_type shall get the parameter type of a Parameter.
 */
TEST(Parameter, Type_getter_shall_get_type) {
  Parameter *parameter = new Parameter(10, 14);
  ASSERT_EQ(parameter->get_type(), 10);
  delete parameter;
}

/**
 * @test get_value shall get the parameter value of a Parameter.
 */
TEST(Parameter, Value_getter_shall_get_value) {
  Parameter *parameter = new Parameter(10, 14);
  ASSERT_EQ(parameter->get_value(), 14);
  delete parameter;
}
