#include <stdio.h>
#include <stdarg.h>

#include "types.h"

#ifndef _ERROR_H_
#define _ERROR_H_

bool debug_enabled;

static inline int __eprintf (const char *format, ...)
{
  va_list arg;
  int done;

  fprintf(stderr, "ERROR: ");

  va_start(arg, format);
  done = vfprintf(stderr, format, arg);
  va_end(arg);

  fprintf(stderr, "\n");

  return done;
}

static inline int __wprintf (const char *format, ...)
{
  va_list arg;
  int done;

  fprintf(stdout, "WARNING: ");

  va_start(arg, format);
  done = vfprintf(stderr, format, arg);
  va_end(arg);

  fprintf(stderr, "\n");

  return done;
}

static inline int __dprintf (const char *format, ...)
{
  va_list arg;
  int done;

  fprintf(stdout, "DEBUG: ");

  va_start(arg, format);
  done = vfprintf(stdout, format, arg);
  va_end(arg);

  fprintf(stdout, "\n");

  return done;
}

#define error __eprintf

#define warning __wprintf

#define debug if (true == debug_enabled) __dprintf

#endif
