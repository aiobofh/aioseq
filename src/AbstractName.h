/**
 * @file AbstractName.h
 *
 * Pure virtual class interface for representing a name.
 */

#ifndef _ABSTRACT_NAME_H_
#define _ABSTRACT_NAME_H_

#include <gtest/gtest_prod.h>

#include <string>

using std::string;

/**
 * Represent a name of something.
 */
class AbstractName {

public:

  /**
   * Dummy constructor.
   */
  AbstractName() {}; // Mock


  /**
   * Constructor taking the name as argument.
   *
   * @param name Name to set.
   */
  AbstractName(const char *name) {}; // Mock


  /**
   * Constructor taking the name as argument.
   *
   * @param name Name to set.
   */
  AbstractName(const string *name) {}; // Mock


  /**
   * Set the name.
   *
   * @param name Name to set.
   */
  virtual void set_name(const char *name) = 0;

  /**
   * Set the name.
   *
   * @param name Name to set.
   */
  virtual void set_name(const string *name) = 0;


  /**
   * Get the name.
   *
   * @return A pointer to a C++ string containing the name.
   */
  virtual string *get_name() = 0;

};

#endif /* _ABSTRACT_NAME_H_ */
