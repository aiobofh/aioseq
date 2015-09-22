/*
 * A project contains patterns, song-parts and songs.
 */
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "error.h"
#include "constants.h"
#include "defaults.h"
#include "types.h"
#include "config.h"
#include "event.h"
#include "studio.h"
#include "columns.h"
#include "update.h"

#include "song.h"
#include "part.h"
#include "track.h"
#include "pattern.h"

#include "project.h"

extern int m_line_counter;

typedef struct __attribute__((__packed__))
{
  char name[MAX_NAME_LENGTH + 1];
  tempo_t tempo;
  track_idx_t tracks;
  track_t track[MAX_TRACKS + 1];
  pattern_idx_t patterns;
  pattern_t pattern[MAX_PATTERNS + 1];
  part_idx_t parts;
  part_t part[MAX_PARTS + 1];
  song_idx_t songs;
  song_t song[MAX_SONGS + 1];

  bool changed;

  char filename[MAX_FILE_NAME_LENGTH + 1];
  edit_t edit;
  project_mode_t mode;
  song_idx_t song_idx;
  row_idx_t row_idx;

  quantization_t quantization;

} project_t;

project_t project;

static bool project_initialized = false;

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

  const track_idx_t tracks = project_get_tracks();

  fstr(name);
  fint(tempo);
  farray(tracks, track);
  farray_generate_pattern_idx_tracks(patterns, pattern);
  farray(parts, part);
  farray(songs, song);
}

static void default_pattern(pattern_idx_t pattern_idx)
{
  pattern_t* pattern = &project.pattern[pattern_idx];
  debug("Creating new default pattern");

  strncpy(pattern->name, DEFAULT_PATTERN_NAME, MAX_NAME_LENGTH);
  /* TODO: Use some kind of config file info default pattern length. */
  pattern->rows = 64;

  track_idx_t tracks = project_get_tracks();

  pattern->pattern_tracks = tracks;

  instrument_idx_t instrument_idx = 0;
  setting_idx_t setting_idx = 0;

  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {
    /* If creating a new pattern copy from the previous one if available */
    if (pattern_idx > 0) {
      instrument_idx = project.pattern[pattern_idx - 1].pattern_track[track_idx].instrument_idx;
      setting_idx = project.pattern[pattern_idx - 1].pattern_track[track_idx].setting_idx;
    }
    pattern->pattern_track[track_idx].instrument_idx = instrument_idx;
    pattern->pattern_track[track_idx].setting_idx = setting_idx;
  }
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

  const device_idx_t devices = studio_get_devices();

  project.tracks = devices;

  for (track_idx_t track_idx = 0; track_idx < devices; ++track_idx) {
    track_t* track = &project.track[track_idx];
    strncpy(track->name, DEFAULT_TRACK_NAME, MAX_NAME_LENGTH);
    track->device_idx = track_idx;
  }

  project.patterns = 1;

  default_pattern(0);

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

static void suggest_filename(char* filename, char* name, char* ext)
{
  for (size_t i = 0; i < strlen(name); i++) {
    if (name[i] == ' ')
      filename[i] = '_';
    else
      filename[i] = tolower(name[i]);
  }
  filename[strlen(name)] = 0;
  strcat(filename, ext);
}

bool project_save(const char* filename,
                  char* (*ask_for_project_filename)(char* filename,
                                                    char* name),
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
    bool result = ask_for_overwrite(f, project.name);
    if (false == result) {
      debug("Have to ask for a new filename for project '%s'",
            project.name);
      char new_filename[MAX_FILE_NAME_LENGTH];
      memset(new_filename, 0, sizeof(new_filename));
      suggest_filename(new_filename, project.name, "asp");
      f = ask_for_project_filename(new_filename, project.name);
    }
    else {
      break;
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

void project_reset()
{
  row_idx_t rows = project.pattern[0].rows;
  update_song_idx(0);
  update_song_part_idx(0, 0);
  update_part_idx(0, 0, 0);
  update_part_pattern_idx(0, 0);
  update_pattern_idx(0, 0, 0);
  update_row_idx(0, 0, rows);
  update_commit();
}


void project_get_pattern_row(char* buf, pattern_idx_t pattern_idx,
                             row_idx_t row_idx) {
  const track_idx_t tracks = project_get_tracks();

  buf[0] = 0;
  row_append(buf, pattern_idx, &project.pattern[pattern_idx].row[row_idx], tracks);
}


void project_update()
{
  CONST_GET(project, song_idx)();
  CONST_GET(project, song_part_idx)(song_idx);
  CONST_GET(project, part_idx)(song_idx, song_part_idx);
  CONST_GET(project, part_pattern_idx)(part_idx);
  CONST_GET(project, pattern_idx)(part_idx, part_pattern_idx);
  CONST_GET(project, row_idx)();

  CONST_GET(columns, column_idx)();
  CONST_GET(columns, column_type)(column_idx);
  CONST_GET(columns, track_idx)(column_idx);

  int events = event_count();

  if (false == project_get_edit()) {
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

  if ((PROJECT_MODE_STOPPED == project_get_project_mode()) &&
      (true == inc_row)) {
    update_row_idx(pattern_idx, row_idx + 1, project_get_pattern_rows(pattern_idx));
  }
}

#undef update_note
#undef update_effect

static void note_off(const pattern_idx_t pattern_idx,
                     const track_idx_t track_idx,
                     const note_idx_t note_idx) {
  device_idx_t device_idx = project_get_device_idx(track_idx);
  instrument_idx_t instrument_idx =
    project_get_instrument_idx(pattern_idx, track_idx);

  if (0 != project.track[track_idx].note_on[note_idx]) {

    update_instrument_note_off(track_idx,
                               note_idx,
                               device_idx,
                               instrument_idx,
                               project.track[track_idx].note_on[note_idx],
                               127);

  }
}

void project_notes_off(const pattern_idx_t pattern_idx)
{
  const track_idx_t tracks = project_get_tracks();
  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {
    const note_idx_t notes = project_get_notes(pattern_idx, track_idx);
    for (note_idx_t note_idx = 0; note_idx < notes; note_idx++) {
      note_off(pattern_idx, track_idx, note_idx);
    }
  }
}

static void prepare_output_row(pattern_idx_t pattern_idx, row_idx_t row_idx)
{
  track_idx_t tracks = project_get_tracks();
  track_idx_t track_idx;
  for (track_idx = 0; track_idx < tracks; track_idx++) {
    effect_idx_t effects = project_get_effects(pattern_idx, track_idx);
    effect_idx_t effect_idx;
    device_idx_t device_idx = project_get_device_idx(track_idx);
    instrument_idx_t instrument_idx =
      project_get_instrument_idx(pattern_idx, track_idx);
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
    note_idx_t notes = project_get_notes(pattern_idx, track_idx);
    note_idx_t note_idx;
    for (note_idx = 0; note_idx < notes; note_idx++) {
      note_t* note =
        &project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx];

      note_off(pattern_idx, track_idx, note_idx);

      if ((0 != note->key) && (0 != note->velocity)) {
        update_instrument_note_on(track_idx,
                                  note_idx,
                                  device_idx,
                                  instrument_idx,
                                  note->key,
                                  note->velocity);
      }
    }
  }

}

void project_step()
{
  CONST_GET(project, project_mode)();

  assert(PROJECT_MODE_STOPPED != project_mode);

  song_idx_t song_idx = project_get_song_idx();
  song_part_idx_t song_part_idx = project_get_song_part_idx(song_idx);
  part_idx_t part_idx = project_get_part_idx(song_idx, song_part_idx);
  part_pattern_idx_t part_pattern_idx =
    project_get_part_pattern_idx(part_idx);

  const song_idx_t songs = project_get_songs();
  const song_part_idx_t song_parts = project_get_song_parts(song_idx);

  const part_pattern_idx_t part_patterns =
    project_get_part_patterns(part_idx);

  CONST_GET(project, pattern_idx)(part_idx, part_pattern_idx);

  row_idx_t row_idx = project_get_row_idx();
  const row_idx_t rows = project_get_pattern_rows(pattern_idx);

  prepare_output_row(pattern_idx, row_idx + 1);

  /*
   * Figure out what just happened :)
   */

  const bool project_playing = (PROJECT_MODE_PLAY_PROJECT == project_mode);

  const bool song_playing = ((PROJECT_MODE_PLAY_SONG == project_mode) ||
                             (true == project_playing));

  const bool part_playing = ((PROJECT_MODE_PLAY_PART == project_mode) ||
                             (true == song_playing));

  const bool last_pattern_row_processed =
    (row_idx == rows - 1);

  const bool last_song_processed =
    ((true == last_pattern_row_processed) &&
     (true == project_playing) &&
     (song_idx == songs - 1));
  const bool last_song_part_processed =
    ((true == last_pattern_row_processed) &&
     (true == song_playing) &&
     (song_part_idx == song_parts - 1));
  const bool last_part_pattern_processed =
    ((true == last_pattern_row_processed) &&
     (true == part_playing) &&
     (part_pattern_idx == part_patterns - 1));

  /* HMM.. Why is this so hard?! */

  if (true == last_pattern_row_processed) {
    debug("End of pattern");
    if (part_playing) {
      if (true == last_part_pattern_processed) {
        debug("End if part");
        if (song_playing) {
          if (true == last_song_part_processed) {
            debug("End of song");
            if (project_playing) {
              if (true == last_song_processed) {
                debug("End of project");
                song_idx = 0;
              }
              else {
                debug("Next song");
                song_idx++;
              }
              update_song_idx(song_idx);
            }
            song_part_idx = 0;
          }
          else {
            debug("Next part");
            song_part_idx++;
          }
          update_song_part_idx(song_idx, song_part_idx);
        }
        part_pattern_idx = 0;
      }
      else {
        debug("Next pattern");
        part_pattern_idx++;
      }
      part_idx = project.song[song_idx].song_part[song_part_idx].part_idx;
      update_part_pattern_idx(part_idx, part_pattern_idx);
    }
    row_idx = 0;
  }
  else {
    row_idx++;
  }
  update_row_idx(pattern_idx, row_idx, rows);
}

void project_set_instrument_note_on(track_idx_t track_idx,
                                    note_idx_t note_idx,
                                    key_t key)
{
  project.track[track_idx].note_on[note_idx] = key;
}

void project_set_instrument_note_off(track_idx_t track_idx,
                                     note_idx_t note_idx,
                                     key_t key)
{
  if (key == project.track[track_idx].note_on[note_idx]) {
    project.track[track_idx].note_on[note_idx] = 0;
  }
}

/************************************************************************
 * Edit mode
 */
void project_set_edit(const edit_t edit)
{
  project.edit = edit;
}

bool project_get_edit()
{
  return project.edit;
}

/************************************************************************
 * Playback mode
 */
void project_set_project_mode(const project_mode_t mode)
{
  song_idx_t song_idx = project.song_idx;
  song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;
  part_idx_t part_idx =
    project.song[song_idx].song_part[song_part_idx].part_idx;
  part_pattern_idx_t part_pattern_idx =
    project.part[part_idx].part_pattern_idx;
  pattern_idx_t pattern_idx =
    project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx;

  track_idx_t tracks = project_get_tracks();
  track_idx_t track_idx;
  for (track_idx = 0; track_idx < tracks; track_idx++) {
    const note_idx_t notes = project_get_notes(pattern_idx, track_idx);
    for (note_idx_t note_idx  = 0; note_idx < notes; note_idx++) {
      note_off(pattern_idx, track_idx, note_idx);
    }
  }

  if (PROJECT_MODE_STOPPED != mode) {
    assert((mode == PROJECT_MODE_PLAY_PROJECT) ||
           (mode == PROJECT_MODE_PLAY_SONG) ||
           (mode == PROJECT_MODE_PLAY_PART) ||
           (mode == PROJECT_MODE_PLAY_PATTERN));

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
      update_row_idx(pattern_idx, 0, project.pattern[pattern_idx].rows);
      break;
    }
    default:
      assert(false);
    }
  }
  else {

  }

  project.mode = mode;
}
project_mode_t project_get_project_mode()
{
  return project.mode;
}

/************************************************************************
 * Playback mode
 */
tempo_t project_get_tempo()
{
  CONST_GET(project, song_idx)();
  CONST_GET(project, song_part_idx)(song_idx);
  CONST_GET(project, part_idx)(song_idx, song_part_idx);
  CONST_GET(project, part_pattern_idx)(part_idx);
  CONST_GET(project, pattern_idx)(part_idx, part_pattern_idx);
  CONST_GET(project, row_idx)(pattern_idx);

  return project.tempo +
    (project.song[song_idx].tempo_relative_to_project) +
    (project.part[part_idx].tempo_relative_to_song) +
    (project.pattern[pattern_idx].tempo_relative_to_part) +
    (project.pattern[pattern_idx].row[row_idx].tempo_relative_to_pattern);

}

/************************************************************************
 * Quantization
 */
void project_set_quantization(const quantization_t quantization)
{
  project.quantization = quantization;
}

quantization_t project_get_quantization()
{
  return project.quantization;
}

/************************************************************************
 * Device index
 */
device_idx_t project_get_device_idx(const track_idx_t track_idx)
{
  return project.track[track_idx].device_idx;
}

/************************************************************************
 * Instrument index
 */
instrument_idx_t project_get_instrument_idx(pattern_idx_t pattern_idx,
                                            track_idx_t track_idx)
{
  return
    project.pattern[pattern_idx].pattern_track[track_idx].instrument_idx;
}

/************************************************************************
 * Setting index
 */
setting_idx_t project_get_setting_idx(pattern_idx_t pattern_idx,
                                      track_idx_t track_idx)
{
  return project.pattern[pattern_idx].pattern_track[track_idx].setting_idx;
}


/************************************************************************
 * Songs
 */
song_idx_t project_get_songs()
{
  return project.songs;
}

/************************************************************************
 * Song index
 */
void project_set_song_idx(const song_idx_t song_idx)
{
  project.song_idx = song_idx;
}

song_idx_t project_get_song_idx()
{
  return project.song_idx;
}

char* project_get_song_name(const song_idx_t song_idx)
{
  return project.song[song_idx].name;
}


/************************************************************************
 * Song part index
 */
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

/************************************************************************
 * Song parts
 */
void project_set_song_parts(const song_idx_t song_idx,
                            const song_part_idx_t song_parts)
{
  assert(song_idx == project.song_idx);

  project.changed = true;

  project.song[song_idx].song_parts = song_parts;
}

song_part_idx_t project_get_song_parts(const song_idx_t song_idx)
{
  return project.song[song_idx].song_parts;
}

/************************************************************************
 * Tracks
 */
track_idx_t project_get_tracks()
{
  return project.tracks;
}

/************************************************************************
 * Part index
 */
void project_set_part_idx(const song_idx_t song_idx,
                          const song_part_idx_t song_part_idx,
                          const part_idx_t part_idx)
{
  assert(song_idx == project.song_idx);
  assert(song_part_idx == project.song[song_idx].song_part_idx);

  project.changed = true;

  project.song[song_idx].song_part[song_part_idx].part_idx = part_idx;
}

part_idx_t project_get_part_idx(const song_idx_t song_idx,
                                const song_part_idx_t song_part_idx)
{
  return project.song[song_idx].song_part[song_part_idx].part_idx;
}

char* project_get_part_name(const part_idx_t part_idx)
{
  return project.part[part_idx].name;
}

/************************************************************************
 * Pattern index
 */
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

void project_set_patterns(const pattern_idx_t patterns)
{
  const pattern_idx_t current = project_get_patterns();
  debug("Current %d patterns %d", current, patterns);
  for (pattern_idx_t idx = current - 1; idx < patterns; idx++) {
    if (project_get_pattern_rows(idx) == 0) {
      default_pattern(idx);
    }
  }
  project.patterns = patterns;
}

pattern_idx_t project_get_patterns()
{
  return project.patterns;
}

/************************************************************************
 * Part patterns
 */
void project_set_part_patterns(const part_idx_t part_idx,
                               const part_pattern_idx_t part_patterns)
{
  const song_idx_t song_idx = project.song_idx;
  const song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;

  assert(part_idx ==
         project.song[song_idx].song_part[song_part_idx].part_idx);

  project.changed = true;

  project.part[part_idx].part_patterns = part_patterns;
}

part_pattern_idx_t project_get_part_patterns(part_idx_t part_idx)
{
  return project.part[part_idx].part_patterns;
}

/************************************************************************
 * Pattern index
 */
void project_set_pattern_idx(const part_idx_t part_idx,
                             const part_pattern_idx_t part_pattern_idx,
                             const pattern_idx_t pattern_idx)
{
  const pattern_idx_t patterns = project_get_patterns();
  const song_idx_t song_idx = project.song_idx;
  const song_part_idx_t song_part_idx = project.song[song_idx].song_part_idx;

  assert(part_idx ==
         project.song[song_idx].song_part[song_part_idx].part_idx);
  assert(part_pattern_idx == project.part[part_idx].part_pattern_idx);

  project.changed = true;

  if (pattern_idx >= patterns - 1) {
    debug("Have to update patterns");
    project_set_patterns(pattern_idx + 1);
  }

  project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx =
    pattern_idx;
}

pattern_idx_t project_get_pattern_idx(const part_idx_t part_idx,
                                      const part_pattern_idx_t part_pattern_idx)
{
  return project.part[part_idx].part_pattern[part_pattern_idx].pattern_idx;
}

char* project_get_pattern_name(const pattern_idx_t pattern_idx)
{
  return project.pattern[pattern_idx].name;
}

/************************************************************************
 * Pattern rows
 */
void project_set_pattern_rows(const pattern_idx_t pattern_idx,
                              const row_idx_t rows)
{
  project.changed = true;
  project.pattern[pattern_idx].rows = rows;
}

row_idx_t project_get_pattern_rows(const pattern_idx_t pattern_idx)
{
  return project.pattern[pattern_idx].rows;
}

/************************************************************************
 * Row index
 */
void project_set_row_idx(const row_idx_t row_idx)
{
  project.row_idx = row_idx;
}

row_idx_t project_get_row_idx()
{
  return project.row_idx;
}

/************************************************************************
 * Notes
 */
note_idx_t project_get_notes(const pattern_idx_t pattern_idx,
                             const track_idx_t track_idx)
{
  const device_idx_t device_idx = project_get_device_idx(track_idx);
  const instrument_idx_t instrument_idx =
    project_get_instrument_idx(pattern_idx, track_idx);

  return studio_get_polyphony(device_idx, instrument_idx);
}

/************************************************************************
 * Effects
 */
effect_idx_t project_get_effects(const pattern_idx_t pattern_idx,
                                 const track_idx_t track_idx)
{
  const device_idx_t device_idx = project_get_device_idx(track_idx);
  const instrument_idx_t instrument_idx =
    project_get_instrument_idx(pattern_idx, track_idx);

  return studio_get_parameters(device_idx, instrument_idx);
}

/************************************************************************
 * Key
 */
void project_set_key(const pattern_idx_t pattern_idx,
                     const row_idx_t row_idx,
                     const track_idx_t track_idx,
                     const note_idx_t note_idx,
                     const key_t key)
{
  project.changed = true;
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].key = key;
}

/************************************************************************
 * Velocity
 */
void project_set_velocity(const pattern_idx_t pattern_idx,
                          const row_idx_t row_idx,
                          const track_idx_t track_idx,
                          const note_idx_t note_idx,
                          const velocity_t velocity)
{
  project.changed = true;
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].velocity = velocity;
}

velocity_t project_get_velocity(const pattern_idx_t pattern_idx,
                                const row_idx_t row_idx,
                                const track_idx_t track_idx,
                                const note_idx_t note_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].note[note_idx].velocity;
}

/************************************************************************
 * Command
 */
void project_set_command(const pattern_idx_t pattern_idx,
                         const row_idx_t row_idx,
                         const track_idx_t track_idx,
                         const effect_idx_t effect_idx,
                         const command_t command)
{
  project.changed = true;
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].command = command;
}

command_t project_get_command(const pattern_idx_t pattern_idx,
                              const row_idx_t row_idx,
                              const track_idx_t track_idx,
                              const effect_idx_t effect_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].command;
}

/************************************************************************
 * Parameter
 */
void project_set_parameter(const pattern_idx_t pattern_idx,
                           const row_idx_t row_idx,
                           const track_idx_t track_idx,
                           const effect_idx_t effect_idx,
                           const parameter_t parameter)
{
  project.changed = true;
  project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].parameter = parameter;
}

parameter_t project_get_parameter(const pattern_idx_t pattern_idx,
                                  const row_idx_t row_idx,
                                  const track_idx_t track_idx,
                                  const effect_idx_t effect_idx)
{
  return project.pattern[pattern_idx].row[row_idx].track_row[track_idx].effect[effect_idx].parameter;
}
