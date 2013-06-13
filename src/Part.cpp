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

Part::~Part() {
  if (NULL != this->name) {
    delete this->name;
  }
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

void Part::add_pattern(AbstractPattern *pattern) {
  this->patterns->push_back(pattern);
}


void Part::insert_pattern(AbstractPattern *before, AbstractPattern *pattern) {
  AbstractPatterns::iterator i;
  for (i = this->patterns->begin(); i != this->patterns->end(); ++ i) {
    if (*i == before) {
      this->patterns->insert(i, pattern);
      return;
    }
  }
  /*
   * @todo Evaluate if this method shall throw an exception if the before-
   *       pattern was not in the list. For now just add the pattern instead.
   */
  add_pattern(pattern);
}


void Part::delete_pattern(AbstractPattern *pattern) {
  this->patterns->remove(pattern);
}
