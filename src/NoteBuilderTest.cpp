/**
 * @file NoteBuilderTest.cpp
 *
 * Test case for the NoteBuilder object factory.
 */

#include <gtest/gtest.h>

#include "MockAbstractNote.h"

#include "NoteBuilder.h"

static NoteBuilder<MockAbstractNote> note_builder;

/**
 * @test Create method shall create a Note object.
 */
TEST(NoteBuilder, Create_method_shall_create_a_note_object) {
  MockAbstractNote *note;
  note = note_builder.create();
  ASSERT_FALSE(NULL == note);
}
