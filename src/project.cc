/**
 * @file project.cc
 */
#include "project.hh"
#include "track.hh"
#include "pattern.hh"
#include "song.hh"

Project::Project(string name, TracksInterface* tracks, PatternsInterface* patterns, SongsInterface* songs) : ProjectTemplate(name, tracks, patterns, songs) {}

Project::~Project() {}
