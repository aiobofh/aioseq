/**
 * @file AbstractStudio.h
 */

#ifndef _ABSTRACT_STUDIO_H_
#define _ABSTRACT_STUDIO_H_

#include "AbstractInstruments.h"
#include "AbstractAudioOutputs.h"
#include "AbstractAudioInputs.h"

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
   */
  AbstractStudio(AbstractInstruments *instruments,
                 AbstractAudioOutputs *audio_outputs,
                 AbstractAudioInputs *audio_inputs) {};

  /**
   * Constructor taking name, instruments, outputs and
   * inputs as arguments.
   *
   * @param name Name of the studio.
   * @param instruments A pointer to a list of pointers to instruments.
   * @param audio_outputs A poiner to a list of pointers to audio outputs.
   * @param audio_inputs A pointer to a list of pointers to audio inputs.
   */
  AbstractStudio(string *name,
                 AbstractInstruments *instruments,
                 AbstractAudioOutputs *audio_outputs,
                 AbstractAudioInputs *audio_inputs) {};

};

#endif
