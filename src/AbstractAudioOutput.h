/**
 * @file AbstractAudioOutput.h
 *
 * This is a pure virtual interface for an audio output.
 */

#ifndef _ABSTRACT_AUIDO_OUTPUT_H_
#define _ABSTRACT_AUDIO_OUTPUT_H_

#include <string>

using std::string;

/**
 * Pure virtual interface for audio output implementations.
 */
class AbstractAudioOutput {

public:

  /**
   * Dummy constructor.
   */
  AbstractAudioOutput() {};

  /**
   * Constructor with arguments that construct an output
   * with the specified name.
   *
   * @param name Name of the audio output.
   */
  AbstractAudioOutput(string *name) {};

};

#endif
