#ifndef _ALSA_H_
#define _ALSA_H_

#include "types.h"

void alsa_init();
void alsa_set_tempo(tempo_t bpm);
void alsa_send_tick();
void alsa_add_device(int idx, const char* name,
                     bool has_input, bool has_output);
void alsa_send_note_on(device_idx_t device_idx,
                       int channel,
                       unsigned char key,
                       unsigned char velocity);
void alsa_send_note_off(device_idx_t device_idx,
                        int channel,
                        unsigned char key,
                        unsigned char velocity);
void alsa_send_control(device_idx_t device_idx,
                       int channel,
                       unsigned char parameter,
                       unsigned char value);
void alsa_send_events();
void alsa_poll_events();
void alsa_cleanup();

#endif
