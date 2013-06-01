/**
 * @file ChannelRow.cpp
 *
 * Implementation if ChannrlRow.
 */

#include "AbstractNotes.h"
#include "AbstractParameters.h"
#include "ChannelRow.h"

using std::list;

ChannelRow::ChannelRow(AbstractNotes *notes, AbstractParameters *parameters) {
  this->notes = notes;
  this->parameters = parameters;
}

AbstractNotes *ChannelRow::get_notes() {
  return this->notes;
}

AbstractParameters *ChannelRow::get_parameters() {
  return this->parameters;
}
