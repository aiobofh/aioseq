/**
 * @file NoteTest.cpp
 *
 * Test cases for Note.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Note.h"

using ::testing::_;

/**
 * @test A new Note without constructor parameters shall instantiate a Note
 *       with the note value set to 0 and the velocity set to 0.
 */
TEST(Note, Constructor_without_arguments_shall_set_note_and_velocity_to_zero) {
  Note *note = new Note();
  ASSERT_EQ(note->note, 0);
  ASSERT_EQ(note->velocity, 0);
  delete note;
}

/**
 * @test A new Note with constructor arguments shall set note number and
 *       velocity accordingly.
 */
TEST(Note, Constructor_shall_set_note_number_and_velocity) {
  Note *note = new Note(10, 14);
  ASSERT_EQ(note->note, 10);
  ASSERT_EQ(note->velocity, 14);
  delete note;
}

/**
 * @test set_note shall set the note value of a Note.
 */
TEST(Note, Note_setter_shall_set_note) {
  Note *note = new Note();
  note->set_note(10);
  ASSERT_EQ(note->note, 10);
  delete note;
}

/**
 * @test set_velocity shall set the velocity value of a Note.
 */
TEST(Note, Velocity_setter_shall_set_velocity) {
  Note *note = new Note();
  note->set_velocity(10);
  ASSERT_EQ(note->velocity, 10);
  delete note;
}

/**
 * @test get_node shall get the note value of a Note.
 */
TEST(Note, Note_getter_shall_get_node) {
  Note *note = new Note(10, 14);
  ASSERT_EQ(note->get_note(), 10);
  delete note;
}

/**
 * @test get_velocity shall get the velocity of a Note.
 */
TEST(Note, Velocity_getter_shall_get_velocity) {
  Note *note = new Note(10, 14);
  ASSERT_EQ(note->get_velocity(), 14);
  delete note;
}
