/**
 * @file AbstractAudioInputs.h
 *
 * List of abstract audio inputs.
 */

#ifndef _ABSTRACT_AUDIO_INPUTS_H_
#define _ABSTRACT_AUDIO_INPUTS_H_

#include <list>

#include "AbstractAudioInput.h"

using std::list;

/**
 * List wrapper class to store pointers to audio inputs.
 */
class AbstractAudioInputs : public list<AbstractAudioInput*> {

public:

 AbstractAudioInputs() : list<AbstractAudioInput*>() {};

};

#endif
