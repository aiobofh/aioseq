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
  int output;
  int input;
  int channel;
  int polyphony;
  int parameters;
} device_t;

typedef struct
{
  bool changed;
  char filename[MAX_FILE_NAME_LENGTH + 1];
  char name[MAX_NAME_LENGTH + 1];
  device_idx_t devices;
  device_t device[MAX_DEVICES];
} studio_t;

studio_t studio;

static bool studio_initialized = false;

static void device_file_format(file_t* file, file_mode_t mode,
                               char* prefix, device_t* device)
{
  FSTR_1(file, mode, "%s.name", device->name, prefix);
  FINT_1(file, mode, "%s.polyphony", device->polyphony, prefix);
  FINT_1(file, mode, "%s.parameters", device->parameters, prefix);
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
  assert(MAX_NAME_LENGTH > strlen(default_new_studio_name()));
  assert(MAX_FILE_NAME_LENGTH > strlen(DEFAULT_USER_STUDIO_FILE_NAME));

  debug("Creating default studio%c", '.');

  studio.changed = true;

  strncpy(studio.filename, DEFAULT_USER_STUDIO_FILE_NAME,
          MAX_FILE_NAME_LENGTH);

  strncpy(studio.name, default_new_studio_name(), MAX_NAME_LENGTH);
  studio.devices = 1;
  strncpy(studio.device[0].name, default_new_device_name(), MAX_NAME_LENGTH);
  studio.device[0].polyphony = 8;
  studio.device[0].parameters = 8;
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
  return studio.device[idx].polyphony;
}

int studio_get_channel_parameters(int idx)
{
  return studio.device[idx].parameters;
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
