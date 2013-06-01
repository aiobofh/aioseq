/**
 * @file AbstractParameters.h
 */

#ifndef _ABSTRACT_PARAMETERS_H_
#define _ABSTRACT_PARAMETERS_H_

#include "AbstractParameter.h"

#include <list>

using std::list;

class AbstractParameters : public list<AbstractParameter*> {
public:
  AbstractParameters() : list<AbstractParameter*>() {};
};

#endif
