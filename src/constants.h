#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include "types.h"

#define MAX_VAL(TYPE) (((1 << (sizeof(TYPE) * 8)) >> 1) - 1)

#ifndef MAX_DEVICES
#define MAX_DEVICES MAX_VAL(device_idx_t)
#endif
#ifndef MAX_EFFECTS
#define MAX_EFFECTS MAX_VAL(effect_idx_t)
#endif
#ifndef MAX_NOTES
#define MAX_NOTES MAX_VAL(note_idx_t)
#endif
#ifndef MAX_PARTS
#define MAX_PARTS MAX_VAL(part_idx_t)
#endif
#ifndef MAX_PART_PATTERNS
#define MAX_PART_PATTERNS MAX_VAL(part_pattern_idx_t)
#endif
#ifndef MAX_PATTERNS
#define MAX_PATTERNS MAX_VAL(pattern_idx_t)
#endif
#ifndef MAX_ROWS
#define MAX_ROWS MAX_VAL(row_idx_t)
#endif
#ifndef MAX_SONGS
#define MAX_SONGS MAX_VAL(song_idx_t)
#endif
#ifndef MAX_SONG_PARTS
#define MAX_SONG_PARTS MAX_VAL(song_part_idx_t)
#endif
#ifndef MAX_TRACKS
#define MAX_TRACKS MAX_VAL(track_idx_t)
#endif
#ifndef MAX_TRACK_ROWS
#define MAX_TRACK_ROWS MAX_VAL(track_row_idx_t)
#endif
#ifndef MAX_SETTINGS
#define MAX_SETTINGS MAX_VAL(setting_idx_t)
#endif
#ifndef MAX_INSTRUMENTS
#define MAX_INSTRUMENTS MAX_VAL(instrument_idx_t)
#endif
#ifndef MAX_COMMAND_PRESETS
#define MAX_COMMAND_PRESETS MAX_VAL(command_preset_idx_t)
#endif
#ifndef MAX_KEY_MAPS
#define MAX_KEY_MAPS MAX_VAL(key_map_idx_t)
#endif

#define MAX_NOTE_LENGTH 3
#define MAX_VELOCITY_LENGTH 2
#define MAX_COMMAND_LENGTH 2
#define MAX_PARAMETER_LENGTH 2

#define MAX_TRACK_LENGTH                                                \
  (((MAX_NOTE_LENGTH + 1 + MAX_VELOCITY_LENGTH) * MAX_NOTES) + 1 +      \
   ((MAX_COMMAND_LENGTH + MAX_PARAMETER_LENGTH + 1) * MAX_EFFECTS))

#define MAX_ROW_LENGTH MAX_TRACK_LENGTH * MAX_TRACKS
#define MAX_COLUMNS                                                     \
  ((MAX_NOTES + MAX_NOTES * MAX_VELOCITY_LENGTH +                       \
    (MAX_EFFECTS * (MAX_COMMAND_LENGTH + MAX_PARAMETER_LENGTH))) *      \
   MAX_TRACKS)

#define MAX_FILE_NAME_LENGTH 1024
#define MAX_NAME_LENGTH 32
#define MAX_STUDIO_VERSION_LENGTH 32

#define MAX_UPDATES 32

#define MAX_SETTINGS_LENGTH 128

#endif
