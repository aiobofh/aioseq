#include <stdlib.h>

#include "config.h"

#include "pattern_track.h"

void pattern_track_file_format(file_format_args(pattern_track_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->instrument_idx) +
                                  sizeof(data->setting_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(instrument_idx);
  fint(setting_idx);
}
