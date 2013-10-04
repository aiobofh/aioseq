/**
 * @file MidiDevice.cpp
 *
 * MIDI device implementation.
 */

#include "Name.h"
#include "AbstractMidiInput.h"
#include "AbstractMidiOutput.h"
#include "AbstractAudioInputs.h"
#include "AbstractAudioOutputs.h"
#include "MidiDevice.h"

MidiDevice::MidiDevice() :
  Name(DEFAULT_MIDI_DEVICE_NAME),
  midi_input(NULL),
  midi_output(NULL),
  audio_inputs(NULL),
  audio_outputs(NULL)
{
}

MidiDevice::MidiDevice(const string *name,
                       AbstractMidiInput *midi_input,
                       AbstractMidiOutput *midi_output,
                       AbstractAudioInputs *audio_inputs,
                       AbstractAudioOutputs *audio_outputs) :
  Name(name),
  midi_input(midi_input),
  midi_output(midi_output),
  audio_inputs(audio_inputs),
  audio_outputs(audio_outputs)
{
}
