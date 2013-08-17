/**
 * @file ChannelRow.h
 *
 * Interface for manipulationg a ChannelRow instance.
 */

#ifndef _CHANNEL_ROW_H_
#define _CHANNEL_ROW_H_

#include <gtest/gtest_prod.h>

#include "AbstractNotes.h"
#include "AbstractParameters.h"

#include "AbstractChannelRow.h"

/**
 * Represennt a row of a channel.
 */
class ChannelRow : public AbstractChannelRow {

  /**
   * Test object is friends with this class, to be able to manipulate and read
   * internal states in the channel row.
   */
  FRIEND_TEST(ChannelRow, Constructor_shall_have_set_the_notes_and_parameters);

private:

  /**
   * Pointer to a list of pointers to Note objects.
   */
  AbstractNotes *notes;

  /**
   * Pointer to a list of pointers to Parameter objects.
   */
  AbstractParameters *parameters;

public:
  /**
   * @copydoc AbstractChannelRow::AbstractChannelRow(AbstractNotes *, AbstractParameters *)
   */
  ChannelRow(AbstractNotes *notes, AbstractParameters *parameters);

  /**
   * @copydoc AbstractChannelRow::get_notes()
   */
  AbstractNotes *get_notes();

  /**
   * @copydoc AbstractChannelRow::get_parameters()
   */
  AbstractParameters *get_parameters();

};

#endif
