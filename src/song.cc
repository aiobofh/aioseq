/**
 * @file song.cc
 */
#include "part.hh"
#include "song.hh"

Song::Song(string name, PartsInterface* parts) : SongTemplate(name, parts) {}

Song::~Song() {}
