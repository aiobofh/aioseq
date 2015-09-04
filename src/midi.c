
#include "alsa.h"
#include "midi.h"

void midi_init()
{
  alsa_init();
}

void midi_add_device(const char* name, bool has_input, bool has_output)
{
  alsa_add_device(name, has_input, has_output);
}

void midi_cleanup()
{
  alsa_cleanup();
}
