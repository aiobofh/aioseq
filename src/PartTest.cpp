/**
 * @file PartTest.cpp
 *
 * Test cases for Part.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Part.h"
#include "MockAbstractPatterns.h"

using namespace std;

/**
 * @test Constructor without arguments shall create a part with a known name.
 */
TEST(Part, Constructor_without_arguments_shall_create_a_part_with_a_known_name) {
  /**
   * Construct a new part without any arguments.
   */
  Part part;
  /**
   * Make sure that the new part has no name (NULL).
   */
  ASSERT_EQ(NULL, part.name);
  /**
   * Make sure that the patterns member variable is still NULL, since no
   * patterns are set with this constructor.
   */
  ASSERT_EQ(NULL, part.patterns);
}


/**
 * @test Constructor with arguments sjall create a part with the correct name.
 */
TEST(Part, Constructor_with_name_arguments_shall_create_a_part_with_the_correct_name_and_contents) {
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
  /**
   * Make sure that the patterns member variable is still NULL, since no
   * patterns are set with this constructor.
   */
  ASSERT_EQ(NULL, part.patterns);
}


/**
 * @test Constructor with patterns argument shall create a part with the
 *       correct patterns.
 */
TEST(Part, Constructor_with_patterns_arguments_shall_create_a_part_with_the_correct_patterns) {
  MockAbstractPatterns patterns;
  /**
   * Since the patterns list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Construct a new part with the string "Foobar" as name.
   */
  Part part(&patterns);
  /**
   * Make sure that the new part has no name (NULL).
   */
  ASSERT_EQ(NULL, part.name);
  /**
   * Make sure that we got a part with the pattern list set.
   */
  ASSERT_EQ(&patterns, part.patterns);
}


/**
 * @test Constructor with name and patterns arguments shall create a part with
 *       the correct name and patterns.
 */
TEST(Part, Constructor_with_name_and_patterns_arguments_shall_create_a_part_with_the_correct_name_and_patterns) {
  string foobar = "Foobar";
  MockAbstractPatterns patterns;
  /**
   * Since the patterns list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Construct a new part with the string "Foobar" as name.
   */
  Part part(&foobar, &patterns);
  /**
   * Make sure that we got a part with the "Foobar" as name, by inspecting the
   * internal protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(foobar, *(part.name));
  /**
   * Make sure that we got a part with the pattern list set.
   */
  ASSERT_EQ(&patterns, part.patterns);
}



/**
 * @test Set name shall set name.
 */
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


/**
 * @test Set name shall set new name.
 */
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


/**
 * @test Get name shall get name.
 */
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


/**
 * @test Get patterns shall get patterns.
 */
TEST(Part, Get_patterns_shall_get_patterns) {
  MockAbstractPatterns patterns;
  /**
   * Since the patterns list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Construct a new part with the string "Foobar" as name.
   */
  Part part(&patterns);
  /**
   * Make sure that we got a part with the "Foobar" as name, by inspecting the
   * internal protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(&patterns, part.get_patterns());
}
