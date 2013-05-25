/**
 * @file AbstractProject.h
 *
 * Pure virtual interface for Project classes.
 */


#ifndef _ABSTRACT_PROJECT_H_
#define _ABSTRACT_PROJECT_H_

#include "AbstractPattern.h"

/**
 * Represent a project in a pure virtual fashion.
 */
class AbstractProject {
public:
  /**
   * Dummy constructor.
   */
  AbstractProject() {};

  /**
   * Add a new, empty pattern to the project.
   *
   * @return A pointer to the new pattern object.
   */
  virtual AbstractPattern *add_pattern() = 0;
};

#endif
