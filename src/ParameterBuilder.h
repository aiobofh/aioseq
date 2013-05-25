/**
 * @file ParameterBuilder.h
 *
 * @copydoc AbstractParameterBuilder.h
 */

#ifndef _PARAMETER_BUILDER_H_
#define _PARAMETER_BUILDER_H_

#include <cstddef>

#include "Parameter.h"
#include "AbstractParameter.h"

#include "AbstractParameterBuilder.h"

template<class PARAMETER_CLASS>

/**
 * @copydoc AbstractParameterBuilder
 */
class ParameterBuilder : AbstractParameterBuilder {

public:

  /**
   * @copydoc AbstractParameterBuilder::create()
   */
  AbstractParameter *create() {
    return new PARAMETER_CLASS();
  }


  /**
   * @copydoc AbstractParameterBuilder::destory(AbstractParameter*)
   */
  void destroy(AbstractParameter **parameter) {
    delete *parameter;
    *parameter = NULL;
  }

};

#endif
