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
  /*
  FRIEND_TEST(Name, Constructor_with_arguments_shall_store_a_name);
  FRIEND_TEST(Name, Name_setter_shall_set_name_and_remove_old_name);
  */
protected:

  /**
   * A pointer to a string contining the name.
   */
  string *name;

public:

  /**
   * Dummy constructor.
   */
  AbstractName() {}; // Mock


  /**
   * Constructor taking the name as argument.
   *
   * @param name @copydoc AbstractName::name
   */
  AbstractName(const char *name) {}; // Mock


  /**
   * Constructor taking the name as argument.
   *
   * @param name @copydoc AbstractName::name
   */
  AbstractName(const string *name) {}; // Mock


  /**
   * Set the name.
   *
   * @param name @copydoc AbstractName::name
   */
  virtual void set_name(const char *name) = 0;

  /**
   * Set the name.
   *
   * @param name @copydoc AbstractName::name
   */
  virtual void set_name(const string *name) = 0;


  /**
   * Get the name.
   *
   * @return @copydoc AbstractName::name
   */
  virtual string *get_name() = 0;

};

#endif /* _ABSTRACT_NAME_H_ */
