/**
 * @file SongTest.cpp
 *
 * Test cases for Song.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Song.h"
#include "MockAbstractParts.h"

using namespace std;


/**
 * @test Constructor without arguments shall create a song with no name and no
 *       parts.
 */
TEST(Song, Constructor_without_arguments_shall_create_a_song_with_no_name_and_no_parts) {
  /**
   * Construct a new song without any arguments.
   */
  Song song;
  /**
   * Make sure that the new song has no name (NULL).
   */
  ASSERT_EQ(NULL, song.name);
  /**
   * Make sure that the parts member variable is still NULL, since no parts are
   * set with the constructor.
   */
  ASSERT_EQ(NULL, song.parts);
}


/**
 * @test Constructor with name argument shall create a song with a name.
 */
TEST(Song, Constructor_with_name_argument_shall_create_a_song_with_a_name) {
  string foobar = "Foobar";
  /**
   * Construct a new song with the string "Foobar" as name.
   */
  Song song(&foobar);
  /**
   * Make sure that we got a song with the name "Foobar", by inspecting the
   * internal protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(foobar, *(song.name));
  /**
   * Make sure that the name is not a pointer to the exact same string, but
   * rather a copy of the string provided as argument to the constructor.
   */
  ASSERT_FALSE(&foobar == song.name);
  /**
   * Make sure that the parts member variable is still NULL, since no parts are
   * set with the this constructor.
   */
  ASSERT_EQ(NULL, song.parts);
}


/**
 * @test Constructor with parts agurment shall create a song with correct
 *       parts.
 */
TEST(Song, Constructor_with_parts_argument_shall_create_a_song_with_correct_parts) {
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
  ASSERT_EQ(NULL, song.name);
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
  string foobar = "Foobar";
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
  Song song(&foobar, &parts);
  /**
   * Make sure that we got a song with the "Foobar" as name, by inspecting the
   * internal protected variable containing a pointer to the name copy.
   */
  ASSERT_EQ(foobar, *(song.name));
  /**
   * Make sure that we got a song with the parts list set.
   */
  ASSERT_EQ(&parts, song.parts);
}


/**
 * @test Set name shall set new name.
 */
TEST(Song, Set_name_shall_set_new_name) {
  string foobar = "Foobar";
  string barfoo = "Barfoo";
  /**
   * Cunstruct a new song with the name "Foobar".
   */
  Song song(&foobar);
  /**
   * Set the name to "Barfoo" instead.
   */
  song.set_name(&barfoo);
  /**
   * Make sure that the new name is set to "Barfoo", by inspecting the
   * internal protected variable containing a pointer to name copy.
   */
  ASSERT_EQ(barfoo, *(song.name));
}


/**
 * @test Get name shall get name.
 */
TEST(Song, Get_name_shall_get_name) {
  string barfoo = "Barfoo";
  /**
   * Construct a new part with the name "Barfoo".
   */
  Song song(&barfoo);
  /**
   * Get the pointer to the name by using the get_name() method and make sure
   * that it is still "Barfoo".
   */
  ASSERT_EQ(barfoo, *(song.get_name()));
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
