/**
 * @file AbstractMidiInput.h
 *
 * This is a pure virtual interface for an midi input.
 */

#ifndef _ABSTRACT_MIDI_INPUT_H_
#define _ABSTRACT_MIDI_INPUT_H_

#include <string>

using std::string;

/**
 * Pure virtual interface for midi input implementations.
 */
class AbstractMidiInput {

public:

  /**
   * Dummy constructor.
   */
  AbstractMidiInput() {};

  /**
   * Constructor with arguments that construct an input
   * with the specified name.
   *
   * @param name Name of the midi input.
   */
  AbstractMidiInput(string *name) {};

};

#endif
