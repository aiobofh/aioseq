/**
 * @file AbstractProject.h
 *
 * Pure virtual interface for Project classes.
 */


#ifndef _ABSTRACT_PROJECT_H_
#define _ABSTRACT_PROJECT_H_

#include <string> // Mock

#include "AbstractSong.h" // Mock
#include "AbstractSongs.h" // Mock
#include "AbstractPattern.h" // Mock
#include "AbstractPatterns.h" // Mock

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
  AbstractProject() {}; // Mock


  /**
   * Constructor taking a list of songs and a list of partterns as arguments.
   *
   * @param songs @copydoc AbstractProject::songs
   * @param patterns @copydoc AbstractProject::patterns
   */
  AbstractProject(AbstractSongs *songs, AbstractPatterns *patterns) {}; // Mock


  /**
   * Constructor taking a name, a list of songs and a list of patterns as
   * arguments.
   *
   * @param name A pointer to the name of the project.
   * @param songs @copydoc AbstractProject::songs
   * @param patterns @copydoc AbstractProject::patterns
   */
  AbstractProject(const string *name, AbstractSongs *songs, AbstractPatterns *patterns) {}; // Mock


  /**
   * Get the list of patterns within a project.
   *
   * @return @copydoc AbstractProject::patterns
   */
  virtual AbstractPatterns *get_patterns() = 0;


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
   * Insert a pattern in the project.
   *
   * @param before A pointer to a pattern in the project to insert the new
   *               pattern before.
   * @param pattern A pointer to a pattern object to insert in the project.
   */
  virtual void insert_pattern(AbstractPattern *before,
                              AbstractPattern *pattern) = 0;


  /**
   * Delete an existing pattern from the project.
   *
   * @param pattern A pointer to the pattern object to delete.
   */
  virtual void delete_pattern(AbstractPattern *pattern) = 0;


  /**
   * Get the list of songs within a project.
   *
   * @return @copydoc AbstractProject::songs
   */
  virtual AbstractSongs *get_songs() = 0;


  /**
   * Add a song to the project.
   *
   * @param song A pointer to the song object to add.
   */
  virtual void add_song(AbstractSong *song) = 0;


  /**
   * Insert a song in the project.
   *
   * @param before A pointer to a song in the project to insert the new
   *               song before.
   * @param song A pointer to a song object to insert in the project.
   */
  virtual void insert_song(AbstractSong *before,
                           AbstractSong *song) = 0;


  /**
   * Delete an existing song from the project.
   *
   * @param song A pointer to the song object to delete.
   */
  virtual void delete_song(AbstractSong *song) = 0;

};

#endif
