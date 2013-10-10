/**
 * @file Studio.cpp
 */

#include "Studio.h"

#include "AbstractInstruments.h"
#include "AbstractAudioOutputs.h"
#include "AbstractAudioInputs.h"
#include "AbstractMidiInputs.h"
#include "AbstractMidiOutputs.h"
#include "AbstractMidiDevices.h"

#include "Name.h"

Studio::Studio(AbstractInstruments *instruments,
               AbstractAudioOutputs *audio_outputs,
               AbstractAudioInputs *audio_inputs,
               AbstractMidiOutputs *midi_outputs,
               AbstractMidiInputs *midi_inputs,
               AbstractMidiDevices *midi_devices) :
  Name(DEFAULT_STUDIO_NAME),
  instruments(instruments),
  audio_outputs(audio_outputs),
  audio_inputs(audio_inputs),
  midi_outputs(midi_outputs),
  midi_inputs(midi_inputs),
  midi_devices(midi_devices) {
}

Studio::Studio(string *name,
               AbstractInstruments *instruments,
               AbstractAudioOutputs *audio_outputs,
               AbstractAudioInputs *audio_inputs,
               AbstractMidiOutputs *midi_outputs,
               AbstractMidiInputs *midi_inputs,
               AbstractMidiDevices *midi_devices) :
  Name(name),
  instruments(instruments),
  audio_outputs(audio_outputs),
  audio_inputs(audio_inputs),
  midi_outputs(midi_outputs),
  midi_inputs(midi_inputs),
  midi_devices(midi_devices) {
}

AbstractAudioOutputs *Studio::get_audio_outputs() {
  return audio_outputs;
}

AbstractAudioInputs *Studio::get_audio_inputs() {
  return audio_inputs;
}

AbstractMidiOutputs *Studio::get_midi_outputs() {
  return midi_outputs;
}

AbstractMidiInputs *Studio::get_midi_inputs() {
  return midi_inputs;
}

AbstractMidiDevices *Studio::get_midi_devices() {
  return midi_devices;
}
