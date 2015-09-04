#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#define DEFAULT_STUDIO_FILE_EXT ".ass"
#define DEFAULT_PROJECT_FILE_EXT ".asf"
#define DEFAULT_CONFIG_SEPARATOR " = "
#define DEFAULT_PATH_SEPARATOR '/'
#define DEFAULT_STUDIO_FILE_NAME "studio.ass"
#define DEFAULT_SYSTEM_STUDIO_FILE_NAME "/etc/aioseq/" DEFAULT_STUDIO_FILE_NAME
#define DEFAULT_USER_STUDIO_FILE_NAME default_user_studio_file_name()

#define DEFAULT_ALSA_PORT_NAME "AiOSeq"
#define DEFAULT_MASTER_KEYBOARD_PORT_NAME "Master keyboard"
#define DEFAULT_STUDIO_NAME "New studio"
#define DEFAULT_INSTRUMENT_NAME "New instrument"
#define DEFAULT_SETTINGS_NAME "New settings"
#define DEFAULT_PROJECT_NAME "New project"
#define DEFAULT_DEVICE_NAME "New device"
#define DEFAULT_SONG_NAME "New song"
#define DEFAULT_PART_NAME "New part"
#define DEFAULT_PATTERN_NAME "New pattern"
#define DEFAULT_TRACK_NAME "New track"

#endif

extern const char* key_map[128];

static inline char* default_user_studio_file_name() {
  char path[MAX_FILE_NAME_LENGTH];
  char* p = &path[0];
  sprintf(path, "%s%c.aioseq%c%s", getenv("HOME"),
          DEFAULT_PATH_SEPARATOR, DEFAULT_PATH_SEPARATOR,
          DEFAULT_STUDIO_FILE_NAME);
  return p;
}
