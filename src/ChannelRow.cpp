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
