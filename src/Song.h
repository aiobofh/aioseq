/**
 * @file Song.h
 *
 * This is the interface declaration for the Song class.
 *
 * @copydetails AbstractSong.h
 */

#ifndef _SONG_H_
#define _SONG_H_

#include <string>

#include <gtest/gtest_prod.h>

#include "AbstractSong.h"
#include "AbstractParts.h"

#include "Name.h"

/**
 * The default song name used if nothing is provided.
 */
#define DEFAULT_SONG_NAME "New song"

/**
 * Represent at song according to the constraints of AbstractSong.
 */
class Song : public AbstractSong, public Name {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_without_arguments_shall_create_a_song_with_a_default_name_and_no_parts);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_name_argument_shall_create_a_song_with_a_name_but_no_parts);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_parts_argument_shall_create_a_song_with_correct_parts_but_no_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_name_and_parts_arguments_shall_create_a_song_with_the_correct_name_and_parts);

private:

  /**
   * A pointer to a list of song parts that the song consists of.
   */
  AbstractParts *parts;

public:

  /**
   * @copydoc AbstractSong::AbstractSong()
   */
  Song();


  /**
   * @copydoc AbstractSong::AbstractSong(const string *)
   */
  Song(const string *name);


  /**
   * @copydoc AbstractSong::AbstractSong(AbstractParts *)
   */
  Song(AbstractParts *parts);


  /**
   * @copydoc AbstractSong::AbstractSong(const string *, AbstractParts *)
   */
  Song(const string *name, AbstractParts *parts);


  /**
   * @copydoc AbstractSong::get_parts()
   */
  AbstractParts *get_parts();


  /**
   * @copydoc AbstractSong::pattern_is_used(AbstractPattern *pattern)
   */
  bool pattern_is_used(AbstractPattern *pattern);


  /**
   * @copydoc AbstractSong::add_part(AbstractPart *part)
   */
  void add_part(AbstractPart *part);


  /**
   * @copydoc AbstractSong::insert_part(AbstractPart *, AbstractPart *)
   */
  void insert_part(AbstractPart *before, AbstractPart *part);


  /**
   * @copydoc AbstractSong::delete_part(AbstractPart *)
   */
  void delete_part(AbstractPart *part);

};

#endif
