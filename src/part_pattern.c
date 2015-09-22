#include "config.h"

#include "part_pattern.h"

void part_pattern_file_format(file_format_args(part_pattern_t))
{
  /* Force programmers to keep file format updated with struct design */
  const size_t serialized_size = (sizeof(data->pattern_idx));
  const size_t ignored_size = (0);

  assert(sizeof(*data) == (serialized_size + ignored_size));

  fint(pattern_idx);
}
