/**
 * @file AbstractSong.h
 *
 * Interface for song objecs.
 */

#ifndef _ABSTRACT_SONG_H_
#define _ABSTRACT_SONG_H_

#include <string>

#include "AbstractParts.h" // Include in mock

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
   * @param name @copydoc AbstractSong::name
   */
  AbstractSong(const string *name) {}; // Mock


  /**
   * Constructor
   *
   * @param parts @copydoc AbstractSong::parts
   */
  AbstractSong(AbstractParts *parts) {}; // Mock


  /**
   * Constructor
   *
   * @param name @copydoc AbstractSong::name
   * @param parts @copydoc AbstractSong::parts
   */
  AbstractSong(const string *name, AbstractParts *parts) {}; // Mock


  /**
   * Get a list of song parts from the song.
   *
   * @return @copydoc AbstractSong::parts
   */
  virtual AbstractParts *get_parts() = 0;


  /**
   * Set the name of the song.
   *
   * @param name A pointer to a string containing the new name of the song.
   */
  virtual void set_name(const string *name) = 0;


  /**
   * Get the name of the song.
   *
   * @return @copydoc AbstractSong::name
   */
  virtual string *get_name() = 0;

};

#endif
