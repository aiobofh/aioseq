#include <alsa/asoundlib.h>

#include "defaults.h"
#include "error.h"

#include "alsa.h"

typedef struct {
  snd_seq_t *seq;
  int master_keyboard_port;
  int poller;
  struct pollfd poll;
  int devices;
  int device[MAX_DEVICES];
} alsa_t;

static bool alsa_initialized = false;

static alsa_t alsa;

void alsa_init()
{
  memset(&alsa, 0, sizeof(alsa));

  /*
   * Initialize the ALSA system
   */
  if (0 > snd_seq_open(&alsa.seq, "default", SND_SEQ_OPEN_DUPLEX, 0)) {
    error("Unable to open ALSA sequencer port");
    exit(EXIT_FAILURE);
  }
  if (0 > snd_seq_set_client_name(alsa.seq, DEFAULT_ALSA_PORT_NAME)) {
    error("Unable to set ALSA client name '%s'", DEFAULT_ALSA_PORT_NAME);
    exit(EXIT_FAILURE);
  }

  /*
   * Create a master-keyboard port
   */
  alsa.master_keyboard_port =
    snd_seq_create_simple_port(alsa.seq,
                               DEFAULT_MASTER_KEYBOARD_PORT_NAME,
                               SND_SEQ_PORT_CAP_WRITE |
                               SND_SEQ_PORT_CAP_SUBS_WRITE,
                               SND_SEQ_PORT_TYPE_APPLICATION);
  if (0 > alsa.master_keyboard_port) {
    error("Unable to create ALSA master keyboard port");
    exit(EXIT_FAILURE);
  }

  /*
   * Create a poller
   */
  alsa.poller = snd_seq_poll_descriptors_count(alsa.seq, POLLIN);
  snd_seq_poll_descriptors(alsa.seq, &alsa.poll, alsa.poller, POLLIN);

  alsa_initialized = true;
}

void alsa_add_device(const char* name, bool has_input, bool has_output)
{
  assert(true == alsa_initialized);
  assert((true == has_input || true == has_output));
  assert(NULL != name);
  assert(NULL != alsa.seq);
  assert(0 != alsa.poller);

  int caps = 0;
  int type = SND_SEQ_PORT_TYPE_APPLICATION;

  /*
   * Shall the port be bi-directional or not?
   */
  if (true == has_input) {
    caps |= SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE;
  }
  if (true == has_output) {
    caps |= SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ;
  }

  alsa.device[alsa.devices] =
    snd_seq_create_simple_port(alsa.seq, name, caps, type);

  if (0 > alsa.device[alsa.devices]) {
    error("Unable to create ALSA port '%s'", name);
    exit(EXIT_FAILURE);
  }

  debug("Adding an ASLA device called '%s'", name);

  alsa.devices++;
}

void alsa_cleanup()
{
  assert(true == alsa_initialized);

  snd_seq_close(alsa.seq);
}
