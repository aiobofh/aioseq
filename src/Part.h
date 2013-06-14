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
#include "AbstractPatterns.h"

using namespace std;


/**
 * @copydoc AbstractPart
 */
class Part : public AbstractPart {

protected:

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_without_arguments_shall_create_a_part_with_no_name_and_no_patterns);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_with_name_argument_shall_create_a_part_with_a_name_but_no_patterns);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Part, Constructor_with_patterns_arguments_shall_create_a_part_with_the_correct_patterns_but_no_name);

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
   * @copydoc AbstractPart::AbstractPart(AbstractPatterns *)
   */
  Part(AbstractPatterns *patterns);


  /**
   * @copydoc AbstractPart::AbstractPart(const string *, AbstractPatterns *)
   */
  Part(const string *name, AbstractPatterns *patterns);


  /**
   * Destructor is cleaning up the object (the name string).
   */
  ~Part();


  /**
   * @copydoc AbstractPart::set_name(const string *)
   */
  void set_name(const string *name);


  /**
   * @copydoc AbstractPart::get_name()
   */
  string *get_name();


  /**
   * @copydoc AbstractPart::get_patterns()
   */
  AbstractPatterns *get_patterns();


  /**
   * @copydoc AbstractPart::add_pattern(AbstractPattern *)
   */
  void add_pattern(AbstractPattern *pattern);


  /**
   * @copydoc AbstractPart::pattern_is_used(AbstractPattern *)
   */
  bool pattern_is_used(AbstractPattern *pattern);


  /**
   * @copydoc AbstractPart::insert_pattern(AbstractPattern *, AbstractPattern *)
   */
  void insert_pattern(AbstractPattern *before, AbstractPattern *pattern);


  /**
   * @copydoc AbstractPart::delete_pattern(AbstractPattern *)
   */
  void delete_pattern(AbstractPattern *pattern);

};

#endif
