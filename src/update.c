#include <string.h>

#include "update.h"

#include "columns.h"
#include "editor.h"
#include "project.h"
#include "midi.h"

update_t update;
static bool update_initialized = false;

void update_init()
{
  update_initialized = true;
  memset(&update, 0, sizeof(update));
}

void update_commit()
{
  assert(true == update_initialized);

  for (int idx = 0; idx < update.instrument_event_cnt; idx++) {
    switch (update.instrument_event_u[idx].type) {
    case INSTRUMENT_EVENT_TYPE_NOTE_ON:
      midi_send_note_on(update.instrument_event_u[idx].device_idx,
                        update.instrument_event_u[idx].channel,
                        update.instrument_event_u[idx].data.note.key,
                        update.instrument_event_u[idx].data.note.velocity);
      break;
    case INSTRUMENT_EVENT_TYPE_NOTE_OFF:
      midi_send_note_off(update.instrument_event_u[idx].device_idx,
                         update.instrument_event_u[idx].channel,
                         update.instrument_event_u[idx].data.note.key,
                         update.instrument_event_u[idx].data.note.velocity);
      break;
    case INSTRUMENT_EVENT_TYPE_CONTROL:
      midi_send_control(update.instrument_event_u[idx].device_idx,
                        update.instrument_event_u[idx].channel,
                        update.instrument_event_u[idx].data.control.parameter,
                        update.instrument_event_u[idx].data.control.value);
      break;
    }
  }

  if (0 != update.key_cnt) {
    for (int i = 0; i < update.key_cnt; i++) {
      project_set_key(update.key_u[i].pattern_idx,
                      update.key_u[i].row_idx,
                      update.key_u[i].track_idx,
                      update.key_u[i].note_idx,
                      update.key_u[i].key);
    }
  }

  if (0 != update.velocity_cnt) {
    for (int i = 0; i < update.velocity_cnt; i++) {
      project_set_velocity(update.velocity_u[i].pattern_idx,
                           update.velocity_u[i].row_idx,
                           update.velocity_u[i].track_idx,
                           update.velocity_u[i].note_idx,
                           update.velocity_u[i].velocity);
    }
  }

  if (0 != update.command_cnt) {
    for (int i = 0; i < update.command_cnt; i++) {
      project_set_command(update.command_u[i].pattern_idx,
                          update.command_u[i].row_idx,
                          update.command_u[i].track_idx,
                          update.command_u[i].effect_idx,
                          update.command_u[i].command);
    }
  }

  if (0 != update.parameter_cnt) {
    for (int i = 0; i < update.parameter_cnt; i++) {
      project_set_parameter(update.parameter_u[i].pattern_idx,
                            update.parameter_u[i].row_idx,
                            update.parameter_u[i].track_idx,
                            update.parameter_u[i].effect_idx,
                            update.parameter_u[i].parameter);
    }
  }

  if (0 != update.edit_cnt) {
    project_set_edit(update.edit_u.edit);
    editor_set_edit(update.edit_u.edit);
  }

  if (0 != update.song_idx_cnt) {
    project_set_song_idx(update.song_idx_u.song_idx);
    editor_set_song_idx(update.song_idx_u.song_idx);
  }

  if (0 != update.song_part_idx_cnt) {
    project_set_song_part_idx(update.song_part_idx_u.song_idx,
                              update.song_part_idx_u.song_part_idx);
    editor_set_song_part_idx(update.song_part_idx_u.song_idx,
                             update.song_part_idx_u.song_part_idx);
  }

  if (0 != update.part_idx_cnt) {
    project_set_part_idx(update.part_idx_u.song_idx,
                         update.part_idx_u.song_part_idx,
                         update.part_idx_u.part_idx);
    editor_set_part_idx(update.part_idx_u.song_idx,
                        update.part_idx_u.song_part_idx,
                        update.part_idx_u.part_idx);
  }

  if (0 != update.part_pattern_idx_cnt) {
    project_set_part_pattern_idx(update.part_pattern_idx_u.part_idx,
                                 update.part_pattern_idx_u.part_pattern_idx);
    editor_set_part_pattern_idx(update.part_pattern_idx_u.part_idx,
                                update.part_pattern_idx_u.part_pattern_idx);
   }

  if (0 != update.pattern_idx_cnt) {
    project_set_pattern_idx(update.pattern_idx_u.part_idx,
                            update.pattern_idx_u.part_pattern_idx,
                            update.pattern_idx_u.pattern_idx);
    editor_set_pattern_idx(update.pattern_idx_u.part_idx,
                           update.pattern_idx_u.part_pattern_idx,
                           update.pattern_idx_u.pattern_idx);
  }

  if (0 != update.row_idx_cnt) {
    project_set_row_idx(update.row_idx_u.row_idx);
    editor_set_row_idx(update.row_idx_u.pattern_idx,
                       update.row_idx_u.row_idx);
  }

  if (0 != update.column_idx_cnt) {
    columns_set_column_idx(update.column_idx_u.column_idx);
    editor_set_column_idx(update.column_idx_u.column_idx);
  }

  memset(&update, 0, sizeof(update));
}
