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

/**
 * Represent at song according to the constraints of AbstractSong.
 */
class Song : public AbstractSong {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_without_arguments_shall_create_a_song_with_no_name_and_no_parts);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_name_argument_shall_create_a_song_with_a_name);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_parts_argument_shall_create_a_song_with_correct_parts);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Constructor_with_name_and_parts_arguments_shall_create_a_song_with_the_correct_name_and_parts);

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Song, Set_name_shall_set_new_name);

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
   * Destructor is cleaning up the object (the name string).
   */
  ~Song();

  /**
   * @copydoc AbstractSong::get_parts()
   */
  AbstractParts *get_parts();


  /**
   * @copydoc AbstractSong::set_name(const string *)
   */
  void set_name(const string *name);


  /**
   * @copydoc AbstractSong::get_name()
   */
  string *get_name();

};

#endif
