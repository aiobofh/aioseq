/**
 * @NameTest.cpp
 *
 * Test cases for Name.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include "Name.h"

TEST(Name, Constructor_with_arguments_shall_store_a_name) {
  Name name("Foobar");
  ASSERT_EQ((string)"Foobar", *(name.name));
}

TEST(Name, Name_setter_with_a_const_char_shall_set_name_and_remove_old_name) {
  Name name("Foobar");
  name.set_name("Barfoo");
  ASSERT_EQ((string)"Barfoo", *(name.name));
}

TEST(Name, Name_setter_with_a_const_string_shall_set_name_and_remove_old_name) {
  Name name("Foobar");
  name.set_name((string*)"Barfoo");
  ASSERT_EQ((string)"Barfoo", *(name.name));
}

TEST(Name, Name_getter_shalle_get_nme) {
  Name name("Foobar");
  ASSERT_EQ((string)"Foobar", *(name.get_name()));
}
