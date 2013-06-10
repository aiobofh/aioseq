/**
 * @file Part.cpp
 *
 * Part implementation.
 *
 * A Part is a section of a song, for example a verse or chorus.
 */

#include "AbstractPatterns.h"
#include "Part.h"

#include <string>

Part::Part() {
  this->name = NULL;
  this->patterns = NULL;
}

Part::Part(const string *name) {
  this->name = NULL;
  this->patterns = NULL;
  set_name(name);
}

Part::Part(AbstractPatterns *patterns) {
  this->name = NULL;
  this->patterns = patterns;
}

Part::Part(const string *name, AbstractPatterns *patterns) {
  this->name = NULL;
  this->patterns = patterns;
  set_name(name);
}

void Part::set_name(const string *name) {
  if (NULL != this->name) {
    delete this->name;
  }
  this->name = new string(*name);
}

string *Part::get_name() {
  return this->name;
}

AbstractPatterns *Part::get_patterns() {
  return this->patterns;
}
