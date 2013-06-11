/**
 * @file AbstractSongBuilder.h
 *
 * Interface for a song factory.
 */

#ifndef _ABSTRACT_SONG_BUILDER_H_
#define _ABSTRACT_SONG_BUILDER_H_

#include "AbstractSong.h"

// Mock: template<class SONG_CLASS>
/**
 * Song object factory class.
 */
class AbstractSongBuilder {

public:

  /**
   * Dummy constructor.
   */
  AbstractSongBuilder() {};


  /**
   * Create a new song object.
   *
   * @param name Name of the song to create.
   *
   * @return A pointer to the new object.
   */
  virtual AbstractSong *create(string *name) = 0;


  /**
   * Destroy a song object.
   *
   * @param song Pointer to a pointer referencing the song object to destroy.
   */
  virtual void destroy(AbstractSong **song) = 0;

};

#endif
