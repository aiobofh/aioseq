/**
 * @file error.hh
 */
#ifndef _ERROR_HH_
#define _ERROR_HH_

#include <iostream>

/// Output to standard error.
#define error(a)                      \
  std::cerr << "ERROR: "              \
  << a                                \
  << endl

#endif
