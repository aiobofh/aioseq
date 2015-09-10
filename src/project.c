/*
 * A project contains patterns, song-parts and songs.
 */
#include <string.h>
#include <assert.h>

#include "error.h"
#include "constants.h"
#include "defaults.h"
#include "types.h"
#include "config.h"
#include "project.h"
#include "event.h"
#include "studio.h"
#include "columns.h"
#include "update.h"

extern int m_line_counter;

project_t project;

static bool project_initialized = false;

static void track_file_format(file_format_args(track_t))
{
  fstr(name);
  fint(device_idx);
}

static void note_append(char* buf, note_t* note) {
  strcat(buf, key_map[note->key]);
  sprintf(buf, "%s %02x", buf, note->velocity);
}

static void effect_append(char* buf, effect_t* effect) {
  sprintf(buf, "%s%02x%02x", buf, effect->command, effect->parameter);
}

static void note_extract(char** buf, note_t* note) {
  char k[4];
  unsigned int v;

  memset(k, 0, sizeof(k));
  sscanf(*buf, "%3s %02x", k, &v);
  *buf += 3 + 1 + 2;

  for (size_t i = 0; i < sizeof(key_map) / (sizeof(char*)); i++) {
    if (0 == strcmp(key_map[i], k)) {
      note->key = i;
      note->velocity = v;
      return;
    }
  }

  assert(false);
}

static void effect_extract(char** buf, effect_t* effect) {
  unsigned int c;
  unsigned int p;

  sscanf(*buf, "%02x%02x", &c, &p);
  *buf += 4;

  effect->command = c;
  effect->parameter = p;
}

static void track_row_append(char* buf,
                             track_row_t* track_row,
                             note_idx_t notes,
                             effect_idx_t effects)
{
  for (note_idx_t idx = 0; idx < notes; idx++) {
    note_append(buf, &track_row->note[idx]);
    if (idx < notes - 1) {
      strcat(buf, " ");
    }
  }

  if ((0 < notes) && (0 < effects)) {
    strcat(buf, " ");
  }

  for (effect_idx_t idx = 0; idx < effects; idx++) {
    effect_append(buf, &track_row->effect[idx]);
    if (idx < effects - 1) {
      strcat(buf, " ");
    }
  }
}

static void track_row_extract(char** buf,
                              track_row_t* track_row,
                              note_idx_t notes,
                              effect_idx_t effects)
{
  for (note_idx_t idx = 0; idx < notes; idx++) {
    note_extract(buf, &track_row->note[idx]);
    if (idx < notes - 1) {
      *buf += 1;
    }
  }

  if ((0 < notes) && (0 < effects)) {
    *buf += 1;
  }

  for (effect_idx_t idx = 0; idx < effects; idx++) {
    effect_extract(buf, &track_row->effect[idx]);
    if (idx < effects - 1) {
      *buf += 1;
    }
  }
}

static void row_append(char* buf, row_t* row)
{
  for (track_idx_t track_idx = 0; track_idx < project.tracks; track_idx++) {

    const device_idx_t device_idx = get_device_idx(track_idx);
    const instrument_idx_t instrument_idx = get_instrument_idx(get_pattern_idx(),
                                                               track_idx);
    const note_idx_t polyphony = studio_get_polyphony(device_idx, instrument_idx);
    const effect_idx_t parameters = studio_get_parameters(device_idx,
                                                          instrument_idx);

    track_row_append(buf, &row->track_row[track_idx], polyphony, parameters);
    if (track_idx != project.tracks - 1) {
      strcat(buf, "|");
    }
  }
}

void get_pattern_row(char* buf, row_idx_t row_idx) {
  buf[0] = 0;

  pattern_idx_t pattern_idx = get_pattern_idx();

  row_append(buf, &project.pattern[pattern_idx].row[row_idx]);
}

static void row_file_format(file_format_args(row_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->track_row) +
                                  sizeof(data->tempo_relative_to_pattern));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  char buf[MAX_ROW_LENGTH + 1];
  char* p = &buf[0];
  memset(buf, 0, sizeof(buf));

  if (MODE_WRITE == mode) {
    row_append(buf, data);
  }

  fint(tempo_relative_to_pattern);

  /* Special */
  FSTR_1(file, mode, "%s", buf, prefix);

  if (MODE_READ == mode) {
    for (track_row_idx_t idx = 0; idx < project.tracks; idx++) {
      const device_idx_t device_idx = get_device_idx(idx);
      const instrument_idx_t instrument_idx = get_instrument_idx(get_pattern_idx(),
                                                                 idx);
      int polyphony = studio_get_polyphony(device_idx, instrument_idx);
      int parameters = studio_get_parameters(device_idx, instrument_idx);
      track_row_extract(&p, &data->track_row[idx], polyphony, parameters);
      if (idx != project.tracks - 1) {
        p++;
      }
    }
  }
}

static void pattern_track_file_format(file_format_args(pattern_track_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->instrument_idx) +
                                  sizeof(data->setting_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(instrument_idx);
  fint(setting_idx);
}

static void pattern_file_format(file_format_args(pattern_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->name) +
                                  sizeof(data->tempo_relative_to_part) +
                                  sizeof(data->rows) +
                                  sizeof(data->row) +
                                  sizeof(data->pattern_tracks) +
                                  sizeof(data->pattern_track));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fstr(name);
  fint(tempo_relative_to_part);
  farray(rows, row);
  farray(pattern_tracks, pattern_track);
}

static void part_pattern_file_format(file_format_args(part_pattern_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->pattern_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(pattern_idx);
}

static void part_file_format(file_format_args(part_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->name) +
                                  sizeof(data->tempo_relative_to_song) +
                                  sizeof(data->part_patterns) +
                                  sizeof(data->part_pattern));
  const size_t ignored_size = (sizeof(data->part_pattern_idx));

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fstr(name);
  fint(tempo_relative_to_song);
  farray(part_patterns, part_pattern);
}

static void song_part_file_format(file_format_args(song_part_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->part_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(part_idx);
}

static void song_file_format(file_format_args(song_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->name) +
                                  sizeof(data->tempo_relative_to_project) +
                                  sizeof(data->song_parts) +
                                  sizeof(data->song_part));

  const size_t ignored_size = (sizeof(data->song_part_idx));

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fstr(name);
  fint(tempo_relative_to_project);
  farray(song_parts, song_part);
}

static void project_file_format(file_t* file, file_mode_t mode)
{
  /* Needed by the convenience macros fstr and fint */
  project_t* data = &project;
  char* prefix = NULL;

  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->name) +
                                  sizeof(data->tempo) +
                                  sizeof(data->tracks) +
                                  sizeof(data->track) +
                                  sizeof(data->patterns) +
                                  sizeof(data->pattern) +
                                  sizeof(data->parts) +
                                  sizeof(data->part) +
                                  sizeof(data->songs) +
                                  sizeof(data->song));

  const size_t ignored_size = (sizeof(data->changed) +
                               sizeof(data->filename) +
                               sizeof(data->edit) +
                               sizeof(data->mode) +
                               sizeof(data->song_idx) +
                               sizeof(data->row_idx) +
                               sizeof(data->quantization));

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fstr(name);
  fint(tempo);
  farray(tracks, track);
  farray(patterns, pattern);
  farray(parts, part);
  farray(songs, song);
}

static void default_project()
{
  assert(true == project_initialized);
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_PROJECT_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_TRACK_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_PATTERN_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_PART_NAME));
  assert(MAX_NAME_LENGTH > strlen(DEFAULT_SONG_NAME));

  debug("Creating default project");

  project.changed = true;

  strncpy(project.name, DEFAULT_PROJECT_NAME, MAX_NAME_LENGTH);

  /* TODO: Use some kind of config file info for default tempo. */
  project.tempo = 120;

  pattern_t* pattern = &project.pattern[0];
  project.patterns = 1;
  strncpy(pattern->name, DEFAULT_PATTERN_NAME, MAX_NAME_LENGTH);
  /* TODO: Use some kind of config file info default pattern length. */
  pattern->rows = 64;

  project.tracks = 2;

  track_t* track = &project.track[0];
  strncpy(track->name, DEFAULT_TRACK_NAME, MAX_NAME_LENGTH);

  track = &project.track[1];
  strncpy(track->name, DEFAULT_TRACK_NAME, MAX_NAME_LENGTH);
  track->device_idx = 1;

#if 0
  /* Dummy contents, just fill the pattern with bogus things. */
  int key = 0;
  int polyphony = studio_get_channel_polyphony(0);
  for (track_idx_t t = 0; t < project.tracks; ++t) {
    for (int i = 0; i < polyphony; ++i) {
      for (row_idx_t r = 0; r < project.pattern[0].rows; ++r) {
        project.pattern[0].row[r].track_row[t].note[i].key = key;
        key++;
        if (key > 127) {
          key = 0;
        }
      }
    }
  }
#endif

  part_t* part = &project.part[0];
  project.parts = 1;
  strncpy(part->name, DEFAULT_PART_NAME, MAX_NAME_LENGTH);
  part->part_patterns = 1;
  part->part_pattern[0].pattern_idx = 0;

  song_t* song = &project.song[0];
  project.songs = 1;
  strncpy(song->name, DEFAULT_SONG_NAME, MAX_NAME_LENGTH);
  song->song_parts = 1;
  song->song_part[0].part_idx = 0;
}

void project_init()
{
  project_initialized = true;
  memset(&project, 0, sizeof(project));
}

bool project_load(const char* filename)
{
  assert(true == project_initialized);

  m_line_counter = 0;

  if (NULL == filename) {
    default_project();
    columns_update();
    return true;
  }

  assert(MAX_FILE_NAME_LENGTH > strlen(filename));
  strncpy(project.filename, filename, MAX_FILE_NAME_LENGTH);

  file_t *fd = fopen(project.filename, "r");
  if (NULL == fd) {
    error("Unable to load project file '%s'.", project.filename);
    default_project();
  }
  else {
    /* WANNADO: Config file version string handling. */
    project_file_format(fd, MODE_READ);

    debug("Loaded project '%s' from file '%s'",
          project.name, project.filename);

    fclose(fd);
  }

  columns_update();

  return true;
}

static bool file_exists(const char* filename)
{
  file_t *fp = fopen(filename, "r");
  if (NULL != fp) {
    fclose(fp);
    return true;
  }
  return false;
}

bool project_save(const char* filename,
                  char* (*ask_for_project_filename)(char* filename, char* name),
                  bool (*ask_for_overwrite)(char* filename, char* name))
{
  char *f = NULL;

  if (false == project.changed) {
    debug("The project '%s' did not change since load, not saving",
          project.name);
    return true;
  }

  debug("Saving project '%s'.", project.name);

  if (NULL != filename) {
    debug("Filename provided for project '%s'.", project.name);
    f = (char*)filename;
  }

  assert(true == project_initialized);
  assert(NULL != ask_for_project_filename);
  assert(NULL != ask_for_overwrite);

  if (NULL == f) {
    debug("Checking for already exsisting filename in the project '%s'",
          project.name);
    if (strlen(project.filename) > 0) {
      f = project.filename;
    }
  }

  /*
   * If the file exists ask the user if he/she wants to overwrite it.
   */
  while ((f == NULL) || (file_exists(f))) {
    debug("Trying to save project '%s'",
          project.name);
    if ((f == NULL) || (true == ask_for_overwrite(f, project.name))) {
      break;
    }
    if (f == NULL) {
      debug("Have to ask for a new filename for project '%s'",
            project.name);
      char new_filename[MAX_FILE_NAME_LENGTH];
      memset(new_filename, 0, sizeof(new_filename));
      f = ask_for_project_filename(new_filename, project.name);
      if (NULL == f) {
        /*
         * The user changed his/her mind and wants to discard the project.
         */
        debug("User aborted");
        return false;
      }
    }
  }

  debug("Saving project '%s' to file '%s'.", project.name, f);

  file_t *fd = fopen(f, "w+");
  if (NULL == fd) {
    error("Unable to save project file '%s'.", f);
    return false;
  }

  project_file_format(fd, MODE_WRITE);

  debug("Saved project '%s' to file '%s'", project.name, project.filename);

  fclose(fd);
  return true;
}

#define update_note(pattern_idx, row_idx, track_idx, note_idx, key, vel) \
  update_key(pattern_idx, row_idx, track_idx, note_idx, key);           \
  update_velocity(pattern_idx, row_idx, track_idx, note_idx, vel)

#define update_effect(pattern_idx, row_idx, track_idx, note_idx, cmd, par) \
  update_command(pattern_idx, row_idx, track_idx, note_idx, cmd);       \
  update_parameter(pattern_idx, row_idx, track_idx, note_idx, par)

void project_update()
{
  const pattern_idx_t pattern_idx = get_pattern_idx();
  const row_idx_t row_idx = get_row_idx();
  const column_idx_t column_idx = columns_get_column_idx();
  const column_type_t column_type = columns_get_column_type(column_idx);
  const track_idx_t track_idx = columns_get_track_idx(column_idx);

  int events = event_count();

  if (false == get_edit()) {
    return;
  }

  bool inc_row = false;

  for (int idx = 0; idx < events; idx++) {
    event_type_args_t* args;
    event_get(idx, &args);
    switch (args->none.type) {
    case EVENT_TYPE_NOTE_ON:
      {
        /* This applies for master keyboard and computer keyboard only */
        if ((COLUMN_TYPE_NOTE != column_type) &&
            (COLUMN_TYPE_VELOCITY_1 != column_type) &&
            (COLUMN_TYPE_VELOCITY_2 != column_type)) {
          break;
        }
        const note_idx_t note_idx = columns_get_note_idx(column_idx);

        event_type_note_on_t* note_on = &args->event_type_note_on;
        /* TODO: Set on the correct track, and note idx */
        update_note(pattern_idx, row_idx, track_idx, note_idx,
                    note_on->note, note_on->velocity);
        inc_row = true;
        break;
      }
    case EVENT_TYPE_CONTROLLER:
      {
        if ((COLUMN_TYPE_COMMAND_1 != column_type) &&
            (COLUMN_TYPE_COMMAND_2 != column_type) &&
            (COLUMN_TYPE_PARAMETER_1 != column_type) &&
            (COLUMN_TYPE_PARAMETER_2 != column_type)) {
          break;
        }
        const effect_idx_t effect_idx = columns_get_effect_idx(column_idx);
        event_type_controller_t* controller = &args->event_type_controller;
        /* TODO: Set on the correct track, and effect idx */
        update_effect(pattern_idx, row_idx, track_idx, effect_idx,
                      controller->parameter, controller->value);
        inc_row = true;
        break;
      }
    case EVENT_TYPE_NOTE_OFF:
      break;
    }
  }

  if ((PROJECT_MODE_STOPPED == get_mode()) && (true == inc_row)) {
    update_row_idx(pattern_idx, row_idx + 1, project_get_pattern_rows(pattern_idx));
  }
}

#undef update_note
#undef update_effect

static void prepare_output_row(pattern_idx_t pattern_idx, row_idx_t row_idx)
{
  track_idx_t tracks = get_tracks();
  track_idx_t track_idx;
  for (track_idx = 0; track_idx < tracks; track_idx++) {
    effect_idx_t effects = get_effects(pattern_idx, track_idx);
    effect_idx_t effect_idx;
    device_idx_t device_idx = get_device_idx(track_idx);
    instrument_idx_t instrument_idx = get_instrument_idx(pattern_idx,
                                                         track_idx);
    for (effect_idx = 0; effect_idx < effects; effect_idx++) {
      effect_t* effect =
        &project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx];
      if ((0 != effect->command) && (0 != effect->parameter)) {
        update_instrument_control(device_idx,
                                  instrument_idx,
                                  effect->command,
                                  effect->parameter);
      }
    }
    note_idx_t notes = get_notes(pattern_idx, track_idx);
    note_idx_t note_idx;
    for (note_idx = 0; note_idx < notes; effect_idx++) {
      note_t* note =
        &project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx];
      if ((0 != note->key) && (0 != note->velocity)) {
        update_instrument_note_on(device_idx,
                                  instrument_idx,
                                  note->key,
                                  note->velocity);
      }
    }
  }

}

void project_step()
{
  const project_mode_t mode = get_mode();

  assert(PROJECT_MODE_STOPPED != mode);

  const pattern_idx_t pattern_idx = get_pattern_idx();
  const row_idx_t row_idx = get_row_idx();
  song_idx_t song_idx = get_song_idx();
  song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;
  part_idx_t part_idx = project.song[song_idx].song_part[song_part_idx].part_idx;
  part_pattern_idx_t part_pattern_idx = project.part[part_idx].part_pattern_idx;

  /*
   * Figure out what just happened :)
   */

  const bool project_playing = (PROJECT_MODE_PLAY_PROJECT == mode);

  const bool song_playing = ((PROJECT_MODE_PLAY_SONG == mode) ||
                             (true == project_playing));

  const bool part_playing = ((PROJECT_MODE_PLAY_PART == mode) ||
                             (true == song_playing));

  const bool last_pattern_row_processed =
    (get_row_idx() == get_pattern_rows());

  const bool last_song_processed =
    ((true == last_pattern_row_processed) &&
     (true == project_playing) &&
     (get_song_idx() == get_songs()));
  const bool last_song_part_processed =
    ((true == last_pattern_row_processed) &&
     (true == song_playing) &&
     (get_song_part_idx() == get_song_parts()));
  const bool last_part_pattern_processed =
    ((true == last_pattern_row_processed) &&
     (true == part_playing) &&
     (get_part_pattern_idx() == get_part_patterns()));

  if (true == last_song_processed) {
    update_song_idx(0);
  }

  if (true == last_song_part_processed) {
    song_idx++;
    update_song_idx(song_idx);
  }

  if (true == last_part_pattern_processed) {
    part_pattern_idx++;
    update_part_pattern_idx(song_idx, part_pattern_idx);
  }

  prepare_output_row(pattern_idx, row_idx + 1);
}

void project_set_edit(const bool edit)
{
  project.edit = edit;
}

bool project_get_edit()
{
  return project.edit;
}

void project_set_quantization(const quantization_t quantization)
{
  project.quantization = quantization;
}

quantization_t project_get_quantization()
{
  return project.quantization;
}

void project_set_mode(const project_mode_t mode)
{
  if (PROJECT_MODE_STOPPED != mode) {
    assert((mode == PROJECT_MODE_PLAY_PROJECT) ||
           (mode == PROJECT_MODE_PLAY_SONG) ||
           (mode == PROJECT_MODE_PLAY_PART) ||
           (mode == PROJECT_MODE_PLAY_PATTERN));

    song_idx_t song_idx = project.song_idx;
    song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;
    part_idx_t part_idx =
      project.song[song_idx].song_part[song_part_idx].part_idx;
    part_pattern_idx_t part_pattern_idx =
      project.part[part_idx].part_pattern_idx;
    pattern_idx_t pattern_idx =
      project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx;

    /* Fall-through switch/case for partial reset of replay. */
    switch (mode) {
    case PROJECT_MODE_PLAY_PROJECT:
      update_song_idx(0);
      song_idx = 0;
    case PROJECT_MODE_PLAY_SONG:
      update_song_part_idx(song_idx, 0);
      song_part_idx = 0;
      part_idx = project.song[song_idx].song_part[song_part_idx].part_idx;
      update_part_idx(song_idx, song_part_idx, part_idx);
    case PROJECT_MODE_PLAY_PART:
      update_part_pattern_idx(part_idx, 0);
      part_pattern_idx = 0;
      pattern_idx =
        project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx;
      update_pattern_idx(part_idx, part_pattern_idx, pattern_idx);
    case PROJECT_MODE_PLAY_PATTERN: {
      //update_row(pattern_idx, 0);
      break;
    }
    default:
      assert(false);
    }
  }
  project.mode = mode;
}

void project_set_song_idx(const song_idx_t song_idx)
{
  project.song_idx = song_idx;
}

void project_set_song_part_idx(const song_idx_t song_idx,
                               const song_part_idx_t song_part_idx)
{
  assert(song_idx == project.song_idx);

  project.song[song_idx].song_part_idx = song_part_idx;
}

song_part_idx_t project_get_song_part_idx(const song_idx_t song_idx)
{
  return project.song[song_idx].song_part_idx;
}

song_part_idx_t project_get_song_parts(const song_idx_t song_idx)
{
  return project.song[song_idx].song_parts;
}

void project_set_part_idx(const song_idx_t song_idx,
                          const song_part_idx_t song_part_idx,
                          const part_idx_t part_idx)
{
  assert(song_idx == project.song_idx);
  assert(song_part_idx == project.song[song_idx].song_part_idx);

  project.song[song_idx].song_part[song_part_idx].part_idx = part_idx;
}

part_idx_t project_get_part_idx(const song_idx_t song_idx,
                                const song_part_idx_t song_part_idx)
{
  return project.song[song_idx].song_part[song_part_idx].part_idx;
}

void project_set_part_pattern_idx(const part_idx_t part_idx,
                                  const part_pattern_idx_t part_pattern_idx)
{
  const song_idx_t song_idx = project.song_idx;
  const song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;

  assert(part_idx ==
         project.song[song_idx].song_part[song_part_idx].part_idx);

  project.part[part_idx].part_pattern_idx = part_pattern_idx;
}

part_pattern_idx_t project_get_part_pattern_idx(const part_idx_t part_idx)
{
  return project.part[part_idx].part_pattern_idx;
}

part_pattern_idx_t project_get_part_patterns(part_idx_t part_idx)
{
  return project.part[part_idx].part_patterns;
}

void project_set_pattern_idx(const part_idx_t part_idx,
                             const part_pattern_idx_t part_pattern_idx,
                             const pattern_idx_t pattern_idx)
{
  const song_idx_t song_idx = project.song_idx;
  const song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;

  assert(part_idx ==
         project.song[song_idx].song_part[song_part_idx].part_idx);
  assert(part_pattern_idx == project.part[part_idx].part_pattern_idx);

  project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx =
    pattern_idx;
}

pattern_idx_t project_get_pattern_idx(const part_idx_t part_idx,
                                      const part_pattern_idx_t part_pattern_idx)
{
  return project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx;
}

void project_set_pattern_rows(const pattern_idx_t pattern_idx,
                              const row_idx_t rows)
{
  project.pattern[pattern_idx].rows = rows;
}

row_idx_t project_get_pattern_rows(const pattern_idx_t pattern_idx)
{
  return project.pattern[pattern_idx].rows;
}

void project_set_row_idx(const row_idx_t row_idx)
{
  project.row_idx = row_idx;
}

row_idx_t project_get_row_idx()
{
  return project.row_idx;
}

void project_set_key(const pattern_idx_t pattern_idx,
                     const row_idx_t row_idx,
                     const track_idx_t track_idx,
                     const note_idx_t note_idx,
                     const key_t key)
{
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].key = key;
}

void project_set_velocity(const pattern_idx_t pattern_idx,
                          const row_idx_t row_idx,
                          const track_idx_t track_idx,
                          const note_idx_t note_idx,
                          const velocity_t velocity)
{
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].velocity = velocity;
}

velocity_t project_get_velocity(const pattern_idx_t pattern_idx,
                                const row_idx_t row_idx,
                                const track_idx_t track_idx,
                                const note_idx_t note_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].velocity;
}

void project_set_command(const pattern_idx_t pattern_idx,
                         const row_idx_t row_idx,
                         const track_idx_t track_idx,
                         const effect_idx_t effect_idx,
                         const command_t command)
{
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].command = command;
}

command_t project_get_command(const pattern_idx_t pattern_idx,
                              const row_idx_t row_idx,
                              const track_idx_t track_idx,
                              const effect_idx_t effect_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].command;
}

void project_set_parameter(const pattern_idx_t pattern_idx,
                           const row_idx_t row_idx,
                           const track_idx_t track_idx,
                           const effect_idx_t effect_idx,
                           const parameter_t parameter)
{
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].parameter = parameter;
}

parameter_t project_get_parameter(const pattern_idx_t pattern_idx,
                                  const row_idx_t row_idx,
                                  const track_idx_t track_idx,
                                  const effect_idx_t effect_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].parameter;
}
