/**
 * @file debug.hh
 */
#ifndef _DEBUG_HH_
#define _DEBUG_HH_

#include <iostream>

/**
 * Helper macro to send debug text to standard error output.
 */
#ifdef DEBUG
#define debug(a)                      \
  std::cerr << "DEBUG: "              \
  << a                                \
  << endl
#else
#define debug(a)
#endif

#endif
