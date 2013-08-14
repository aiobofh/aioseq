/**
 * @file Header.h
 *
 * Declaration of the interface to manipulate and use a
 * header for something.
 *
 * @copydoc AbstractHeader.h
 */

#ifndef _HEADER_H_
#define _HEADER_H_

#include <string>

#include <gtest/gtest_prod.h>

#include "AbstractHeader.h"
#include "Name.h"

/**
 * The default header name if nothing is provided.
 */
#define DEFAULT_HEADER_NAME "New"

/**
 * Represent a header for something according to the constraints of
 * AbstractHeader.
 */
class Header : public AbstractHeader, public Name {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Constructor_without_arguments_shall_create_a_header_with_a_default_name_and_no_mute_and_no_solo);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Constructor_without_arguments_shall_create_a_header_with_the_correct_settings);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Mute_shall_mute_header);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Unmute_shall_unmute_header);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Solo_shall_solo_header);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Header, Unsolo_shall_unsolo_header);


public:

  /**
   * Dummy constructor.
   */
  Header();


  /**
   * @copydoc AbstractHeader::AbstractHeader(const string *, bool, bool)
   */
  Header(const string *name, bool mute, bool solo);


  /**
   * @copydoc AbstractHeader::mute
   */
  void mute();


  /**
   * @copydoc AbstractHeader::unmute
   */
  void unmute();


  /**
   * @copydoc AbstractHeader::solo
   */
  void solo();


  /**
   * @copydoc AbstractHeader::unsolo
   */
  void unsolo();

};

#endif
