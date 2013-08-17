/**
 * @file AbstractSong.h
 *
 * Interface for song objecs.
 */

#ifndef _ABSTRACT_SONG_H_
#define _ABSTRACT_SONG_H_

#include <string>

#include "AbstractPart.h" // Include in mock
#include "AbstractParts.h" // Include in mock

/**
 * Represent a song.
 */
class AbstractSong {

public:

  /**
   * Dummy constructor.
   */
  AbstractSong() {}; // Mock


  /**
   * Constructor
   *
   * @param name A pointer to the name of the song.
   */
  AbstractSong(const string *name) {}; // Mock


  /**
   * Constructor
   *
   * @param parts A pointer to a list of song parts that the song consists of.
   */
  AbstractSong(AbstractParts *parts) {}; // Mock


  /**
   * Constructor
   *
   * @param name A pointer to the name of the song.
   * @param parts A pointer to a list of song parts that the song consists of.
   */
  AbstractSong(const string *name, AbstractParts *parts) {}; // Mock


  /**
   * Get a list of song parts from the song.
   *
   * @return A pointer to a list of song parts that the song consists of.
   */
  virtual AbstractParts *get_parts() = 0;


  /**
   * Determine if a pattern is used in a part.
   * @param pattern A pointer to the patterh to search for.
   *
   * @return Boolean true if it is used, false if it is not used.
   */
  virtual bool pattern_is_used(AbstractPattern *pattern) = 0;


  /**
   * Add a part to the song.
   *
   * @param part A pointer to the part object to add.
   */
  virtual void add_part(AbstractPart *part) = 0;


  /**
   * Insert a part in the song.
   *
   * @param before A pointer to a part in the song to insert the new
   *               part before.
   * @param part A pointer to a part object to insert in the song.
   */
  virtual void insert_part(AbstractPart *before, AbstractPart *part) = 0;


  /**
   * Delete an existing part from the song.
   *
   * @param part A pointer to the part object to delete.
   */
  virtual void delete_part(AbstractPart *part) = 0;

};

#endif
