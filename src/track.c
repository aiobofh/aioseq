#include "config.h"

#include "track.h"

void track_file_format(file_format_args(track_t))
{
  fstr(name);
  fint(device_idx);
}
