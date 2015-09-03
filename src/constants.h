#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include "types.h"

#define MAX_VAL(TYPE) (((1 << (sizeof(TYPE) * 8)) >> 1) - 1)

#define MAX_DEVICES (((1 << (sizeof(device_idx_t) * 8)) >> 1) - 1)
#define MAX_EFFECTS (((1 << (sizeof(effect_idx_t) * 8)) >> 1) - 1)
#define MAX_NOTES (((1 << (sizeof(note_idx_t) * 8)) >> 1) - 1)
#define MAX_PARTS (((1 << (sizeof(part_idx_t) * 8)) >> 1) - 1)
#define MAX_PART_PATTERNS (((1 << (sizeof(part_pattern_idx_t) * 8)) >> 1) - 1)
#define MAX_PATTERNS (((1 << (sizeof(pattern_idx_t) * 8)) >> 1) - 1)
#define MAX_ROWS (((1 << (sizeof(row_idx_t) * 8)) >> 1) - 1)
#define MAX_SONGS (((1 << (sizeof(song_idx_t) * 8)) >> 1) - 1)
#define MAX_SONG_PARTS (((1 << (sizeof(song_part_idx_t) * 8)) >> 1) - 1)
#define MAX_TRACKS (((1 << (sizeof(track_idx_t) * 8)) >> 1) - 1)
#define MAX_TRACK_ROWS (((1 << (sizeof(track_row_idx_t) * 8)) >> 1) - 1)

#define MAX_NOTE_LENGTH 3
#define MAX_VELOCITY_LENGTH 2 // TODO: Redefine this to be max chars
#define MAX_COMMAND_LENGTH 2
#define MAX_PARAMETER_LENGTH 2

#define MAX_ROW_LENGTH (((MAX_NOTE_LENGTH + 1 + MAX_VELOCITY_LENGTH) * MAX_NOTES) + 1 + ((MAX_COMMAND_LENGTH + MAX_PARAMETER_LENGTH + 1) * MAX_EFFECTS)) * MAX_TRACKS
#define MAX_COLUMNS ((MAX_NOTES + MAX_NOTES * MAX_VELOCITY_LENGTH + (MAX_EFFECTS * (MAX_COMMAND_LENGTH + MAX_PARAMETER_LENGTH))) * MAX_TRACKS)

#define MAX_FILE_NAME_LENGTH 1024
#define MAX_NAME_LENGTH 32
#define MAX_STUDIO_VERSION_LENGTH 32

#define MAX_UPDATES 32

#endif
