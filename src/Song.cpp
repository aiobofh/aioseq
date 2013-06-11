/**
 * @file Song.cpp
 *
 * Song implementation.
 */

#include "Song.h"
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
