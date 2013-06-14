/**
 * @file Project.cpp
 *
 * Project implementation.
 */

#include <string>

#include "Project.h"

#include "AbstractPattern.h"
#include "AbstractPatterns.h"
#include "AbstractSong.h"
#include "AbstractSongs.h"

/**
 * For now this is the default number of channels in an AiOSeq project.
 */
#define DEFAULT_CHANNELS 8
/**
 * For now this is the default pattern length for new patterns in an AiOSeq
 * project.
 */
#define DEFAULT_PATTERN_LENGTH 64

Project::Project() {
  name = NULL;
  channels = DEFAULT_CHANNELS;
  default_pattern_length = DEFAULT_PATTERN_LENGTH;
  songs = NULL;
  patterns = NULL;
}

Project::Project(AbstractSongs *songs, AbstractPatterns *patterns) {
  name = NULL;
  channels = DEFAULT_CHANNELS;
  default_pattern_length = DEFAULT_PATTERN_LENGTH;
  this->songs = songs;
  this->patterns = patterns;
}

Project::Project(const string *name,
                 AbstractSongs *songs,
                 AbstractPatterns *patterns) {
  this->name = NULL;
  channels = DEFAULT_CHANNELS;
  default_pattern_length = DEFAULT_PATTERN_LENGTH;
  this->songs = songs;
  this->patterns = patterns;
  set_name(name);
}

Project::~Project() {
  if (NULL != this->name) {
    delete this->name;
  }
}

void Project::set_name(const string *name) {
  if (NULL != this->name) {
    delete this->name;
  }
  this->name = new string(*name);
}

string* Project::get_name() {
  return name;
}

AbstractPatterns *Project::get_patterns() {
  return patterns;
}

void Project::add_pattern(AbstractPattern *pattern) {
  this->patterns->push_back(pattern);
}

bool Project::pattern_is_used(AbstractPattern *pattern) {
  AbstractSongs::iterator song;
  for (song = this->songs->begin(); song != this->songs->end(); ++song) {
    AbstractSong *s = *song;
    if (true == s->pattern_is_used(pattern)) {
      return true;
    }
  }
  return false;
}

void Project::insert_pattern(AbstractPattern *before,
                             AbstractPattern *pattern) {
  AbstractPatterns::iterator i;
  for (i = this->patterns->begin(); i != this->patterns->end(); ++i) {
    if (*i == before) {
      this->patterns->insert(i, pattern);
      return;
    }
  }
  /*
   * @todo Evaluate if this method shall throw an exception if the before-
   *       pattern was not in the list. For now just add the pattern instead.
   */
  add_pattern(pattern);
}

void Project::delete_pattern(AbstractPattern *pattern) {
  this->patterns->remove(pattern);
}

AbstractSongs *Project::get_songs() {
  return songs;
}

void Project::add_song(AbstractSong *song) {
  this->songs->push_back(song);
}

void Project::insert_song(AbstractSong *before, AbstractSong *song) {
  AbstractSongs::iterator i;
  for (i = this->songs->begin(); i != this->songs->end(); ++i) {
    if (*i == before) {
      this->songs->insert(i, song);
      return;
    }
  }
  /*
   * @todo Evaluate if this method shall throw an exception if the before-
   *       pattern was not in the list. For now just add the pattern instead.
   */
  add_song(song);
}

void Project::delete_song(AbstractSong *song) {
  this->songs->remove(song);
}
