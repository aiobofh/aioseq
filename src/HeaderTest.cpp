/**
 * @file HeaderTest.cpp
 *
 * Test cases for Header.
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Header.h"

using namespace std;

/**
 * @test Constructor without arguments shall create a header with a default
 *       name and no mute and no solo.
 */
TEST(Header, Constructor_without_arguments_shall_create_a_header_with_a_default_name_and_no_mute_and_no_solo) {

  /**
   * Construct a new header without any arguments.
   */
  Header header;

  /**
   * Make sure that the new header has a default name.
   */
  ASSERT_EQ((string)DEFAULT_HEADER_NAME, *(header.get_name()));

  /**
   * Make sure that the header is not muted.
   */
  ASSERT_FALSE(header.muted);

  /**
   * Make sure that the header is not solo.
   */
  ASSERT_FALSE(header.soloed);

}


/**
 * @test Constructor with arguments shall create a header with the correct
 *       settings.
 */
TEST(Header, Constructor_without_arguments_shall_create_a_header_with_the_correct_settings) {

  /**
   * Construct a new header with a name and unmute and solo.
   */
  Header header1((const string*)"Foobar", false, true);

  /**
   * Construct a new header with a name and mute and unsolo.
   */
  Header header2((const string*)"Barfoo", true, false);

  /**
   * Make sure that the first header has a name.
   */
  ASSERT_EQ((string)"Foobar", *(header1.get_name()));

  /**
   * Make sure that the second header has a name.
   */
  ASSERT_EQ((string)"Barfoo", *(header2.get_name()));

  /**
   * Make sure that the first header is not muted.
   */
  ASSERT_FALSE(header1.muted);

  /**
   * Make sure that the second header is muted.
   */
  ASSERT_TRUE(header2.muted);

  /**
   * Make sure that the first header is solo.
   */
  ASSERT_TRUE(header1.soloed);

  /**
   * Make sure that the second header is not solo.
   */
  ASSERT_FALSE(header2.soloed);

}


/**
 * @test Mute shall mute header.
 */
TEST(Header, Mute_shall_mute_header) {

  /**
   * Construct a new header without any arguments.
   */
  Header header;

  /**
   * Call the design under test.
   */
  header.mute();

  /**
   * Make sure that the header is muted.
   */
  ASSERT_TRUE(header.muted);

}


/**
 * @test Un-mute shall un-mute header.
 */
TEST(Header, Unmute_shall_unmute_header) {

  /**
   * Construct a new header without any arguments.
   */
  Header header;

  /**
   * Make sure that the header is muted.
   */
  header.muted = true;

  /**
   * Call the design under test.
   */
  header.unmute();

  /**
   * Make sure that the header is not muted.
   */
  ASSERT_FALSE(header.muted);

}


/**
 * @test Solo shall solo header.
 */
TEST(Header, Solo_shall_solo_header) {

  /**
   * Construct a new header without any arguments.
   */
  Header header;

  /**
   * Call the design under test.
   */
  header.solo();

  /**
   * Make sure that the header is muted.
   */
  ASSERT_TRUE(header.soloed);

}


/**
 * @test Un-solo shall un-solo header.
 */
TEST(Header, Unsolo_shall_unsolo_header) {

  /**
   * Construct a new header without any arguments.
   */
  Header header;

  /**
   * Make sure that the header is muted.
   */
  header.soloed = true;

  /**
   * Call the design under test.
   */
  header.unsolo();

  /**
   * Make sure that the header is not muted.
   */
  ASSERT_FALSE(header.muted);

}
