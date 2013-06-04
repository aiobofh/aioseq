/**
 * PartTest.cpp
 *
 * Test cases for Part.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Part.h"

using namespace std;

TEST(Part, Constructor_without_arguments_shall_create_a_part_with_a_known_name) {
  /**
   * Construct a new part without any arguments.
   */
  Part part;
  /**
   * Make sure that the new part has no name (NULL).
   */
  ASSERT_EQ(NULL, part.name);
}


TEST(Part, Constructor_with_arguments_shall_create_a_part_with_the_correct_name_and_contents) {
  string foobar = "Foobar";
  /**
   * Construct a new part with the string "Foobar" as name.
   */
  Part part(&foobar);
  /**
   * Make sure that we got a part with the "Foobar" as name, by inspecting the
   * internal protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(foobar, *(part.name));
  /**
   * Make sure that the name is not a pointer to the exact same string, but
   * rather a copy of the string provided as argument to the constructor.
   */
  ASSERT_FALSE(&foobar == part.name);
}


TEST(Part, Set_name_shall_set_name) {
  string barfoo = "Barfoo";
  /**
   * Construct a new part without arguments.
   */
  Part part;
  /**
   * Set the name using the set_name() method to "Barfoo".
   */
  part.set_name(&barfoo);
  /**
   * Make sure that the part name is "Barfoo", by inspecting the internal
   * protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(barfoo, *(part.name));
}


TEST(Part, Set_name_shall_set_new_name) {
  string foobar = "Foobar";
  string barfoo = "Barfoo";
  /**
   * Construct a new part with the name "Foobar".
   */
  Part part(&foobar);
  /**
   * Set the name to "Barfoo" instead.
   */
  part.set_name(&barfoo);
  /**
   * Make sure that the new name is set to "Barfoo", by inspecting the
   * internal protected variable containing a pointer to name copy.
   */
  ASSERT_EQ(barfoo, *(part.name));
}


TEST(Part, Get_name_shall_get_name) {
  string barfoo = "Barfoo";
  /**
   * Construct a new part with the name "Barfoo".
   */
  Part part(&barfoo);
  /**
   * Get the pointer to the name by using the get_name() method and make sure
   * that it is still "Barfoo".
   */
  ASSERT_EQ(barfoo, *(part.get_name()));
}
