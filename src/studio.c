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

static void key_map_file_format(file_format_args(key_map_t))
{
  fstr(name);
  fstr(short_name);
  fint(key);
}

static void command_preset_file_format(file_format_args(command_preset_t))
{
  fstr(name);
  fstr(short_name);
  fint(command);
}

static void setting_file_format(file_format_args(settings_t))
{
  fstr(name);
}

static void instrument_file_format(file_format_args(instrument_t))
{
  fstr(name);
  fint(polyphony);
  fint(bank);
  fint(program);
  farray(settings, setting);
  farray(command_presets, command_preset);
  farray(key_maps, key_map);
}

static void device_file_format(file_format_args(device_t))
{
  fstr(name);
  fint(parameters);
  farray(instruments, instrument);
}

static void studio_file_format(file_t* file, mode_t mode)
{
  /* Needed by the convenience macros fstr and fint */
  studio_t* data = &studio;
  char* prefix = NULL;

  fstr(name);
  farray(devices, device);
}

static void default_studio()
{
  assert(true == studio_initialized);
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_STUDIO_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_DEVICE_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_INSTRUMENT_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_SETTINGS_NAME));
  assert(MAX_FILE_NAME_LENGTH > strlen(DEFAULT_USER_STUDIO_FILE_NAME));

  debug("Creating default studio");

  studio.changed = true;

  strncpy(studio.filename, DEFAULT_USER_STUDIO_FILE_NAME,
          MAX_FILE_NAME_LENGTH);
  strncpy(studio.name, DEFAULT_STUDIO_NAME, MAX_NAME_LENGTH);

  device_t* device = &studio.device[0];
  studio.devices = 1;
  strncpy(device->name, DEFAULT_DEVICE_NAME, MAX_NAME_LENGTH);
  device->parameters = 8;

  instrument_t* instrument = &device->instrument[0];
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
