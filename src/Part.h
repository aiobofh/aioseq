/**
 * @file Part.h
 *
 * @copydoc AbstractPart.h
 */

#ifndef _PART_H_
#define _PART_H_

#include <string>

#include <gtest/gtest_prod.h>

#include "AbstractPart.h"

using namespace std;


/**
 * @copydoc AbstractPart
 */
class Part : protected AbstractPart {

protected:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_without_arguments_shall_create_a_part_with_a_known_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_with_name_arguments_shall_create_a_part_with_the_correct_name_and_contents);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_with_patterns_arguments_shall_create_a_part_with_the_correct_patterns);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_with_name_and_patterns_arguments_shall_create_a_part_with_the_correct_name_and_patterns);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Set_name_shall_set_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Set_name_shall_set_new_name);

public:

  /**
   * @copydoc AbstractPart::AbstractPart()
   */
  Part();


  /**
   * @copydoc AbstractPart::AbstractPart(const string *)
   */
  Part(const string *name);


  /**
   * @copydoc AbstractPart::AbstractPart(const AbstractPatterns *)
   */
  Part(const AbstractPatterns *patterns);


  /**
   * @copydoc AbstractPart::AbstractPart(const string *, const AbstractPatterns *)
   */
  Part(const string *name, const AbstractPatterns *patterns);


  /**
   * @copydoc AbstractPart::set_name(const string *)
   */
  void set_name(const string *name);


  /**
   * @copydoc AbstractPart::get_name()
   */
  string *get_name();

};

#endif
