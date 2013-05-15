/**
 * @file Parameter.cpp
 *
 * Parameter implementation.
 *
 * A Parameter is a simple entity that can contain a parameter type and a
 * value to the parameter. These are often used for effects on a Note that
 * is playing.
 */

#include "Parameter.h"

Parameter::Parameter() {
  this->type = this->value = 0;
}

Parameter::Parameter(int type, int value) {
  this->type = type;
  this->value = value;
}

void Parameter::set_type(int type) {
  this->type = type;
}

int Parameter::get_type() {
  return this->type;
}

void Parameter::set_value(int value) {
  this->value = value;
}

int Parameter::get_value() {
  return this->value;
}
