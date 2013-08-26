/**
 * @file AbstractAudioInput.h
 *
 * This is a pure virtual interface for an audio input.
 */

#ifndef _ABSTRACT_AUIDO_INPUT_H_
#define _ABSTRACT_AUDIO_INPUT_H_

#include <string>

using std::string;

/**
 * Pure virtual interface for audio input implementations.
 */
class AbstractAudioInput {

public:

  /**
   * Dummy constructor.
   */
  AbstractAudioInput() {};

  /**
   * Constructor with arguments that construct an input
   * with the specified name.
   *
   * @param name Name of the audio input.
   */
  AbstractAudioInput(string *name) {};

};

#endif
