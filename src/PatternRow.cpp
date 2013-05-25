/**
 * @file PatternRow.cpp
 *
 * Implementation of the pattern row object.
 */

#include "PatternRow.h"

PatternRow::PatternRow(list<AbstractChannelRow*> *channels) {
  this->channel_rows = channel_rows;
}

list<AbstractChannelRow*> *PatternRow::get_channel_rows() {
  return this->channel_rows;
}
