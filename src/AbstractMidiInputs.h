/**
 * @file AbstractMidiInputs.h
 *
 * List of abstract midi inputs.
 */

#ifndef _ABSTRACT_MIDI_INPUTS_H_
#define _ABSTRACT_MIDI_INPUTS_H_

#include "AbstractMidiInput.h"

using std::list;

/**
 * List wrapper class to store pointers to MIDI inputs.
 */
class AbstractMidiInputs : public list<AbstractMidiInput*> {

public:

 AbstractMidiInputs() : list<AbstractMidiInput*>() {};

};

#endif
