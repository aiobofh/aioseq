/**
 * @file pattern_row.cc
 */
#include "pattern_row.hh"

#include "track_entry.hh"

PatternRow::PatternRow(TrackEntriesInterface *track_entries) :
  PatternRowTemplate<TrackEntries>(track_entries) {}

PatternRow::~PatternRow() {}
