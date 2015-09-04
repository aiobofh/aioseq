#ifndef _ALSA_H_
#define _ALSA_H_

#include "types.h"

void alsa_init();
void alsa_add_device(const char* name, bool has_input, bool has_output);
void alsa_cleanup();

#endif
