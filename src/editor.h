#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "types.h"

void editor_init();
void editor_cleanup();
void editor_move_selected_line(row_idx_t old_row_idx,
                               row_idx_t new_row_idx);
void editor_refresh_pattern();
void editor_read_kbd();
void editor_refresh_windows();

#endif
