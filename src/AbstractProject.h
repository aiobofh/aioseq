/**
 * @file AbstractProject.h
 *
 * Pure virtual interface for Project classes.
 */


#ifndef _ABSTRACT_PROJECT_H_
#define _ABSTRACT_PROJECT_H_

#include "AbstractSong.h"
#include "AbstractSongs.h"
#include "AbstractPattern.h"
#include "AbstractPatterns.h"

/**
 * Represent a project in a pure virtual fashion.
 */
class AbstractProject {

protected:

  /**
   * A pointer to the list of pointers to songs within a project.
   */
  AbstractSongs *songs;

  /**
   * A pointer to the list of pointers to patterns within a project.
   */
  AbstractPatterns *patterns;

public:

  /**
   * Dummy constructor.
   */
  AbstractProject() {};


  /**
   * Add a pattern to the project.
   *
   * @param pattern A pointer to the pattern object to add.
   */
  virtual void add_pattern(AbstractPattern *pattern) = 0;


  /**
   * Determine if a pattern is used anywhere in the project or not.
   *
   * @param pattern A pointer to the patterh to search for.
   *
   * @return Boolean true if it is used, false if it is not used.
   */
  virtual bool pattern_is_used(AbstractPattern *pattern) = 0;


  /**
   * Delete an existing pattern from the project.
   *
   * @param pattern A pointer to the pattern object to delete.
   */
  virtual void delete_pattern(AbstractPattern *pattern) = 0;


  /**
   * Add a song to the project.
   *
   * @param song A pointer to the song object to add.
   */
  virtual void add_song(AbstractSong *song) = 0;


  /**
   * Delete an existing song from the project.
   *
   * @param song A pointer to the song object to delete.
   */
  virtual void delete_song(AbstractSong *song) = 0;

};

#endif
