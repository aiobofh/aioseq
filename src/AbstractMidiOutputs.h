/**
 * @file AbstractMidiOutputs.h
 *
 * List of abstract midi inputs.
 */

#ifndef _ABSTRACT_MIDI_OUTPUTS_H_
#define _ABSTRACT_MIDI_OUTPUTS_H_

#include "AbstractMidiOutput.h"

using std::list;

/**
 * List wrapper class to store pointers to MIDI inputs.
 */
class AbstractMidiOutputs : public list<AbstractMidiOutput*> {

public:

 AbstractMidiOutputs() : list<AbstractMidiOutput*>() {};

};

#endif
