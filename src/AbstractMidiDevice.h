/**
 * @file AbstractMidiDevice.h
 *
 * Pure virtual interface for a MIDI device of some sort.
 */

#ifndef _ABSTRACT_MIDI_DEVICE_H_
#define _ABSTRACT_MIDI_DEVICE_H_

#include <string>

#include "AbstractMidiInput.h" // Include in mock
#include "AbstractMidiOutput.h" // Include in mock
#include "AbstractAudioInputs.h" // Include in mock
#include "AbstractAudioOutputs.h" // Include in mock

/**
 * Pure virtual interface for MIDI device implementations.
 */
class AbstractMidiDevice {

public:

  /**
   * Dummy constructor.
   */
  AbstractMidiDevice() {}; // Mock

  /**
   * Constructor with arguments that construct a MIDI device with the
   * specified name, inputs and outputs.
   *
   * @param name Name of the MIDI device.
   * @param midi_input Pointer to a MIDI input to use for this device.
   * @param midi_output Pointer to a MIDI output to use for this device.
   * @param audio_inputs Pointer to a list of audio inputs from this device.
   * @param audio_outputs Pointer to a list fo audio outputs to this device.
   */
  AbstractMidiDevice(const string *name, // Mock
                     AbstractMidiInput *midi_input, // Mock
                     AbstractMidiOutput *midi_output, // Mock
                     AbstractAudioInputs *audio_inputs, // Mock
                     AbstractAudioOutputs *audio_outputs); // Mock

};

#endif
