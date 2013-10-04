/**
 * @file AbstractMidiDevices.h
 *
 * List of abstract MIDI devices.
 */

#ifndef _ABSTRACT_MIDI_DEVICES_H_
#define _ABSTRACT_MIDI_DEVICES_H_

#include "AbstractMidiDevice.h"

using std::list;

/**
 * List wrapper class to store pointers to MIDI devices.
 */
class AbstractMidiDevices : public list<AbstractMidiDevice*> {

public:

  AbstractMidiDevices() : list<AbstractMidiDevice*>() {};

};

#endif
