/**
 * @file PartTest.cpp
 *
 * Test cases for Part.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Part.h"
#include "AbstractPatterns.h"
#include "MockAbstractPattern.h"
#include "MockAbstractPatterns.h"

using namespace std;

/**
 * @test Constructor without arguments shall create a part with no name and no
 *       patterns.
 */
TEST(Part, Constructor_without_arguments_shall_create_a_part_with_no_name_and_no_patterns) {
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
 * @test Constructor with name argument shall create a part with a name but no
 *       patterns.
 */
TEST(Part, Constructor_with_name_argument_shall_create_a_part_with_a_name_but_no_patterns) {
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
 *       correct patterns but no name.
 */
TEST(Part, Constructor_with_patterns_arguments_shall_create_a_part_with_the_correct_patterns_but_no_name) {
  MockAbstractPatterns patterns;
  /**
   * Since the patterns list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Construct a new part with the pattern list as argument.
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
   * Make sure that the get_patters method returns the correct pointer.
   */
  ASSERT_EQ(&patterns, part.get_patterns());
}


/**
 * @test Add pattern shall add a pattern to the part.
 */
TEST(Part, Add_pattern_shall_add_a_pattern_to_the_part) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die()).Times(1);

  /**
   * Call the design under test.
   */
  part.add_pattern(&pattern);

  /**
   * Get the pattern list from the song and validate that it contains one
   * pattern and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = part.get_patterns();

  /**
   * Make sure that there is one pattern in the list.
   */
  ASSERT_EQ(1, patterns_got->size());

  /**
   * Make sure that the one item is exactly the same pattern that that was
   * provided.
   */
  ASSERT_EQ(&pattern, patterns_got->front());
}


/**
 * @test Adding several patterns shall add several patterns to the part.
 */
TEST(Part, Adding_several_patterns_shall_add_several_patterns_to_the_part) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern1;
  MockAbstractPattern pattern2;
  MockAbstractPattern pattern3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern1, Die()).Times(1);
  EXPECT_CALL(pattern2, Die()).Times(1);
  EXPECT_CALL(pattern3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  part.add_pattern(&pattern1);
  part.add_pattern(&pattern2);
  part.add_pattern(&pattern3);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = part.get_patterns();

  /**
   * Make sure that there is three patterns in the list.
   */
  ASSERT_EQ(3, patterns_got->size());

  /**
   * Make sure that the one item is exactly the same pattern that that was
   * provided.
   */
  ASSERT_EQ(&pattern1, patterns_got->front());
  ASSERT_EQ(&pattern3, patterns_got->back());
}


/**
 * @test Inserting a pattern shall insert a pattern in the part.
 */
TEST(Part, Inserting_a_pattern_shall_insert_a_pattern_in_the_part) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern1;
  MockAbstractPattern pattern2;
  MockAbstractPattern pattern3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern1, Die()).Times(1);
  EXPECT_CALL(pattern2, Die()).Times(1);
  EXPECT_CALL(pattern3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  part.add_pattern(&pattern1);
  part.add_pattern(&pattern3);
  part.insert_pattern(&pattern1, &pattern2);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = part.get_patterns();

  /**
   * Make sure that there is three patterns in the list.
   */
  ASSERT_EQ(3, patterns_got->size());

  /**
   * Make sure that the one item is exactly the same pattern that that was
   * provided.
   */
  ASSERT_EQ(&pattern2, patterns_got->front());
  ASSERT_EQ(&pattern3, patterns_got->back());
}


/**
 * @test Inserting a pattern before an unexsisting pattern shall add the
 *       pattern to the end of the part.
 */
TEST(Part, Inserting_a_pattern_before_an_unexisting_pattern_shall_add_the_pattern_to_the_end_of_the_part) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern1;
  MockAbstractPattern pattern2;
  MockAbstractPattern pattern3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern1, Die()).Times(1);
  EXPECT_CALL(pattern2, Die()).Times(1);
  EXPECT_CALL(pattern3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  part.add_pattern(&pattern1);
  part.insert_pattern(&pattern3, &pattern2);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = part.get_patterns();

  /**
   * Make sure that there is three patterns in the list.
   */
  ASSERT_EQ(2, patterns_got->size());

  /**
   * Make sure that the one item is exactly the same pattern that that was
   * provided.
   */
  ASSERT_EQ(&pattern1, patterns_got->front());
  ASSERT_EQ(&pattern2, patterns_got->back());
}


/**
 * @test Deleting a pattern shall delete a pattern from the part.
 */
TEST(Part, Deleting_a_pattern_shall_delete_a_pattern_from_the_part) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern1;
  MockAbstractPattern pattern2;
  MockAbstractPattern pattern3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern1, Die()).Times(1);
  EXPECT_CALL(pattern2, Die()).Times(1);
  EXPECT_CALL(pattern3, Die()).Times(1);

  /**
   * Add some patterns
   */
  part.add_pattern(&pattern1);
  part.add_pattern(&pattern2);
  part.add_pattern(&pattern3);

  /**
   * Calle the design under test.
   */
  part.delete_pattern(&pattern1);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = part.get_patterns();

  /**
   * Make sure that there is three patterns in the list.
   */
  ASSERT_EQ(2, patterns_got->size());

  /**
   * Make sure that the one item is exactly the same pattern that that was
   * provided.
   */
  ASSERT_EQ(&pattern2, patterns_got->front());
  ASSERT_EQ(&pattern3, patterns_got->back());
}


/**
 * @test Pattern is used-method shall return true if pattern is used.
 */
TEST(Part, Pattern_is_used_method_shall_return_true_if_pattern_is_used) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die()).Times(1);

  /**
   * Add some patterns
   */
  part.add_pattern(&pattern);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(true == part.pattern_is_used(&pattern));
}


/**
 * @test Pattern is used-method shall return false if pattern is not used.
 */
TEST(Part, Pattern_is_used_method_shall_return_false_if_pattern_is_not_used) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new part with the pattern list.
   */
  Part part(&patterns);

  /**
   * Create a pattern mock-up to add.
   */
  MockAbstractPattern pattern1;
  /**
   * Create another pattern mock-up to search for.
   */
  MockAbstractPattern pattern2;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern1, Die()).Times(1);
  EXPECT_CALL(pattern2, Die()).Times(1);

  /**
   * Add some patterns
   */
  part.add_pattern(&pattern1);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(false == part.pattern_is_used(&pattern2));
}
