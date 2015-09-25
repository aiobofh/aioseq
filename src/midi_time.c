#include <unistd.h>
#include "alsa.h"

int main(int argc, char* argv[])
{
  argc = argc;
  argv = argv;

  alsa_init();

  alsa_add_device(0, "Timer output", false, true);

  alsa_set_tempo(120);

  while (1) {
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    alsa_send_tick();
    sleep(1);
  }

  return 0;
}
