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

};

#endif
