#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "constants.h"
#include "types.h"
#include "project.h"

void editor_init();
char* editor_ask_for_project_filename(char* filename, char* name);
bool editor_ask_for_overwrite(char* filename, char* name);
void editor_cleanup();
void editor_move_selected_line(row_idx_t old_row_idx,
                               row_idx_t new_row_idx);
void editor_refresh_pattern();
void editor_read_kbd();
void editor_refresh_windows();

void editor_set_octave(int octave);

void editor_refresh_song_idx();
void editor_refresh_part_idx();
void editor_refresh_pattern_idx();
void editor_refresh_tempo();
void editor_refresh_status();
void editor_refresh_devices();

int editor_debug(const char *format, ...);


void editor_set_edit(const bool edit);
void editor_set_quantization(const quantization_t quantization);
void editor_set_project_mode(const project_mode_t mode);
void editor_set_song_idx(const song_idx_t song_idx);
void editor_set_song_part_idx(const song_idx_t song_idx,
                              const song_part_idx_t song_part_idx);
void editor_set_song_parts(const song_idx_t song_idx,
                           const song_part_idx_t song_parts);
void editor_set_part_idx(const song_idx_t song_idx,
                         const song_part_idx_t song_part_idx,
                         const part_idx_t part_idx);
void editor_set_part_pattern_idx(const part_idx_t part_idx,
                                 const part_pattern_idx_t part_pattern_idx);
void editor_set_part_patterns(const part_idx_t part_idx,
                              const part_pattern_idx_t part_patterns);
void editor_set_pattern_idx(const part_idx_t part_idx,
                            const part_pattern_idx_t part_pattern_idx,
                            const pattern_idx_t pattern_idx);
void editor_set_pattern_rows(const pattern_idx_t pattern_idx,
                             const row_idx_t rows);
void editor_set_row_idx(const pattern_idx_t pattern_idx,
                        const row_idx_t row_idx);
void editor_set_column_idx(const column_idx_t column);

#endif
