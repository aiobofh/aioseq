#ifndef _MIDI_H_
#define _MIDI_H_

#include "types.h"

void midi_init();
void midi_add_device(int idx, const char* name,
                     bool has_input, bool has_output);
void midi_poll_events();
void midi_send_events();
void midi_cleanup();

#endif
