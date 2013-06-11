/**
 * SongBuilderTest.cpp
 *
 * Test cases for the SongBuilder class.
 */

#include <gtest/gtest.h>

#include "SongBuilder.h"
#include "Song.h"
#include "AbstractSong.h"
#include "MockAbstractSong.h"
#include "MockAbstractParts.h"

using ::testing::Mock;
using ::testing::_;


/**
 * @test Creae method shall creatae a song.
 */
TEST(SongBuilderTest, Create_method_shall_create_a_song) {

  string *name = new string("foobar");

  AbstractSong *song;

  /**
   * Set-up the system under test using mocks inside.
   */
  SongBuilder<MockAbstractSong> song_builder;

  /**
   * Call the design under test.
   */
  song = song_builder.create(name);

  /**
   * A song shall be created.
   */
  ASSERT_FALSE(NULL == song);

}


/**
 * @test Destroy method shall destroy a song.
 */
TEST(SongBuilderTest, Destroy_method_shall_destroy_a_song) {

  /**
   * Set-up the system under test.
   */
  SongBuilder<MockAbstractSong> song_builder;

  /**
   * Create an empty list of parts (as a mock).
   */
  AbstractParts *parts = new AbstractParts();

  /**
   * Create a new song.
   */
  AbstractSong *song = new Song(parts);

  /**
   * Call the design under test.
   */
  song_builder.destroy(&song);

  /**
   * Make sure that the destroy method sets the referenced song pointer to
   * NULL.
   */
  ASSERT_EQ(NULL, song);

}
