/**
 * @file AbstractAudioOutputs.h
 *
 * List of abstract audio outputs.
 */

#ifndef _ABSTRACT_AUDIO_OUTPUTS_H_
#define _ABSTRACT_AUDIO_OUTPUTS_H_

#include "AbstractAudioOutput.h"

using std::list;

/**
 * List wrapper class to store pointers to audio outputs.
 */
class AbstractAudioOutputs : public list<AbstractAudioOutput*> {

public:

 AbstractAudioOutputs() : list<AbstractAudioOutput*>() {};

};

#endif
