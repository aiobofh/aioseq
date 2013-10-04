/**
 * @file AbstractMidiOutput.h
 *
 * This is a pure virtual interface for an MIDI output.
 */

#ifndef _ABSTRACT_MIDI_OUTPUT_H_
#define _ABSTRACT_MIDI_OUTPUT_H_

#include <string>

using std::string;

/**
 * Pure virtual interface for MIDI output implementations.
 */
class AbstractMidiOutput {

public:

  /**
   * Dummy constructor.
   */
  AbstractMidiOutput() {};

  /**
   * Constructor with arguments that construct an output
   * with the specified name.
   *
   * @param name Name of the MIDI output.
   */
  AbstractMidiOutput(string *name) {};

};

#endif
