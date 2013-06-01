/**
 * @file AbstractParameters.h
 */

#ifndef _ABSTRACT_PARAMETERS_H_
#define _ABSTRACT_PARAMETERS_H_

#include "AbstractParameter.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointers to parameters.
 */
class AbstractParameters : public list<AbstractParameter*> {
public:
  AbstractParameters() : list<AbstractParameter*>() {};
};

#endif
