/**
 * @file ProjectTest.cpp
 *
 * Test cases for the Project class.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractPattern.h"
#include "MockAbstractPatterns.h"
#include "MockAbstractSong.h"
#include "MockAbstractSongs.h"
#include "Project.h"

using ::testing::_;
using ::testing::Invoke;

/**
 * Stub function to replace the pattern_is_used method in the MockAbstractSong
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
 * Stub function to replace the pattern_is_used method in the MockAbstractSong
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
 * @test Constructor with no arguments shall create a project with default name.
 */
TEST(Project, Constructor_withj_no_arguments_shall_create_a_project_with_default_name) {
  Project project;
  ASSERT_EQ((string)DEFAULT_PROJECT_NAME, *(project.get_name()));
}

/**
 * @test Constructor with songs and patterns as arguments shall create a project.
 */
TEST(Project, Constructor_with_songs_and_patterns_as_arguments_shall_create_a_project) {
  /**
   * Instantiate a mock-up for a pattern list.
   */
  MockAbstractPatterns patterns;
  /**
   * Isntantiate a mock-up for a song list.
   */
  MockAbstractSongs songs;
  /**
   * Since the pattern list mock-up will be deleted at the end of the test case
   * lets expect it to be deleted.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Since the song list mock-up will be deleted at the end of the test case
   * lets expect it to be deleted.
   */
  EXPECT_CALL(songs, Die()).Times(1);
  /**
   * Create the system under test.
   */
  Project *project = new Project(&songs, &patterns);
  /**
   * Make sure that a project is created.
   */
  ASSERT_FALSE(NULL == project);
  /**
   * Make sure that the project has a default name.
   */
  ASSERT_EQ((string)DEFAULT_PROJECT_NAME, *(project->get_name()));
  /**
   * Make sure that the correct list of patterns is used within the project.
   */
  ASSERT_EQ(&patterns, project->patterns);
  /**
   * Make sure that the correct list of songs is used within the project.
   */
  ASSERT_EQ(&songs, project->songs);
}


/**
 * @test Constructor with name, songs and patterns as arguments shall create a project.
 */
TEST(Project, Constructor_with_name_songs_and_patterns_as_arguments_shall_create_a_project) {
  /**
   * Instantiate a mock-up for a pattern list.
   */
  MockAbstractPatterns patterns;
  /**
   * Isntantiate a mock-up for a song list.
   */
  MockAbstractSongs songs;
  /**
   * Since the pattern list mock-up will be deleted at the end of the test case
   * lets expect it to be deleted.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Since the song list mock-up will be deleted at the end of the test case
   * lets expect it to be deleted.
   */
  EXPECT_CALL(songs, Die()).Times(1);
  /**
   * Create the system under test.
   */
  Project *project = new Project((string*)"Foobar", &songs, &patterns);
  /**
   * Make sure that a project is created.
   */
  ASSERT_FALSE(NULL == project);
  /**
   * Make sure that the correct list of patterns is used within the project.
   */
  ASSERT_EQ(&patterns, project->patterns);
  /**
   * Make sure that the correct list of songs is used within the project.
   */
  ASSERT_EQ(&songs, project->songs);
  /**
   * Make sure that the correct name is set to the project.
   */
  ASSERT_EQ((string)"Foobar", *(project->get_name()));
}


/**
 * @test Get patterns shall get patterns.
 */
TEST(Project, Get_patterns_shall_get_patterns) {
  MockAbstractPatterns patterns;
  /**
   * Since the patterns list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(patterns, Die()).Times(1);
  /**
   * Construct a new project with the string "Foobar" as name.
   */
  Project project(NULL, &patterns);
  /**
   * Make sure that the get_patters method returns the correct pointer.
   */
  ASSERT_EQ(&patterns, project.get_patterns());
}


/**
 * @test Add pattern shall add a pattern to the project.
 */
TEST(Project, Add_pattern_shall_add_a_pattern_to_the_project) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new project with the pattern list.
   */
  Project project(NULL, &patterns);

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
  project.add_pattern(&pattern);

  /**
   * Get the pattern list from the song and validate that it contains one
   * pattern and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = project.get_patterns();

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
 * @test Adding several patterns shall add several patterns to the project.
 */
TEST(Project, Adding_several_patterns_shall_add_several_patterns_to_the_project) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new project with the pattern list.
   */
  Project project(NULL, &patterns);

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
  project.add_pattern(&pattern1);
  project.add_pattern(&pattern2);
  project.add_pattern(&pattern3);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = project.get_patterns();

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
 * @test Inserting a pattern shall insert a pattern in the project.
 */
TEST(Project, Inserting_a_pattern_shall_insert_a_pattern_in_the_project) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new project with the pattern list.
   */
  Project project(NULL, &patterns);

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
  project.add_pattern(&pattern1);
  project.add_pattern(&pattern3);
  project.insert_pattern(&pattern1, &pattern2);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = project.get_patterns();

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
 *       pattern to the end of the project.
 */
TEST(Project, Inserting_a_pattern_before_an_unexisting_pattern_shall_add_the_pattern_to_the_end_of_the_project) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new project with the pattern list.
   */
  Project project(NULL, &patterns);

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
  project.add_pattern(&pattern1);
  project.insert_pattern(&pattern3, &pattern2);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = project.get_patterns();

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
 * @test Deleting a pattern shall delete a pattern from the project.
 */
TEST(Project, Deleting_a_pattern_shall_delete_a_pattern_from_the_project) {
  /**
   * Provide a real list of patterns to system under test.
   */
  AbstractPatterns patterns;

  /**
   * Construct a new project with the pattern list.
   */
  Project project(NULL, &patterns);

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
  project.add_pattern(&pattern1);
  project.add_pattern(&pattern2);
  project.add_pattern(&pattern3);

  /**
   * Calle the design under test.
   */
  project.delete_pattern(&pattern1);

  /**
   * Get the pattern list from the song and validate that it contains three
   * patterns and it should be he mock-up pattern created in this test-case.
   */
  AbstractPatterns *patterns_got = project.get_patterns();

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
 * @test Add song shall add a song to the project.
 */
TEST(Project, Add_song_shall_add_a_song_to_the_project) {
  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new project with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(song, Die()).Times(1);

  /**
   * Call the design under test.
   */
  project.add_song(&song);

  /**
   * Get the song list from the song and validate that it contains one
   * song and it should be he mock-up song created in this test-case.
   */
  AbstractSongs *songs_got = project.get_songs();

  /**
   * Make sure that there is one song in the list.
   */
  ASSERT_EQ(1, songs_got->size());

  /**
   * Make sure that the one item is exactly the same song that that was
   * provided.
   */
  ASSERT_EQ(&song, songs_got->front());
}


/**
 * @test Adding several songs shall add several songs to the project.
 */
TEST(Project, Adding_several_songs_shall_add_several_songs_to_the_project) {
  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new project with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song1;
  MockAbstractSong song2;
  MockAbstractSong song3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(song1, Die()).Times(1);
  EXPECT_CALL(song2, Die()).Times(1);
  EXPECT_CALL(song3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  project.add_song(&song1);
  project.add_song(&song2);
  project.add_song(&song3);

  /**
   * Get the song list from the song and validate that it contains three
   * songs and it should be he mock-up song created in this test-case.
   */
  AbstractSongs *songs_got = project.get_songs();

  /**
   * Make sure that there is three songs in the list.
   */
  ASSERT_EQ(3, songs_got->size());

  /**
   * Make sure that the one item is exactly the same song that that was
   * provided.
   */
  ASSERT_EQ(&song1, songs_got->front());
  ASSERT_EQ(&song3, songs_got->back());
}


/**
 * @test Inserting a song shall insert a song in the project.
 */
TEST(Project, Inserting_a_song_shall_insert_a_song_in_the_project) {
  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new project with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song1;
  MockAbstractSong song2;
  MockAbstractSong song3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(song1, Die()).Times(1);
  EXPECT_CALL(song2, Die()).Times(1);
  EXPECT_CALL(song3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  project.add_song(&song1);
  project.add_song(&song3);
  project.insert_song(&song1, &song2);

  /**
   * Get the song list from the song and validate that it contains three
   * songs and it should be he mock-up song created in this test-case.
   */
  AbstractSongs *songs_got = project.get_songs();

  /**
   * Make sure that there is three songs in the list.
   */
  ASSERT_EQ(3, songs_got->size());

  /**
   * Make sure that the one item is exactly the same song that that was
   * provided.
   */
  ASSERT_EQ(&song2, songs_got->front());
  ASSERT_EQ(&song3, songs_got->back());
}


/**
 * @test Inserting a song before an unexsisting song shall add the
 *       song to the end of the project.
 */
TEST(Project, Inserting_a_song_before_an_unexisting_song_shall_add_the_song_to_the_end_of_the_project) {
  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new project with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song1;
  MockAbstractSong song2;
  MockAbstractSong song3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(song1, Die()).Times(1);
  EXPECT_CALL(song2, Die()).Times(1);
  EXPECT_CALL(song3, Die()).Times(1);

  /**
   * Call the design under test.
   */
  project.add_song(&song1);
  project.insert_song(&song3, &song2);

  /**
   * Get the song list from the song and validate that it contains three
   * songs and it should be he mock-up song created in this test-case.
   */
  AbstractSongs *songs_got = project.get_songs();

  /**
   * Make sure that there is three songs in the list.
   */
  ASSERT_EQ(2, songs_got->size());

  /**
   * Make sure that the one item is exactly the same song that that was
   * provided.
   */
  ASSERT_EQ(&song1, songs_got->front());
  ASSERT_EQ(&song2, songs_got->back());
}


/**
 * @test Deleting a song shall delete a song from the project.
 */
TEST(Project, Deleting_a_song_shall_delete_a_song_from_the_project) {
  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new project with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song1;
  MockAbstractSong song2;
  MockAbstractSong song3;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(song1, Die()).Times(1);
  EXPECT_CALL(song2, Die()).Times(1);
  EXPECT_CALL(song3, Die()).Times(1);

  /**
   * Add some songs
   */
  project.add_song(&song1);
  project.add_song(&song2);
  project.add_song(&song3);

  /**
   * Calle the design under test.
   */
  project.delete_song(&song1);

  /**
   * Get the song list from the song and validate that it contains three
   * songs and it should be he mock-up song created in this test-case.
   */
  AbstractSongs *songs_got = project.get_songs();

  /**
   * Make sure that there is three songs in the list.
   */
  ASSERT_EQ(2, songs_got->size());

  /**
   * Make sure that the one item is exactly the same song that that was
   * provided.
   */
  ASSERT_EQ(&song2, songs_got->front());
  ASSERT_EQ(&song3, songs_got->back());
}








/**
 * @test Pattern is used-method shall return true if pattern is used.
 */
TEST(Project, Pattern_is_used_method_shall_return_true_if_pattern_is_used) {
  /**
   * Provide a pattern mock-up to search for.
   */
  MockAbstractPattern pattern;

  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new song with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die());
  EXPECT_CALL(song, Die()).Times(1);

  /**
   * Replace the pattern_is_used() method in the Song mock-up to always return
   * true, in effect the first song in the list appears to have the pattern.
   */
  ON_CALL(song, pattern_is_used(_)).WillByDefault(Invoke(&return_true));
  EXPECT_CALL(song, pattern_is_used(_)).Times(1);

  /**
   * Add some patterns
   */
  project.add_song(&song);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(true == project.pattern_is_used(&pattern));
}


/**
 * @test Pattern is used-method shall return false if pattern is not used.
 */
TEST(Project, Pattern_is_used_method_shall_return_false_if_pattern_is_not_used) {
  /**
   * Provide a pattern mock-up to search for.
   */
  MockAbstractPattern pattern;

  /**
   * Provide a real list of songs to system under test.
   */
  AbstractSongs songs;

  /**
   * Construct a new song with the song list.
   */
  Project project(&songs, NULL);

  /**
   * Create a song mock-up to add.
   */
  MockAbstractSong song;

  /**
   * Since the mock will be deleted after the test-case a call to its
   * destructor is to be expected.
   */
  EXPECT_CALL(pattern, Die());
  EXPECT_CALL(song, Die()).Times(1);
  EXPECT_CALL(song, pattern_is_used(_)).Times(1);

  /**
   * Replace the pattern_is_used() method in the Song mock-up to always return
   * true, in effect the first song in the list appears to have the pattern.
   */
  ON_CALL(song, pattern_is_used(_)).WillByDefault(Invoke(&return_false));

  /**
   * Add some patterns
   */
  project.add_song(&song);

  /**
   * Calle the design under test and make sure that the pattern_is_used-
   * method returns true.
   */
  ASSERT_TRUE(false == project.pattern_is_used(&pattern));
}

