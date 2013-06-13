/**
 * @file Project.cpp
 *
 * Project implementation.
 */

#include "Project.h"

#include "AbstractPattern.h"
#include "PatternBuilder.h"

Project::Project() {
  channels = 0;
  default_pattern_length = 16;
}

void Project::add_pattern(AbstractPattern *pattern) {
}

bool Project::pattern_is_used(AbstractPattern *pattern) {
}

void Project::delete_pattern(AbstractPattern *pattern) {
}

void Project::add_song(AbstractSong *song) {
}

void Project::delete_song(AbstractSong *song) {
}
