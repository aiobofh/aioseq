#include "config.h"

#include "row.h"
#include "pattern_track.h"

#include "pattern.h"

void pattern_file_format(file_format_args(pattern_t),
                         const pattern_idx_t pattern_idx,
                         const track_idx_t tracks)
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
  farray_with_pattern_idx_tracks(rows, row);
  farray(pattern_tracks, pattern_track);
}
