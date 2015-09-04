/*
 * A studio is a virtual representation of the user's physical equipment.
 *
 * File-format:
 *
 * name = <name>
 * devices = 1
 * device[0].name = <name>
 * device[0].midi.output = <id>
 * devuce[0].midi.channel = <channel>
 * device[0].midi.input = <id>
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"
#include "defaults.h"
#include "error.h"
#include "types.h"
#include "config.h"
#include "studio.h"

typedef struct
{
  char name[MAX_NAME_LENGTH + 1];
  char short_name[3];
  command_t command;
} command_preset_t;

typedef struct
{
  char name[MAX_NAME_LENGTH + 1];
  char short_name[3];
  key_t key;
} key_map_t;

typedef struct
{
  char name[MAX_NAME_LENGTH + 1];
  unsigned char bytes[MAX_SETTINGS_LENGTH + 1];
} settings_t;

typedef struct
{
  char name[MAX_NAME_LENGTH + 1];
  int polyphony;
  bank_idx_t bank;
  program_idx_t program;
  settings_idx_t settings;
  settings_t setting[MAX_SETTINGS + 1];
  command_preset_idx_t command_presets;
  command_preset_t command_preset[MAX_COMMAND_PRESETS];
  key_map_idx_t key_maps;
  key_map_t key_map[MAX_KEY_MAPS + 1];
} instrument_t;

typedef struct
{
  char name[MAX_NAME_LENGTH + 1];
  int output;
  int input;
  int channel;
  int parameters;
  instrument_idx_t instruments;
  instrument_t instrument[MAX_INSTRUMENTS + 1];
} device_t;

typedef struct
{
  bool changed;
  char filename[MAX_FILE_NAME_LENGTH + 1];
  char name[MAX_NAME_LENGTH + 1];
  device_idx_t devices;
  device_t device[MAX_DEVICES + 1];
} studio_t;

studio_t studio;

static bool studio_initialized = false;

static void key_map_file_format(file_t* file, file_mode_t mode, char* prefix, key_map_t* key_map)
{
  FSTR_1(file, mode, "%s.name", key_map->name, prefix);
  FSTR_1(file, mode, "%s.short_name", key_map->short_name, prefix);
  FINT_1(file, mode, "%s.key", key_map->key, prefix);
}

static void command_preset_file_format(file_t* file, file_mode_t mode, char* prefix, command_preset_t* command_preset)
{
  FSTR_1(file, mode, "%s.name", command_preset->name, prefix);
  FSTR_1(file, mode, "%s.short_name", command_preset->short_name, prefix);
  FINT_1(file, mode, "%s.command", command_preset->command, prefix);
}

static void settings_file_format(file_t* file, file_mode_t mode, char* prefix, settings_t* settings)
{
  FSTR_1(file, mode, "%s.name", settings->name, prefix);
}

static void instrument_file_format(file_t* file, file_mode_t mode, char* prefix, instrument_t* instrument)
{
  FSTR_1(file, mode, "%s.name", instrument->name, prefix);
  FINT_1(file, mode, "%s.polyphony", instrument->polyphony, prefix);
  FINT_1(file, mode, "%s.bank", instrument->bank, prefix);
  FINT_1(file, mode, "%s.program", instrument->program, prefix);

  FINT_1(file, mode, "%s.settings", instrument->settings, prefix);

  for (settings_idx_t idx = 0; idx < instrument->settings; idx++) {
    char pfx[128];
    sprintf(pfx, "%s.setting[%d]", prefix, idx);
    settings_file_format(file, mode, pfx, &instrument->setting[idx]);
  }

  FINT_1(file, mode, "%s.command_presets", instrument->command_presets, prefix);

  for (command_preset_idx_t idx = 0; idx < instrument->command_presets; idx++) {
    char pfx[128];
    sprintf(pfx, "%s.command_preset[%d]", prefix, idx);
    command_preset_file_format(file, mode, pfx, &instrument->command_preset[idx]);
  }

  FINT_1(file, mode, "%s.key_maps", instrument->key_maps, prefix);

  for (key_map_idx_t idx = 0; idx < instrument->key_maps; idx++) {
    char pfx[128];
    sprintf(pfx, "%s.key_map[%d]", prefix, idx);
    key_map_file_format(file, mode, pfx, &instrument->key_map[idx]);
  }
}

static void device_file_format(file_t* file, file_mode_t mode,
                               char* prefix, device_t* device)
{
  FSTR_1(file, mode, "%s.name", device->name, prefix);
  FINT_1(file, mode, "%s.parameters", device->parameters, prefix);
  FINT_1(file, mode, "%s.instruments", device->instruments, prefix);

  for (instrument_idx_t idx = 0; idx < device->instruments; idx++) {
    char pfx[128];
    sprintf(pfx, "%s.instrument[%d]", prefix, idx);
    instrument_file_format(file, mode, pfx, &device->instrument[idx]);
  }
}

static void studio_file_format(file_t* file, mode_t mode)
{
  FSTR(file, mode, "name", studio.name);
  FINT(file, mode, "devices", studio.devices);

  for (device_idx_t idx = 0; idx < studio.devices; idx++) {
    char prefix[128];
    sprintf(prefix, "device[%d]", idx);
    device_file_format(file, mode, prefix, &studio.device[idx]);
  }
}

static void default_studio()
{
  assert(true == studio_initialized);
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_STUDIO_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_DEVICE_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_INSTRUMENT_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_SETTINGS_NAME));
  assert(MAX_FILE_NAME_LENGTH > strlen(DEFAULT_USER_STUDIO_FILE_NAME));

  debug("Creating default studio%c", '.');

  studio.changed = true;

  strncpy(studio.filename, DEFAULT_USER_STUDIO_FILE_NAME,
          MAX_FILE_NAME_LENGTH);
  strncpy(studio.name, DEFAULT_STUDIO_NAME, MAX_NAME_LENGTH);

  device_t* device = &studio.device[0];
  assert(NULL != device);
  assert(NULL != device->name);
  studio.devices = 1;
  strncpy(device->name, DEFAULT_DEVICE_NAME, MAX_NAME_LENGTH);
  device->parameters = 8;

  instrument_t* instrument = &device->instrument[0];
  assert(NULL != instrument);
  assert(NULL != instrument->name);
  device->instruments = 1;
  strncpy(instrument->name, DEFAULT_INSTRUMENT_NAME, MAX_NAME_LENGTH);
  instrument->polyphony = 1;
  instrument->settings = 1;

  settings_t* setting = &instrument->setting[0];
  strncpy(setting->name, DEFAULT_SETTINGS_NAME, MAX_NAME_LENGTH);
}

static int strposr(const char* in)
{
  int pos = strlen(in);
  while (pos > 0 && in[pos] != DEFAULT_PATH_SEPARATOR)
    pos--;

  if (pos > 0) {
    return pos;
  }
  return -1;
}

static bool check_for_or_create_config_dir(const char* file_path)
{
  int slash_pos = strposr(file_path);

  if (-1 == slash_pos) {
    return true;
  }

  char path[MAX_FILE_NAME_LENGTH];
  strncpy(path, file_path, slash_pos);
  path[slash_pos] = 0;
  if (0 != mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
    if (EEXIST != errno) {
      error("Unable to create config folder '%s'.", path);
      return false;
    }
  }
  return true;
}


void studio_init()
{
  studio_initialized = true;
  memset(&studio, 0, sizeof(studio));
}

bool studio_load(const char* filename)
{
  assert(true == studio_initialized);

  m_line_counter = 0;

  if (NULL == filename) {
    default_studio();
    return true;
  }

  assert(MAX_FILE_NAME_LENGTH > strlen(filename));
  strncpy(studio.filename, filename, MAX_FILE_NAME_LENGTH);

  file_t *fd = fopen(studio.filename, "r");
  if (NULL == fd) {
    error("Unable to studio file '%s'.", studio.filename);
    default_studio();
    // Override filename from default.
    strncpy(studio.filename, filename, MAX_FILE_NAME_LENGTH);
    debug("Setting up studio in file '%s'.", studio.filename);
  }
  else {
    studio_file_format(fd, MODE_READ);

    debug("Loaded studio '%s' from file '%s'", studio.name, studio.filename);

    fclose(fd);
  }
  return true;
}

int studio_get_channel_polyphony(int idx)
{
  return 3; /* TODO: Get real info from the track instrument */
}

int studio_get_channel_parameters(int idx)
{
  return 3; /* TODO: Get real info from the track instrument */
}

bool studio_save(const char* filename)
{
  char *f = (char*)filename;
  assert(true == studio_initialized);
  assert(0 != studio.filename[0]);

  if (false == studio.changed) {
    debug("The studio '%s' did not change since load, not saving",
          studio.name);
    return true;
  }

  if (NULL == f) {
    f = studio.filename;
    debug("Using previous studio filename '%s' for save.", f);
  }

  if (false == check_for_or_create_config_dir(f)) {
    error("Could not create folder for saving studio '%s'.", f);
    return false;
  }

  file_t *fd = fopen(f, "w+");
  if (NULL == fd) {
    error("Unable to save studio file '%s'.", f);
    return false;
  }

  studio_file_format(fd, MODE_WRITE);

  debug("Saved studio '%s' from file '%s'", studio.name, studio.filename);

  fclose(fd);

  return true;
}

#undef file_format
