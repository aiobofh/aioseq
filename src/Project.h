/**
 * @file Project.h
 *
 * The interface declaration for the Project class.
 */

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <list>

#include "AbstractPattern.h"
#include "AbstractProject.h"

using std::list;

/**
 * Represent a project.
 */
class Project : public AbstractProject {
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
   * List of pointers to pattern objects within the project.
   */
  list<AbstractPattern*> patterns;

public:

  /**
   * Constructor will create a pattern.
   */
  Project();


  /**
   * Add a new pattern to the project.
   *
   * @return A pointer to the new pattern.
   */
  AbstractPattern *add_pattern();


  /**
   * @copydoc AbstractProject::add_pattern(AbstractPattern *)
   */
  void add_pattern(AbstractPattern *pattern);

  /**
   * @copydoc AbstractProject::pattern_is_used(AbstractPattern *pattern)
   */
  bool pattern_is_used(AbstractPattern *pattern);

  /**
   * @copydoc AbstractProject::delete_pattern(AbstractPattern *pattern)
   */
  void delete_pattern(AbstractPattern *pattern);

  /**
   * @copydoc AbstractProject::add_song(AbstractSong *song)
   */
  void add_song(AbstractSong *song);

  /**
   * @copydoc AbstractProject::delete_song(AbstractSong *song)
   */
  void delete_song(AbstractSong *song);

};

#endif
