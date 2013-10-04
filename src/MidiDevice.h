/**
 * @file MidiDevice.h
 *
 * This is the interface declaration for the MidiDevice class.
 */

#ifndef _MIDI_DEVICE_H_
#define _MIDI_DEVICE_H_

#include <string>

#include <gtest/gtest_prod.h>

#include "Name.h"
#include "AbstractMidiDevice.h"
#include "AbstractMidiInput.h"
#include "AbstractMidiOutput.h"
#include "AbstractAudioInputs.h"
#include "AbstractAudioOutputs.h"

/**
 * The default MIDI device name used if nothing is provided.
 */
#define DEFAULT_MIDI_DEVICE_NAME "New MIDI device"

/**
 * Represent a MIDI device according to the constraints of AbstractMidiDevice.
 */
class MidiDevice : public AbstractMidiDevice, public Name {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(MidiDevice, Constructor_without_arguments_shall_create_a_MIDI_device_with_a_default_name_and_no_inputs_or_outputs);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(MidiDevice, Constructor_with_arguments_shall_create_a_MIDI_device_with_the_correct_contents);

private:

  /**
   * A pointer to the MIDI input to use for the MIDI device.
   */
  AbstractMidiInput *midi_input;

  /**
   * A pointer to the MIDI output to use for the MIDI device.
   */
  AbstractMidiOutput *midi_output;

  /**
   * A pointer to a list of audio inputs from the MIDI device.
   */
  AbstractAudioInputs *audio_inputs;

  /**
   * A pointer to a list of audio outputs to the MIDI device.
   */
  AbstractAudioOutputs *audio_outputs;

public:

  /**
   * @copydoc AbstractMidiDevice::AbstractMidiDevice()
   */
  MidiDevice();

  /**
   * @copydoc AbstractMidiDevice::AbstractMidiDevice(const string *, AbstractMidiInput *, AbstractMidiOutput *, AbstractAudioInputs *, AbstractAudioOutputs *)
   */
  MidiDevice(const string *name,
             AbstractMidiInput *midi_input,
             AbstractMidiOutput *midi_output,
             AbstractAudioInputs *audio_inputs,
             AbstractAudioOutputs *audio_outputs);

};

#endif
