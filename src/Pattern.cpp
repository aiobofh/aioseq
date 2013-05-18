/**
 * @file Pattern.cpp
 */

#include "Pattern.h"

using std::list;

Pattern::Pattern(list<AbstractPatternRow*> *rows) {
  this->rows = rows;
}
