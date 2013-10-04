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
