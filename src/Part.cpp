/**
 * @file Part.cpp
 *
 * Part implementation.
 *
 * A Part is a section of a song, for example a verse or chorus.
 */

#include "Part.h"

#include <string>

Part::Part() {
  this->name = NULL;
}

Part::Part(string *name) {
  this->name = NULL;
  set_name(name);
}

void Part::set_name(string *name) {
  if (NULL != this->name) {
    delete this->name;
  }
  this->name = new string(*name);
}

string *Part::get_name() {
  return this->name;
}
