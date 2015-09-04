#ifndef _MIDI_H_
#define _MIDI_H_

#include "types.h"

void midi_init();
void midi_add_device(const char* name, bool has_input, bool has_output);
void midi_cleanup();

#endif
