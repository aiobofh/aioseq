/**
 * @file track_entry.cc
 */
#include "track_entry.hh"
#include "note.hh"
#include "effect.hh"
#include "track.hh"

TrackEntry::TrackEntry(NotesInterface* notes, EffectsInterface *effects, TrackInterface* track) :
  TrackEntryTemplate<Notes, Effects, Track>(notes, effects, track) {}

TrackEntry::~TrackEntry() {}
