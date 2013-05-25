/**
 * @file ChannelRow.cpp
 *
 * Implementation if ChannrlRow.
 */

#include "AbstractNote.h"
#include "ChannelRow.h"

using std::list;

ChannelRow::ChannelRow(list<AbstractNote*> *notes, list<AbstractParameter*> *parameters) {
  this->notes = notes;
  this->parameters = parameters;
}

list<AbstractNote*> *ChannelRow::get_notes() {
  return this->notes;
}

list<AbstractParameter*> *ChannelRow::get_parameters() {
  return this->parameters;
}
