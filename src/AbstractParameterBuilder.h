/**
 * @file AbstractParameterBuilder.h
 *
 * Interface for a parameter factory.
 */

#ifndef _ABSTRACT_PARAMETER_BUILDER_H_
#define _ABSTRACT_PARAMETER_BUILDER_H_

#include "AbstractParameter.h"

// Mock: template<class PARAMETER_BUILDER_CLASS>
/**
 * Parameter object factory class.
 */
class AbstractParameterBuilder {

public:

  /**
   * Dummy constructor.
   */
  AbstractParameterBuilder() {};


  /**
   * Create a new Parameter object.
   *
   * @return A pointer to the new object.
   */
  virtual AbstractParameter *create() = 0;


  /**
   * Destroy a Parameter object.
   *
   * @param parameter Pointer to a pointer referencing the parameter object
   *        to destroy.
   */
  virtual void destroy(AbstractParameter **parameter) = 0;

};

#endif
