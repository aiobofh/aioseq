/**
 * @file ParameterBuilder.h
 *
 * Interface for a Parameter factory.
 */

#ifndef _PARAMETER_BUILDER_H_
#define _PARAMETER_BUILDER_H_

#include "AbstractParameter.h"

template<class PARAMETER_CLASS>
/**
 * Parameter object factory class.
 */
class ParameterBuilder {
public:
  /**
   * Create a new Parameter object.
   *
   * @return A pointer to the new object.
   */
  PARAMETER_CLASS *create() {
    return new PARAMETER_CLASS();
  }

  /**
   * Destroy a Parameter object.
   *
   * @param parameter Pointer to the Parameter object to destroy.
   */
  void destroy(PARAMETER_CLASS *parameter) {
    delete parameter;
  }
};

#endif
