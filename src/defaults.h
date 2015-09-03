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

static inline const char* default_new_studio_name() { return "New studio"; }
static inline const char* default_new_device_name() { return "New device"; }
static inline char* default_user_studio_file_name() {
  char path[MAX_FILE_NAME_LENGTH];
  char* p = &path[0];
  sprintf(path, "%s%c.aioseq%c%s", getenv("HOME"),
          DEFAULT_PATH_SEPARATOR, DEFAULT_PATH_SEPARATOR,
          DEFAULT_STUDIO_FILE_NAME);
  return p;
}
static inline const char* default_new_project_name() { return "New project"; }
static inline const char* default_new_pattern_name() { return "New pattern"; }
static inline const char* default_new_part_name() { return "New part"; }
static inline const char* default_new_song_name() { return "New song"; }
static inline const char* default_new_track_name() { return "New track"; }
static inline const char* default_new_instrument_name() { return "New instrument"; }
static inline const char* default_new_settings_name() { return "New settings"; }

#endif
