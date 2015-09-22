#include "part_pattern.h"

#include "part.h"

void part_file_format(file_format_args(part_t))
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
