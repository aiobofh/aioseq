/**
 * @file Song.cpp
 *
 * Song implementation.
 */

#include "AbstractPart.h"
#include "AbstractParts.h"
#include "Name.h"
#include "Song.h"

Song::Song() : Name(DEFAULT_SONG_NAME) {
  this->parts = NULL;
}

Song::Song(const string *name) : Name(name) {
  this->parts = NULL;
}

Song::Song(AbstractParts *parts) : Name(DEFAULT_SONG_NAME) {
  this->parts = parts;
}

Song::Song(const string *name, AbstractParts *parts) : Name (name) {
  this->parts = parts;
}

AbstractParts *Song::get_parts() {
  return this->parts;
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
