/**
 * @file NoteBuilderTest.cpp
 *
 * Test cases for the NoteBuilder class.
 */

#include <gtest/gtest.h>

#include "NoteBuilder.h"
#include "AbstractNote.h"
#include "MockAbstractNote.h"

using ::testing::Mock;

/**
 * @test Create method shall create a note object.
 */
TEST(NoteBuilder, Create_method_shall_create_a_note_object) {
  NoteBuilder<MockAbstractNote> note_builder;
  AbstractNote *note;

  note = note_builder.create();

  ASSERT_FALSE(NULL == note);
}


/**
 * @test Destroy method shall destroy a note object.
 */
TEST(NoteBuilder, Destroy_method_shall_destroy_a_note_object) {
  NoteBuilder<MockAbstractNote> note_builder;
  MockAbstractNote *note = new MockAbstractNote();
  AbstractNote *abstract_note = note;

  EXPECT_CALL(*note, Die()).Times(1);

  note_builder.destroy(&abstract_note);

  ASSERT_EQ(NULL, abstract_note);

  /**
   * Since the object is deleted out of context for Google Mock we need to
   * allow the memory leakage of the note mock.
   */
  Mock::AllowLeak(note);
}

