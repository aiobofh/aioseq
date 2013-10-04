/**
 * @file AbstractInstrument.h
 *
 * Pure virtual interface for an instrument of som sort.
 */

#ifndef _ABSTRACT_INSTRUMENT_H_
#define _ABSTRACT_INSTRUMENT_H_

#include <string>

using std::string;

/**
 * Pure virtual interface for instrument implementations.
 */
class AbstractInstrument {

public:

  /**
   * Dummy constructor.
   */
  AbstractInstrument() {}; // Mock

  /**
   * Constructor with arguments that construct an
   * instrument with the specified name.
   *
   * @param name Name of the instrument.
   */
  AbstractInstrument(string *name) {}; // Mock

};

#endif
