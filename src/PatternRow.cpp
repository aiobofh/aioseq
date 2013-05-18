/**
 * @file PatternRow.cpp
 *
 * Implementation of the pattern row object.
 */

#include "PatternRow.h"

PatternRow::PatternRow(list<AbstractChannelRow*> *channels) {
  this->channels = channels;
}
