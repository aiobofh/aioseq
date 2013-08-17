/**
 * @file Project.h
 *
 * The interface declaration for the Project class.
 */

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <string>

#include <gtest/gtest_prod.h>

#include "AbstractPattern.h"
#include "AbstractProject.h"

#include "Name.h"

/**
 * The default project name used if nothing is provided.
 */
#define DEFAULT_PROJECT_NAME "New project"

using std::string;

/**
 * Represent a project.
 */
class Project : public AbstractProject, public Name {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Project, Constructor_with_songs_and_patterns_as_arguments_shall_create_a_project);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Project, Constructor_with_name_songs_and_patterns_as_arguments_shall_create_a_project);

private:

  /**
   * Number of channels in the project.
   */
  int channels;


  /**
   * The number of rows to allocate for new patterns.
   */
  int default_pattern_length;


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
   * @copydoc AbstractProject::AbstractProject()
   */
  Project();


  /**
   * @copydoc AbstractProject::AbstractProject(AbstractSongs *, AbstractPatterns *)
   */
  Project(AbstractSongs *songs, AbstractPatterns *patterns);


  /**
   * @copydoc AbstractProject::AbstractProject(const string *, AbstractSongs *, AbstractPatterns *)
   */
  Project(const string *name,
          AbstractSongs *songs,
          AbstractPatterns *patterns);


  /**
   * @copydoc AbstractProject::get_patterns()
   */
  AbstractPatterns *get_patterns();


  /**
   * @copydoc AbstractProject::add_pattern(AbstractPattern *)
   */
  void add_pattern(AbstractPattern *pattern);


  /**
   * @copydoc AbstractProject::pattern_is_used(AbstractPattern *pattern)
   */
  bool pattern_is_used(AbstractPattern *pattern);


  /**
   * @copydoc AbstractProject::insert_pattern(AbstractPattern *before, AbstractPattern *pattern)
   */
  void insert_pattern(AbstractPattern *before, AbstractPattern *pattern);


  /**
   * @copydoc AbstractProject::delete_pattern(AbstractPattern *pattern)
   */
  void delete_pattern(AbstractPattern *pattern);


  /**
   * @copydoc AbstractProject::get_songs()
   */
  AbstractSongs *get_songs();

  /**
   * @copydoc AbstractProject::add_song(AbstractSong *song)
   */
  void add_song(AbstractSong *song);


  /**
   * @copydoc AbstractProject::insert_song(AbstractSong *before, AbstractSong *song)
   */
  void insert_song(AbstractSong *before, AbstractSong *song);


  /**
   * @copydoc AbstractProject::delete_song(AbstractSong *song)
   */
  void delete_song(AbstractSong *song);

};

#endif
