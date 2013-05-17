/**
 * Parameterr.h
 *
 * The interface declaration for the Parameter class.
 */

#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include <gtest/gtest_prod.h>

#include "AbstractParameter.h"

/**
 * Represent a parameter to a channel.
 */
class Parameter : public AbstractParameter {

private:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Parameter, Constructor_without_arguments_shall_set_type_and_value_to_zero);
  FRIEND_TEST(Parameter, Constructor_shall_set_type_and_value);
  FRIEND_TEST(Paramater, Type_setter_shall_set_type);
  FRIEND_TEST(Parameter, Value_setter_shall_set_value);

  /**
   * Parameter type, could be effect number or similar.
   */
  int type;

  /**
   * Value for the specified parameter type.
   */
  int value;

public:

  /**
   * Constructor with no parameters will produce a useless parameter.
   */
  Parameter();

  /**
   * Constructor with arguments.
   *
   * @param type Parameter type, could be effect number or similar.
   * @param value Value for the specified parameter type.
   *
   */
  Parameter(int type, int value);

  /**
   * Set the parameter type of a Parameter.
   *
   * @param type The parameter type to set the Parameter as.
   */
  void set_type(int type);

  /**
   * Get the parameter type of a Parameter.
   *
   * @return The parameter type of the Parameter.
   */
  int get_type();

  /**
   * Set the value of a parameter.
   *
   * @param value The value of the Parameter.
   */
  void set_value(int value);

  /**
   * Get the value of a parameter.
   *
   * @return The value of a parameter.
   */
  int get_value();
};

#endif
