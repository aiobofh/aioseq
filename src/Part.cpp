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

Part::Part() : Name(DEFAULT_PART_NAME) {
  this->patterns = NULL;
}


Part::Part(const string *name) : Name(name) {
  this->patterns = NULL;
}


Part::Part(AbstractPatterns *patterns) : Name(DEFAULT_PART_NAME) {
  this->patterns = patterns;
}


Part::Part(const string *name, AbstractPatterns *patterns) : Name(name) {
  this->patterns = patterns;
}


AbstractPatterns *Part::get_patterns() {
  return this->patterns;
}


void Part::add_pattern(AbstractPattern *pattern) {
  this->patterns->push_back(pattern);
}


bool Part::pattern_is_used(AbstractPattern *pattern) {
  AbstractPatterns::iterator i;
  for (i = this->patterns->begin(); i != this->patterns->end(); ++i) {
    if (*i == pattern) {
      return true;
    }
  }
  return false;
}


void Part::insert_pattern(AbstractPattern *before, AbstractPattern *pattern) {
  AbstractPatterns::iterator i;
  for (i = this->patterns->begin(); i != this->patterns->end(); ++i) {
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
