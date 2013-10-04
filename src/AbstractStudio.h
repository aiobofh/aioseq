/**
 * @file AbstractStudio.h
 */

#ifndef _ABSTRACT_STUDIO_H_
#define _ABSTRACT_STUDIO_H_

#include "AbstractInstruments.h" // Include in mock
#include "AbstractMidiInputs.h" // Include in mock
#include "AbstractMidiOutputs.h" // Include in mock
#include "AbstractAudioOutputs.h" // Include in mock
#include "AbstractAudioInputs.h" // Include in mock
#include "AbstractMidiDevices.h" // Include in mock

/**
 * Pure virtual interface for studio implementations.
 */
class AbstractStudio {

public:

  /**
   * Dummy constructor.
   */
  AbstractStudio() {}; // Mock

  /**
   * Constructor taking instruments, outputs and
   * inputs as arguments.
   *
   * @param instruments A pointer to a list of pointers to instruments.
   * @param audio_outputs A poiner to a list of pointers to audio outputs.
   * @param audio_inputs A pointer to a list of pointers to audio inputs.
   * @param midi_outputs A pointer to a list of pointers to MIDI outputs.
   * @param midi_inputs A pointer to a list of pointers to MIDI inputs.
   * @param midi_devices A pointer to a list of pointers to MIDI devices.
   */
  AbstractStudio(AbstractInstruments *instruments, // Mock
                 AbstractAudioOutputs *audio_outputs, // Mock
                 AbstractAudioInputs *audio_inputs, // Mock
                 AbstractMidiOutputs *midi_outputs, // Mock
                 AbstractMidiInputs *midi_inputs, // Mock
                 AbstractMidiDevices *midi_devices) {}; // Mock

  /**
   * Constructor taking name, instruments, outputs and
   * inputs as arguments.
   *
   * @param name Name of the studio.
   * @param instruments A pointer to a list of pointers to instruments.
   * @param audio_outputs A poiner to a list of pointers to audio outputs.
   * @param audio_inputs A pointer to a list of pointers to audio inputs.
   * @param midi_outputs A pointer to a list of pointers to MIDI outputs.
   * @param midi_inputs A pointer to a list of pointers to MIDI inputs.
   * @param midi_devices A pointer to a list of pointers to MIDI devices.
   */
  AbstractStudio(string *name, // Mock
                 AbstractInstruments *instruments, // Mock
                 AbstractAudioOutputs *audio_outputs, // Mock
                 AbstractAudioInputs *audio_inputs, // Mock
                 AbstractMidiOutputs *midi_outputs, // Mock
                 AbstractMidiInputs *midi_inputs, // Mock
                 AbstractMidiDevices *midi_devices) {}; // Mock

};

#endif
