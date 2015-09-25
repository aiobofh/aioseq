#include <alloca.h>
#include <alsa/asoundlib.h>

#include "defaults.h"
#include "error.h"
#include "event.h"
#include "types.h"

#include "alsa.h"
#undef debug
#define debug printf

typedef struct {
  snd_seq_t *seq;
  int master_keyboard_port;
  int poller;
  int queue;
  struct pollfd poll;
  int devices;
  int device[MAX_DEVICES];
  int device_map[MAX_DEVICES];
  int tick;
} alsa_t;

static bool alsa_initialized = false;

static alsa_t alsa;

void alsa_init()
{
  memset(&alsa, 0, sizeof(alsa));
  memset(&alsa.device_map, -1, sizeof(alsa.device_map));

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
   * Create a poller
   */
  alsa.poller = snd_seq_poll_descriptors_count(alsa.seq, POLLIN);
  snd_seq_poll_descriptors(alsa.seq, &alsa.poll, alsa.poller, POLLIN);

  alsa.queue = snd_seq_alloc_queue(alsa.seq);
  if (0 > alsa.queue) {
    error("Unable to set-up the ALSA queue");
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

  alsa_initialized = true;
}

void alsa_set_tempo(tempo_t bpm)
{
  snd_seq_queue_tempo_t *tempo;
  snd_seq_queue_tempo_alloca(&tempo);
  snd_seq_queue_tempo_set_tempo(tempo, 60 * 1000000 / bpm);
  snd_seq_queue_tempo_set_ppq(tempo, 96);
  if (0 > snd_seq_set_queue_tempo(alsa.seq, alsa.queue, tempo)) {
    error("Could not set tempo for the queue");
    exit(EXIT_FAILURE);
  }
}

void alsa_send_tick()
{
  snd_seq_event_t ev;
  ev.type = SND_SEQ_EVENT_CLOCK;

  ev.tag = 127;

  snd_seq_ev_set_fixed( &ev );
  snd_seq_ev_set_priority( &ev, 1 );

  /* set source */
  snd_seq_ev_set_source(&ev, 1);
  snd_seq_ev_set_subs(&ev);

  // its immediate
  snd_seq_ev_set_direct( &ev );

  /* pump it into the queue */
  snd_seq_event_output(alsa.seq, &ev);

  snd_seq_drain_output(alsa.seq);
}

void alsa_add_device(int idx, const char* name,
                     bool has_input, bool has_output)
{
  assert(true == alsa_initialized);
  assert((true == has_input || true == has_output));
  assert(NULL != name);
  assert(NULL != alsa.seq);
  assert(0 != alsa.poller);
  assert(0 <= idx);

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

  assert(-1 == alsa.device_map[idx]);

  alsa.device_map[idx] = alsa.devices;

  alsa.devices++;
}

void alsa_send_note_on(device_idx_t device_idx, int channel, unsigned char key, unsigned char velocity)
{
  snd_seq_event_t ev;
  debug("seinding note %d %d on channel %d on device %d (alsa device %d)",
        key, velocity, channel, device_idx, alsa.device[device_idx]);
  snd_seq_ev_clear(&ev);
  snd_seq_ev_set_source(&ev, alsa.device[device_idx]);
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_schedule_tick(&ev, alsa.queue, 0, alsa.tick++);
  ev.type = SND_SEQ_EVENT_NOTEON;
  ev.data.note.channel = channel;
  ev.data.note.note = key;
  ev.data.note.velocity = velocity;
  ev.queue = alsa.queue;
  snd_seq_event_output(alsa.seq, &ev);
}

void alsa_send_note_off(device_idx_t device_idx, int channel, unsigned char key, unsigned char velocity)
{
  snd_seq_event_t ev;
  debug("stopping note %d %d on channel %d on device %d (alsa device %d)",
        key, velocity, channel, device_idx, alsa.device[device_idx]);
  snd_seq_ev_clear(&ev);
  snd_seq_ev_set_source(&ev, alsa.device[device_idx]);
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_schedule_tick(&ev, alsa.queue, 0, alsa.tick++);
  ev.type = SND_SEQ_EVENT_NOTEOFF;
  ev.data.note.channel = channel;
  ev.data.note.note = key;
  ev.data.note.velocity = velocity;
  ev.data.note.off_velocity = velocity;
  ev.queue = alsa.queue;
  snd_seq_event_output(alsa.seq, &ev);
}

void alsa_send_control(device_idx_t device_idx, int channel, unsigned char parameter, unsigned char value)
{
  return;
}

void alsa_send_events()
{
  alsa.tick = 0;
  snd_seq_start_queue(alsa.seq, alsa.queue, 0);
}


void alsa_poll_events()
{
  /*
  assert(true == alsa_initialized);

  snd_seq_event_t *ev;

  if (0 >= poll(&alsa.poll, alsa.poller, 10)) {
    return;
  }

  debug("Got ASLA events");

  do {
    snd_seq_event_input(alsa.seq, &ev);
    snd_seq_ev_set_subs(ev);
    snd_seq_ev_set_direct(ev);

    switch (ev->type) {
    case SND_SEQ_EVENT_NOTEON:
      {
        debug("Got ALSA note on: channel=%d, note=%d, velocity=%d",
              ev->data.note.channel,
              ev->data.note.note,
              ev->data.note.velocity);
        event_type_args_t args;
        event_type_note_on_t* note_on = &args.event_type_note_on;
        note_on->note = ev->data.note.note + 1;
        note_on->velocity = ev->data.note.velocity;
        note_on->channel = ev->data.note.channel;
        event_add(EVENT_TYPE_NOTE_ON, args);
        break;
      }
    case SND_SEQ_EVENT_NOTEOFF:
      {
        debug("Got ALSA note off: channel=%d, note=%d, velocity=%d",
              ev->data.note.channel,
              ev->data.note.note,
              ev->data.note.off_velocity);
        event_type_args_t args;
        event_type_note_off_t* note_off = &args.event_type_note_off;
        note_off->note = ev->data.note.note + 1;
        note_off->velocity = ev->data.note.off_velocity;
        note_off->channel = ev->data.note.channel;
        event_add(EVENT_TYPE_NOTE_OFF, args);
        break;
      }
    case SND_SEQ_EVENT_CONTROLLER:
      {
        debug("Got ALSA controller: channel=%d, param=%d, value=%d",
              ev->data.control.channel,
              ev->data.control.param,
              ev->data.control.value);
        event_type_args_t args;
        event_type_controller_t* controller = &args.event_type_controller;
        controller->parameter = ev->data.control.param;
        controller->value = ev->data.control.value;
        controller->channel = ev->data.control.channel;
        event_add(EVENT_TYPE_CONTROLLER, args);
        break;
      }
    }

    snd_seq_free_event(ev);

  } while (0 < snd_seq_event_input_pending(alsa.seq, 0));
  */
}

/*
void alsa_send_events()
{
  int events = event_count();

  for (int idx = 0; idx < events; idx++) {
    snd_seq_event_t ev;
    int output_port = -1;
    event_type_args_t* args;
    event_get(idx, &args);

    switch (args->none.type) {
    case EVENT_TYPE_NOTE_ON:
      {
        event_type_note_on_t* note_on = &args->event_type_note_on;
        ev.type = SND_SEQ_EVENT_NOTEON;
        ev.data.note.note = note_on->note - 1;
        ev.data.note.velocity = note_on->velocity;
        ev.data.note.channel = note_on->channel;
        debug("Sent ALSA note on: channel=%d, note=%d, velocity=%d",
              ev.data.note.channel,
              ev.data.note.note,
              ev.data.note.velocity);
        break;
      }
    case EVENT_TYPE_NOTE_OFF:
      {
        event_type_note_off_t* note_off = &args->event_type_note_off;
        ev.type = SND_SEQ_EVENT_NOTEOFF;
        ev.data.note.note = note_off->note - 1;
        ev.data.note.off_velocity = note_off->velocity;
        ev.data.note.channel = note_off->channel;
        debug("Sent ALSA note off: channel=%d, note=%d, velocity=%d",
              ev.data.note.channel,
              ev.data.note.note,
              ev.data.note.off_velocity);
        break;
      }
    case EVENT_TYPE_CONTROLLER:
      {
        event_type_controller_t* controller = &args->event_type_controller;
        ev.type = SND_SEQ_EVENT_CONTROLLER;
        ev.data.control.param = controller->parameter;
        ev.data.control.value = controller->value;
        ev.data.control.channel = controller->channel;
        debug("Sent ALSA controller: channel=%d, param=%d, value=%d",
              ev.data.control.channel,
              ev.data.control.param,
              ev.data.control.value);
        break;
      }
    }
    if (0 < args->none.output) {
      output_port = alsa.device_map[args->none.output];
    }

    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);

    if (-1 == output_port) {
      for (int i = 0; i < alsa.devices; i++) {
        debug("Sending to port %d of (%d of %d)", alsa.device[i], i + 1, alsa.devices);
        snd_seq_ev_set_source(&ev, alsa.device[i]);
        snd_seq_event_output_direct(alsa.seq, &ev);
      }
    }
    else {
      debug("Sending to port %d", output_port);
      snd_seq_ev_set_source(&ev, output_port);
      snd_seq_event_output_direct(alsa.seq, &ev);
    }
  }
}
*/
void alsa_cleanup()
{
  assert(true == alsa_initialized);

  snd_seq_close(alsa.seq);
}
