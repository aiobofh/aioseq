/**
 * @file SongTest.cpp
 *
 * Test cases for Song.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Song.h"
#include "AbstractPattern.h"
#include "MockAbstractPart.h"
#include "MockAbstractParts.h"
#include "MockAbstractPattern.h"

using namespace std;

using ::testing::_;
using ::testing::Invoke;

/**
 * Stub function to replace the pattern_is_used method in the MockAbstractPart
 * class.
 *
 * @param pattern A pointer to the pattern to search for.
 *
 * @return true
 */
bool return_true(AbstractPattern *pattern) {
  return true;
}

/**
 * Stub function to replace the pattern_is_used method in the MockAbstractPart
 * class.
 *
 * @param pattern A pointer to the pattern to search for.
 *
 * @return false
 */
bool return_false(AbstractPattern *pattern) {
  return false;
}


/**
 * @test Constructor without arguments shall create a song with a default_name
 *       and no parts.
 */
TEST(Song, Constructor_without_arguments_shall_create_a_song_with_a_default_name_and_no_parts) {
  /**
   * Construct a new song without any arguments.
   */
  Song song;
  /**
   * Make sure that the new song has a default name.
   */
  ASSERT_EQ((string)DEFAULT_SONG_NAME, *(song.get_name()));
  /**
   * Make sure that the parts member variable is still NULL, since no parts are
   * set with the constructor.
   */
  ASSERT_EQ(NULL, song.parts);
}


/**
 * @test Constructor with name argument shall create a song with a name but no
 *       parts.
 */
TEST(Song, Constructor_with_name_argument_shall_create_a_song_with_a_name_but_no_parts) {
  /**
   * Construct a new song with the string "Foobar" as name.
   */
  Song song((const string*)"Foobar");
  /**
   * Make sure that we got a song with the name "Foobar".
   */
  ASSERT_EQ((string)"Foobar", *(song.get_name()));
  /**
   * Make sure that the parts member variable is still NULL, since no parts are
   * set with the this constructor.
   */
  ASSERT_EQ(NULL, song.parts);
}


/**
 * @test Constructor with parts agurment shall create a song with correct
 *       parts but no name.
 */
TEST(Song, Constructor_with_parts_argument_shall_create_a_song_with_correct_parts_but_no_name) {
  MockAbstractParts parts;
  /**
   * Since the parts list is a mock a call of the destructor shall be expected
   * when this test is done.
   */
  EXPECT_CALL(parts, Die()).Times(1);
  /**
   * Construct a new song with the parts list as argument.
   */
  Song song(&parts);
  /**
   * Make sure that the new song has no name (NULL).
   */
  ASSERT_EQ((string)DEFAULT_SONG_NAME, *(song.get_name()));
  /**
   * Make sure that we got a song with the parts list set.
   */
  ASSERT_EQ(&parts, song.parts);
}


/**
 * @test Constructor with name and parts arguments shall create a song with
 *       the correct name and parts.
 */
TEST(Song, Constructor_with_name_and_parts_arguments_shall_create_a_song_with_the_correct_name_and_parts) {
  MockAbstractParts parts;
  /**
   * Since the parts list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(parts, Die()).Times(1);
  /**
   * Construct a new song with the string "Foobar" as name and a mock as parts
   * list.
   */
  Song song((string*)"Foobar", &parts);
  /**
   * Make sure that we got a song with the "Foobar" as name.    */
  ASSERT_EQ((string)"Foobar", *(song.get_name()));
  /**
   * Make sure that we got a song with the parts list set.
   */
  ASSERT_EQ(&parts, song.parts);
}


/**
 * @test Get parts shall get parts.
 */
TEST(Song, Get_parts_shall_get_parts) {
  MockAbstractParts parts;
  /**
   * Since the parts list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(parts, Die()).Times(1);
  /**
   * Construct a new song with the string "Foobar" as name.
   */
  Song song(&parts);
  /**
   * Make sure that the get_parts method returns the correct pointer.
   */
  ASSERT_EQ(&parts, song.get_parts());
}


/**
 * @test Add part shall add a part to the song.
 */
TEST(Song, Add_part_shall_add_a_part_to_the_song) {
  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new song with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(part, Die()).Times(1);

  /**
   * Call the design under test.
   */
  song.add_part(&part);

  /**
   * Get the part list from the song and validate that it contains one
   * part and it should be he mock-up part created in this test-case.
   */
  AbstractParts *parts_got = song.get_parts();

  /**
   * Make sure that there is one part in the list.
   */
  ASSERT_EQ(1, parts_got->size());

  /**
   * Make sure that the one item is exactly the same part that that was
   * provided.
   */
  ASSERT_EQ(&part, parts_got->front());
}


/**
 * @test Adding several parts shall add several parts to the song.
 */
TEST(Song, Adding_several_parts_shall_add_several_parts_to_the_song) {
  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new song with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part1;
  MockAbstractPart part2;
  MockAbstractPart part3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(part1, Die()).Times(1);
  EXPECT_CALL(part2, Die()).Times(1);
  EXPECT_CALL(part3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  song.add_part(&part1);
  song.add_part(&part2);
  song.add_part(&part3);

  /**
   * Get the part list from the song and validate that it contains three
   * parts and it should be he mock-up part created in this test-case.
   */
  AbstractParts *parts_got = song.get_parts();

  /**
   * Make sure that there is three parts in the list.
   */
  ASSERT_EQ(3, parts_got->size());

  /**
   * Make sure that the one item is exactly the same part that that was
   * provided.
   */
  ASSERT_EQ(&part1, parts_got->front());
  ASSERT_EQ(&part3, parts_got->back());
}


/**
 * @test Inserting a part shall insert a part in the song.
 */
TEST(Song, Inserting_a_part_shall_insert_a_part_in_the_song) {
  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new song with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part1;
  MockAbstractPart part2;
  MockAbstractPart part3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(part1, Die()).Times(1);
  EXPECT_CALL(part2, Die()).Times(1);
  EXPECT_CALL(part3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  song.add_part(&part1);
  song.add_part(&part3);
  song.insert_part(&part1, &part2);

  /**
   * Get the part list from the song and validate that it contains three
   * parts and it should be he mock-up part created in this test-case.
   */
  AbstractParts *parts_got = song.get_parts();

  /**
   * Make sure that there is three parts in the list.
   */
  ASSERT_EQ(3, parts_got->size());

  /**
   * Make sure that the one item is exactly the same part that that was
   * provided.
   */
  ASSERT_EQ(&part2, parts_got->front());
  ASSERT_EQ(&part3, parts_got->back());
}


/**
 * @test Inserting a part before an unexsisting part shall add the
 *       part to the end of the song.
 */
TEST(Song, Inserting_a_part_before_an_unexisting_part_shall_add_the_part_to_the_end_of_the_song) {
  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new song with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part1;
  MockAbstractPart part2;
  MockAbstractPart part3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(part1, Die()).Times(1);
  EXPECT_CALL(part2, Die()).Times(1);
  EXPECT_CALL(part3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  song.add_part(&part1);
  song.insert_part(&part3, &part2);

  /**
   * Get the part list from the song and validate that it contains three
   * parts and it should be he mock-up part created in this test-case.
   */
  AbstractParts *parts_got = song.get_parts();

  /**
   * Make sure that there is three parts in the list.
   */
  ASSERT_EQ(2, parts_got->size());

  /**
   * Make sure that the one item is exactly the same part that that was
   * provided.
   */
  ASSERT_EQ(&part1, parts_got->front());
  ASSERT_EQ(&part2, parts_got->back());
}


/**
 * @test Deleting a part shall delete a part from the song.
 */
TEST(Song, Deleting_a_part_shall_delete_a_part_from_the_song) {
  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new song with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part1;
  MockAbstractPart part2;
  MockAbstractPart part3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(part1, Die()).Times(1);
  EXPECT_CALL(part2, Die()).Times(1);
  EXPECT_CALL(part3, Die()).Times(1);

  /**
   * Add some parts
   */
  song.add_part(&part1);
  song.add_part(&part2);
  song.add_part(&part3);

  /**
   * Calle the design under test.
   */
  song.delete_part(&part1);

  /**
   * Get the part list from the song and validate that it contains three
   * parts and it should be he mock-up part created in this test-case.
   */
  AbstractParts *parts_got = song.get_parts();

  /**
   * Make sure that there is three parts in the list.
   */
  ASSERT_EQ(2, parts_got->size());

  /**
   * Make sure that the one item is exactly the same part that that was
   * provided.
   */
  ASSERT_EQ(&part2, parts_got->front());
  ASSERT_EQ(&part3, parts_got->back());
}


/**
 * @test Pattern is used-method shall return true if pattern is used.
 */
TEST(Song, Pattern_is_used_method_shall_return_true_if_pattern_is_used) {
  /**
   * Provide a pattern mock-up to search for.
   */
  MockAbstractPattern pattern;

  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new part with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die());
  EXPECT_CALL(part, Die()).Times(1);

  /**
   * Replace the part_is_used() method in the Part mock-up to always return
   * true, in effect the first part in the list appears to have the pattern.
   */
  ON_CALL(part, pattern_is_used(_)).WillByDefault(Invoke(&return_true));
  EXPECT_CALL(part, pattern_is_used(_)).Times(1);

  /**
   * Add some patterns
   */
  song.add_part(&part);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(true == song.pattern_is_used(&pattern));
}


/**
 * @test Pattern is used-method shall return false if pattern is not used.
 */
TEST(Song, Pattern_is_used_method_shall_return_false_if_pattern_is_not_used) {
  /**
   * Provide a pattern mock-up to search for.
   */
  MockAbstractPattern pattern;

  /**
   * Provide a real list of parts to system under test.
   */
  AbstractParts parts;

  /**
   * Construct a new part with the part list.
   */
  Song song(&parts);

  /**
   * Create a part mock-up to add.
   */
  MockAbstractPart part;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die());
  EXPECT_CALL(part, Die()).Times(1);
  EXPECT_CALL(part, pattern_is_used(_)).Times(1);

  /**
   * Replace the part_is_used() method in the Part mock-up to always return
   * true, in effect the first part in the list appears to have the pattern.
   */
  ON_CALL(part, pattern_is_used(_)).WillByDefault(Invoke(&return_false));

  /**
   * Add some patterns
   */
  song.add_part(&part);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(false == song.pattern_is_used(&pattern));
}


