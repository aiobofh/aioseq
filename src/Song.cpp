/**
 * @file Song.cpp
 *
 * Song implementation.
 */

#include "Song.h"
#include "AbstractPart.h"
#include "AbstractParts.h"

Song::Song() {
  this->name = NULL;
  this->parts = NULL;
}

Song::Song(const string *name) {
  this->name = NULL;
  this->parts = NULL;
  set_name(name);
}

Song::Song(AbstractParts *parts) {
  this->name = NULL;
  this->parts = parts;
}

Song::Song(const string *name, AbstractParts *parts) {
  this->name = NULL;
  this->parts = parts;
  set_name(name);
}

Song::~Song() {
  if (NULL != this->name) {
    delete this->name;
  }
}

AbstractParts *Song::get_parts() {
  return this->parts;
}

void Song::set_name(const string *name) {
  if (NULL != this->name) {
    delete this->name;
  }
  this->name = new string(*name);
}

string *Song::get_name() {
  return this->name;
}

bool Song::pattern_is_used(AbstractPattern *pattern) {
  AbstractParts::iterator i;
  for (i = this->parts->begin(); i != this->parts->end(); ++i) {
    AbstractPart *part = *i;
    if (true == part->pattern_is_used(pattern)) {
      return true;
    }
  }
  return false;
}

void Song::add_part(AbstractPart *part) {
  this->parts->push_back(part);
}

void Song::insert_part(AbstractPart *before,
                             AbstractPart *part) {
  AbstractParts::iterator i;
  for (i = this->parts->begin(); i != this->parts->end(); ++i) {
    if (*i == before) {
      this->parts->insert(i, part);
      return;
    }
  }
  /*
   * @todo Evaluate if this method shall throw an exception if the before-
   *       part was not in the list. For now just add the part instead.
   */
  add_part(part);
}

void Song::delete_part(AbstractPart *part) {
  this->parts->remove(part);
}
