/**
 * @file PatternRow.cpp
 *
 * Implementation of the pattern row object.
 */

#include "PatternRow.h"

PatternRow::PatternRow(AbstractChannelRows *channel_rows) {
  this->channel_rows = channel_rows;
}

AbstractChannelRows *PatternRow::get_channel_rows() {
  return this->channel_rows;
}
