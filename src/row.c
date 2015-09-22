#include "config.h"

#include "studio.h"

#include "row.h"

extern track_idx_t project_get_tracks();
extern device_idx_t project_get_device_idx(const track_idx_t track_idx);
extern instrument_idx_t project_get_instrument_idx(pattern_idx_t pattern_idx,
                                                   track_idx_t track_idx);

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

void row_append(char* buf, pattern_idx_t pattern_idx, row_t* row,
                track_idx_t tracks)
{
  for (track_idx_t track_idx = 0; track_idx < tracks; track_idx++) {

    const device_idx_t device_idx = project_get_device_idx(track_idx);
    const instrument_idx_t instrument_idx =
      project_get_instrument_idx(pattern_idx, track_idx);
    const note_idx_t polyphony = studio_get_polyphony(device_idx, instrument_idx);
    const effect_idx_t parameters = studio_get_parameters(device_idx,
                                                          instrument_idx);

    track_row_append(buf, &row->track_row[track_idx], polyphony, parameters);
    if (track_idx != tracks - 1) {
      strcat(buf, "|");
    }
  }
}

void row_file_format(file_format_args(row_t),
                     const pattern_idx_t pattern_idx,
                     const track_idx_t tracks)
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
    row_append(buf, pattern_idx, data, tracks);
  }

  fint(tempo_relative_to_pattern);

  /* Special */
  FSTR_1(file, mode, "%s", buf, prefix);

  if (MODE_READ == mode) {
    for (track_row_idx_t idx = 0; idx < tracks; idx++) {

      const device_idx_t device_idx = project_get_device_idx(idx);
      const instrument_idx_t instrument_idx =
        project_get_instrument_idx(pattern_idx, idx);

      int polyphony = studio_get_polyphony(device_idx, instrument_idx);
      int parameters = studio_get_parameters(device_idx, instrument_idx);

      track_row_extract(&p, &data->track_row[idx], polyphony, parameters);

      if (idx != tracks - 1) {
        p++;
      }
    }
  }
}
