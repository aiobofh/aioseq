/**
 * @file AbstractParameter.h
 *
 * Pure virtual interface for Parameter classes.
 */

#ifndef _ABSTRACT_PARAMETER_H_
#define _ABSTRACT_PARAMETER_H_

/**
 * Represent a parameter to a channel in a pure virtual fashion.
 */
class AbstractParameter {
public:
  /**
   * Dummy constructor.
   */
  AbstractParameter() {};

  /**
   * Dummy constructor.
   *
   * @param type Parameter type.
   * @param value Paramter value.
   */
  AbstractParameter(int type, int value) {};

  /**
   * Virtual method to set the paramter type.
   *
   * @param type Parameter type.
   */
  virtual void set_type(int type) = 0;

  /**
   * Virtual mehtod to get the parameter type.
   *
   * @return Parameeter type.
   */
  virtual int get_type() = 0;

  /**
   * Virtual method to set parameter value.
   *
   * @param value Parameter value.
   */
  virtual void set_value(int value) = 0;

  /**
   * Virtual method to get the parameter value.
   *
   * @return Parameter value.
   */
  virtual int get_value() = 0;
};

#endif
