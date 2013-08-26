/**
 * @file Studio.cpp
 */

#include "Studio.h"

#include "AbstractInstruments.h"
#include "AbstractAudioOutputs.h"
#include "AbstractAudioInputs.h"

#include "Name.h"

Studio::Studio(AbstractInstruments *instruments,
               AbstractAudioOutputs *audio_outputs,
               AbstractAudioInputs *audio_inputs) : Name(DEFAULT_STUDIO_NAME), instruments(instruments), audio_outputs(audio_outputs), audio_inputs(audio_inputs) {
}

Studio::Studio(string *name,
               AbstractInstruments *instruments,
               AbstractAudioOutputs *audio_outputs,
               AbstractAudioInputs *audio_inputs) : Name(name), instruments(instruments), audio_outputs(audio_outputs), audio_inputs(audio_inputs) {
}
