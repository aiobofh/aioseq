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

public:

  /**
   * @copydoc AbstractStudio::AbstractStudio(AbstractInstruments*, AbstractAudioOutputs*, AbstractAudioInputs*)
   */
  Studio(AbstractInstruments *instruments,
         AbstractAudioOutputs *audio_outputs,
         AbstractAudioInputs *audio_inputs);

  /**
   * @copydoc AbstractStudio::AbstractStudio(string*, AbstractInstruments*, AbstractAudioOutputs*, AbstractAudioInputs*)
   */
  Studio(string *name,
         AbstractInstruments *instruments,
         AbstractAudioOutputs *audio_outputs,
         AbstractAudioInputs *audio_inputs);

};

#endif
