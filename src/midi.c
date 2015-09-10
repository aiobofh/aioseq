
#include "alsa.h"
#include "midi.h"

void midi_init()
{
  alsa_init();
}

void midi_add_device(int idx, const char* name,
                     bool has_input, bool has_output)
{
  alsa_add_device(idx, name, has_input, has_output);
}

void midi_send_note_on(device_idx_t device_idx,
                       int channel,
                       unsigned char key,
                       unsigned char velocity)
{
  alsa_send_note_on(device_idx, channel, key, velocity);
}

void midi_send_note_off(device_idx_t device_idx,
                        int channel,
                        unsigned char key,
                        unsigned char velocity)
{
  alsa_send_note_off(device_idx, channel, key, velocity);
}

void midi_send_control(device_idx_t device_idx,
                       int channel,
                       unsigned char parameter,
                       unsigned char value)
{
  alsa_send_control(device_idx, channel, parameter, value);
}

void midi_poll_events()
{
  alsa_poll_events();
}

void midi_send_events()
{
  alsa_send_events();
}

void midi_cleanup()
{
  alsa_cleanup();
}
