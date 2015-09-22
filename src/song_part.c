#include "song_part.h"

void song_part_file_format(file_format_args(song_part_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->part_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(part_idx);
}
