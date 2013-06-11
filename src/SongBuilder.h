/**
 * @file SongBuilder.h
 *
 * Interface for a Song factory. Unfortunatly this file also contains the
 * class implementation, since it is a template class.
 */

#ifndef _SONG_BUILDER_H_
#define _SONG_BUILDER_H_

/**
 * AbstractParts is used as a pointer type containing a list of parts in
 * the song.
 */
#include "AbstractParts.h"

/**
 * AbstractSong is used as a pointer returned when creating and for destroying
 * song objects.
 */
#include "AbstractSong.h"

/**
 * SongBuilder inherits from this class interface.
 */
#include "AbstractSongBuilder.h"

#include <cstddef>

template<class SONG_CLASS>

/**
 * @copydoc AbstractSongBuilder
 */
class SongBuilder : AbstractSongBuilder {

public:

  /**
   * @copydoc AbstractSongBuilder::create(string *)
   */
  AbstractSong *create(string *name) {
    AbstractParts *parts = new AbstractParts();
    return new SONG_CLASS(name, parts);
  }


  /**
   * @copydoc AbstractSongBuilder::destroy(AbstractSong **)
   */
  void destroy(AbstractSong **song) {
    AbstractParts *parts = (*song)->get_parts();
    delete parts;
    delete *song;
    *song = NULL;
  }

};

#endif
