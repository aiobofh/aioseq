/**
 * @file Name.h
 *
 * Declaration of a simple implementation of a name.
 */

#ifndef _NAME_H_
#define _NAME_H_

#include <string>

#include "AbstractName.h"

/**
 * A simple name
 */
class Name : protected AbstractName {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Name, Constructor_with_arguments_shall_store_a_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Name, Name_setter_with_a_const_char_shall_set_name_and_remove_old_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Name, Name_setter_with_a_const_string_shall_set_name_and_remove_old_name);

public:

  /**
   * Constructor taking the name as argument.
   *
   * @param name @copydoc AbstractName::name
   */
  Name(const char *name);


  /**
   * Constructor taking the name as argument.
   *
   * @param name @copydoc AbstractName::name
   */
  Name(const string *name);


  /**
   * Destrucor.
   */
  ~Name();


  /**
   * @copydoc AbstractName::set_name
   */
  void set_name(const char *name);


  /**
   * @copydoc AbstractName::set_name
   */
  void set_name(const string *name);


  /**
   * @copydoc AbstractName::get_name
   */
  string *get_name();

};

#endif /* _NAME_H_ */
