/**
 * @file Studio.h
 *
 * The interface declaration for the Studio class.
 *
 * @copydetails AbstractStudio.h
 */

#ifndef _STUDIO_H_
#define _STUDIO_H_

#include <gtest/gtest_prod.h>

#include "AbstractInstruments.h"
#include "AbstractAudioOutputs.h"
#include "AbstractAudioInputs.h"
#include "AbstractMidiOutputs.h"
#include "AbstractMidiInputs.h"
#include "AbstractMidiDevices.h"
#include "AbstractStudio.h"
#include "Name.h"

/**
 * The default studio name if nothing is provided.
 */
#define DEFAULT_STUDIO_NAME "New studio"

/**
 * A studio is a class to store a users studio with connections to
 * instruments and audio card hardware.
 */
class Studio : public AbstractStudio, public Name {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Studio, Constructor_with_name_instruments_audio_outputs_and_inputs_shall_construct_a_studio_with_name_audio_outputs_and_inputs);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Studio, Constructor_without_name_instruments_audio_outputs_and_inputs_shall_construct_a_studio_with_default_name_audio_outputs_and_inputs);

private:

  /**
   * A pointer to a list of pointers to instruments.
   */
  AbstractInstruments *instruments;

  /**
   * A pointer to a list of pointers to audio outputs.
   */
  AbstractAudioOutputs *audio_outputs;

  /**
   * A pointer to a list of pointers to audio inputs.
   */
  AbstractAudioInputs *audio_inputs;

  /**
   * A pointer to a list of pointers to MIDI outputs.
   */
  AbstractMidiOutputs *midi_outputs;

  /**
   * A pointer to a list of pointers to MIDI inputs.
   */
  AbstractMidiInputs *midi_inputs;

  /**
   * A pointer to a list of pointers to MIDI devices.
   */
  AbstractMidiDevices *midi_devices;

public:

  /**
   * @copydoc AbstractStudio::AbstractStudio(AbstractInstruments*, AbstractAudioOutputs*, AbstractAudioInputs*, AbstractMidiOutputs*, AbstractMidiInputs*, AbstractMidiDevices*)
   */
  Studio(AbstractInstruments *instruments,
         AbstractAudioOutputs *audio_outputs,
         AbstractAudioInputs *audio_inputs,
         AbstractMidiOutputs *midi_outputs,
         AbstractMidiInputs *midi_inputs,
         AbstractMidiDevices *midi_devices);

  /**
   * @copydoc AbstractStudio::AbstractStudio(string*, AbstractInstruments*, AbstractAudioOutputs*, AbstractAudioInputs*, AbstractMidiOutputs*, AbstractMidiInputs*, AbstractMidiDevices*)
   */
  Studio(string *name,
         AbstractInstruments *instruments,
         AbstractAudioOutputs *audio_outputs,
         AbstractAudioInputs *audio_inputs,
         AbstractMidiOutputs *midi_outputs,
         AbstractMidiInputs *midi_inputs,
         AbstractMidiDevices *midi_devices);

  /**
   * @copydoc AbstractStudio::get_audio_outputs
   */
  AbstractAudioOutputs *get_audio_outputs();

  /**
   * @copydoc AbstractStudio::get_audio_inputs
   */
  AbstractAudioInputs *get_audio_inputs();

  /**
   * @copydoc AbstractStudio::get_midi_outputs
   */
  AbstractMidiOutputs *get_midi_outputs();

  /**
   * @copydoc AbstractStudio::get_midi_inputs
   */
  AbstractMidiInputs *get_midi_inputs();

  /**
   * @copydoc AbstractStudio::get_midi_devices
   */
  AbstractMidiDevices *get_midi_devices();

};

#endif
