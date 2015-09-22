#include "config.h"
#include "song_part.h"

#include "song.h"

void song_file_format(file_format_args(song_t))
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
