/**
 * @file AbstractSongs.h
 *
 * List of abstract songs.
 */

#ifndef _ABSTRACT_SONGS_H_
#define _ABSTRACT_SONGS_H_

#include "AbstractSong.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointers to notes.
 */
class AbstractSongs : public list<AbstractSong*> {
public:
  AbstractSongs() : list<AbstractSong*>() {};
};

#endif
