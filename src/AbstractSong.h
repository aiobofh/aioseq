/**
 * AbstractSong.h
 *
 * Interface for song objecs.
 */

#ifndef _ABSTRACT_SONG_H_
#define _ABSTRACT_SONG_H_

#include <string>

#include "AbstractSongParts.h"

/**
 * Represent a song.
 */
class AbstractSong {

protected:

  /**
   * A pointer to a string containing the name of the song.
   */
  string *name;

  /**
   * A pointer to a list of song parts that the song consists of.
   */
  AbstractParts *parts;

public:

  /**
   * Dummy constructor.
   */
  AbstractSong() {}; // Mock

  /**
   * Constructor
   *
   * @param parts @copydoc AbstractSong::parts
   */
  AbstractSong(AbstractSongParts *parts) {};

  /**
   * Get a list of song parts from the song.
   *
   * @return @copydoc AbstractSong::parts
   */
  virtual AbstractSongParts *get_parts() = 0;

  /**
   * Set the name of the song.
   *
   * @param name A pointer to a string containing the new name of the song.
   */
  virtual void set_name(string *name) = 0;

  /**
   * Get the name of the song.
   *
   * @return @copydoc AbstractSong::name
   */
  virtual string *get_name() = 0;

};

#endif
