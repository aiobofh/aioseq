#include <stdbool.h>

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>

typedef unsigned char command_t;
typedef unsigned char device_idx_t;
typedef unsigned char effect_idx_t;
typedef unsigned char key_t;
typedef unsigned char note_idx_t;
typedef unsigned char parameter_t;
typedef unsigned char part_idx_t;
typedef unsigned char part_pattern_idx_t;
typedef unsigned char pattern_idx_t;
typedef char relative_tempo_t;
typedef unsigned char row_idx_t;
typedef unsigned char track_idx_t;
typedef track_idx_t track_row_idx_t;
typedef unsigned char song_idx_t;
typedef unsigned char song_part_idx_t;
typedef unsigned char tempo_t;
typedef unsigned char velocity_t;

typedef unsigned int column_idx_t;
typedef unsigned char program_idx_t;
typedef unsigned char bank_idx_t;
typedef unsigned char instrument_idx_t;
typedef unsigned char setting_idx_t;
typedef unsigned char command_preset_idx_t;
typedef unsigned char key_map_idx_t;
typedef track_idx_t pattern_track_idx_t;

typedef FILE file_t;

typedef enum __attribute__((__packed__)) {
  PROJECT_MODE_STOPPED,
  PROJECT_MODE_PLAY_PROJECT,
  PROJECT_MODE_PLAY_SONG,
  PROJECT_MODE_PLAY_PART,
  PROJECT_MODE_PLAY_PATTERN
} project_mode_t;

#endif
