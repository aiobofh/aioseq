#ifndef _ALSA_H_
#define _ALSA_H_

#include "types.h"

void alsa_init();
void alsa_add_device(int idx, const char* name,
                     bool has_input, bool has_output);
void alsa_poll_events();
void alsa_send_events();
void alsa_cleanup();

#endif
