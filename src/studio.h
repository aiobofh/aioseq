#include <assert.h>

#ifndef _STUDIO_H_
#define _STUDIO_H_

#include "types.h"
#include "constants.h"

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  char short_name[3];
  command_t command;
} command_preset_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  char short_name[3];
  key_t key;
} key_map_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  unsigned char bytes[MAX_SETTINGS_LENGTH + 1];
} setting_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  note_idx_t polyphony;
  effect_idx_t parameters;
  bank_idx_t bank;
  program_idx_t program;
  setting_idx_t settings;
  setting_t setting[MAX_SETTINGS + 1];
  command_preset_idx_t command_presets;
  command_preset_t command_preset[MAX_COMMAND_PRESETS + 1];
  key_map_idx_t key_maps;
  key_map_t key_map[MAX_KEY_MAPS + 1];
} instrument_t;

typedef enum __attribute__((__packed__)) {
  DIRECTION_BOTH,
  DIRECTION_IN,
  DIRECTION_OUT
} direction_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  direction_t direction;
  int channel;
  instrument_idx_t instruments;
  instrument_t instrument[MAX_INSTRUMENTS + 1];
} device_t;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  device_idx_t devices;
  device_t device[MAX_DEVICES + 1];

  bool changed;
  char filename[MAX_FILE_NAME_LENGTH + 1];
} studio_t;

void studio_init();
bool studio_load(const char* filename);
bool studio_save(const char* filename);

note_idx_t studio_get_polyphony(device_idx_t device_idx,
                         instrument_idx_t instrument_idx);
effect_idx_t studio_get_parameters(device_idx_t device_idx,
                                   instrument_idx_t instrument_idx);
const char* studio_get_device_name(device_idx_t device_idx);
const char* studio_get_instrument_name(device_idx_t device_idx,
                                       instrument_idx_t instrument_idx);
const char* studio_get_setting_name(device_idx_t device_idx,
                                    instrument_idx_t instrument_idx,
                                    setting_idx_t setting_idx);
int studio_get_channel(device_idx_t device_idx);

#endif

extern studio_t studio;
