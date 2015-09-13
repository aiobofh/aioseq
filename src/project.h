#ifndef _PROJECT_H_
#define _PROJECT_H_

#include "types.h"
#include "constants.h"

typedef char* (*ask_for_project_filename_t)(char* filename,
                                            char* name);
typedef bool (*ask_for_overwrite_t)(char* filename, char* name);

/*
 * Initialize an empty project state memory area
 **/
void project_init();

/*
 * Load the specified project filename. If filename is NULL a default
 * project will be generated from the basics of the current studio
 * configuration.
 */
bool project_load(const char* filename);

/*
 * Save the current project to disk. If filename is NULL there is a
 * callback function (ask_for_project_filename) that will be called,
 * to ask the user for a new filename, and if the filename already
 * exist there is a callback function (ask_for_overwrite) that will
 * enable the user to overwrite and existing project file.
 */
bool project_save(const char* filename,
                  char* (*ask_for_project_filename)(char* filename,
                                                    char* name),
                  bool (*ask_for_overwrite)(char* filename, char* name));

/*
 * Specialized function that fills the buf-array with a row from the
 * specified pattern.
 *
 * Example contents filled to buf:
 *
 * --- 00 --- 00 0000 0000|C-1 3f 0000
 */
void project_get_pattern_row(char* buf, pattern_idx_t pattern_idx,
                             row_idx_t row_idx);

void project_reset();

void project_update();

void project_step();

void play(project_mode_t mode);
void stop();
void edit();

/************************************************************************
 * Setters and getters - Hopefully they get inlined by -flto flag to gcc.
 */

/* Edit mode */
void project_set_edit(const edit_t edit);
bool project_get_edit();

/* Command */
void project_set_command(const pattern_idx_t pattern_idx,
                         const row_idx_t row_idx,
                         const track_idx_t track_idx,
                         const effect_idx_t effect_idx,
                         const command_t command);
command_t project_get_command(const pattern_idx_t pattern_idx,
                              const row_idx_t row_idx,
                              const track_idx_t track_idx,
                              const effect_idx_t effect_idx);

/* Device index */
device_idx_t project_get_device_idx(track_idx_t track_idx);

/* Effects */
effect_idx_t project_get_effects(const pattern_idx_t pattern_idx,
                                 const track_idx_t track_idx);

/* Instrument index */
instrument_idx_t project_get_instrument_idx(pattern_idx_t pattern_idx,
                                            track_idx_t track_idx);

/* Notes */
note_idx_t project_get_notes(const pattern_idx_t pattern_idx,
                             const track_idx_t track_idx);

/************************************************************************
 * Effects
 */

/* Key */
void project_set_key(const pattern_idx_t pattern_idx,
                     const row_idx_t row_idx,
                     const track_idx_t track_idx,
                     const note_idx_t note_idx,
                     const key_t key);

/* Parameter */
void project_set_parameter(const pattern_idx_t pattern_idx,
                           const row_idx_t row_idx,
                           const track_idx_t track_idx,
                           const effect_idx_t effect_idx,
                           const parameter_t parameter);
parameter_t project_get_parameter(const pattern_idx_t pattern_idx,
                                  const row_idx_t row_idx,
                                  const track_idx_t track_idx,
                                  const effect_idx_t effect_idx);

/* Part index */
void project_set_part_idx(const song_idx_t song_idx,
                          const song_part_idx_t song_part_idx,
                          const part_idx_t part_idx);
part_idx_t project_get_part_idx(const song_idx_t song_idx,
                                const song_part_idx_t song_part_idx);

/* Part name */
char* project_get_part_name(const part_idx_t part_idx);

/* Part pattern index */
void project_set_part_pattern_idx(const part_idx_t part_idx,
                                  const part_pattern_idx_t part_pattern_idx);
part_pattern_idx_t project_get_part_pattern_idx(const part_idx_t part_idx);

/* Part patterns */
void project_set_part_patterns(const part_idx_t part_idx,
                               const part_pattern_idx_t part_patterns);
part_pattern_idx_t project_get_part_patterns(part_idx_t part_idx);

/* Pattern index */
void project_set_pattern_idx(const part_idx_t part_idx,
                             const part_pattern_idx_t part_pattern_idx,
                             const pattern_idx_t pattern_idx);
pattern_idx_t project_get_pattern_idx(const part_idx_t part_idx,
                                      const part_pattern_idx_t part_pattern_idx);

/* Pattern name */
char* project_get_pattern_name(const pattern_idx_t pattern_idx);

/* Patterns */
void project_set_patterns(const pattern_idx_t patterns);
pattern_idx_t project_get_patterns();

/* Pattern rows */
void project_set_pattern_rows(const pattern_idx_t pattern_idx,
                              const row_idx_t rows);
row_idx_t project_get_pattern_rows(const pattern_idx_t pattern_idx);

/* Playback mode */
void project_set_project_mode(project_mode_t mode);
project_mode_t project_get_project_mode();

/* Quantization */
void project_set_quantization(const quantization_t quantization);
quantization_t project_get_quantization();

/* Row index */
void project_set_row_idx(const row_idx_t row_idx);
row_idx_t project_get_row_idx();

/* Setting index */
setting_idx_t project_get_setting_idx(pattern_idx_t pattern_idx,
                                      track_idx_t track_idx);

/* Songs */
song_idx_t project_get_songs();

/* Song index */
void project_set_song_idx(const song_idx_t song_idx);
song_idx_t project_get_song_idx();

/* Song name */
char* project_get_song_name(const song_idx_t song_idx);

/* Song part index */
void project_set_song_part_idx(const song_idx_t song_idx,
                               const song_part_idx_t song_part_idx);
song_part_idx_t project_get_song_part_idx(const song_idx_t song_idx);

/* Song parts */
void project_set_song_parts(const song_idx_t song_idx,
                            const song_part_idx_t song_parts);
song_part_idx_t project_get_song_parts(const song_idx_t song_idx);

/* Tempo */
tempo_t project_get_tempo();

/* Tracks */
track_idx_t project_get_tracks();

/* Velocity */
void project_set_velocity(const pattern_idx_t pattern_idx,
                          const row_idx_t row_idx,
                          const track_idx_t track_idx,
                          const note_idx_t note_idx,
                          const velocity_t velocity);
velocity_t project_get_velocity(const pattern_idx_t pattern_idx,
                                const row_idx_t row_idx,
                                const track_idx_t track_idx,
                                const note_idx_t note_idx);

#endif
