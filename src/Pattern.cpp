/**
 * @file Pattern.cpp
 */

#include "Pattern.h"

using std::list;

Pattern::Pattern(list<AbstractPatternRow*> *rows) {
  this->rows = rows;
}

list<AbstractPatternRow*> *Pattern::get_pattern_rows() {
  return this->rows;
}
