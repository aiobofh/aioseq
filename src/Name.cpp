/**
 * Name.cpp
 *
 * Implementation of a simple name.
 */

#include <string>

#include "Name.h"

Name::Name(const char *name) {
  this->name = new string(name);
}

Name::Name(const string *name) {
  this->name = new string((const char*)name);
}

Name::~Name() {
  delete name;
}

void Name::set_name(const char *name) {
  delete this->name;
  this->name = new string(name);
}

void Name::set_name(const string *name) {
  delete this->name;
  this->name = new string((const char*)name);
}

string *Name::get_name() {
  return this->name;
}
