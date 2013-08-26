/**
 * @file AbstractInstruments.h
 *
 * List of abstract instruments.
 */

#ifndef _ABSTRACT_INSTRUMENTS_H_
#define _ABSTRACT_INSTRUMENTS_H_

#include <list>

#include "AbstractInstrument.h"

using std::list;

/**
 * List wrapper class that store pointers to instruments.
 */
class AbstractInstruments : public list<AbstractInstruments*> {

public:

  AbstractInstruments() : list<AbstractInstruments*>() {};

};

#endif
