/**
 * @file Pattern.cpp
 */

#include "Pattern.h"

using std::list;

Pattern::Pattern(AbstractPatternRows *rows) {
  this->rows = rows;
}

AbstractPatternRows *Pattern::get_pattern_rows() {
  return this->rows;
}
