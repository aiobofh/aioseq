/**
 * @file ChannelRow.h
 *
 * Interface for manipulationg a ChannelRow instance.
 */

#ifndef _CHANNEL_ROW_H_
#define _CHANNEL_ROW_H_

#include <gtest/gtest_prod.h>

#include "AbstractChannelRow.h"

#include "AbstractNote.h"
#include "AbstractParameter.h"

using std::list;

/**
 * Represennt a row of a channel.
 */
class ChannelRow : public AbstractChannelRow {
private:
  FRIEND_TEST(ChannelRow, Constructor_shall_have_set_the_notes_and_parameters);

  //  friend class ChannelRowBuilder;

  /**
   * Pointer to a std::list of pointers to Note objects.
   */
  list<AbstractNote*> *notes;

  /**
   * Pointer to a std::list of pointers to Parameter objects.
   */
  list<AbstractParameter*> *parameters;
public:
  /**
   * Constructor which just arranges the notes and parameters into the channel
   * row.
   *
   * @param notes Pointer to a std::list of Note objects.
   * @param parameters Pointer to a std::list of Parameter objects.
   */
  ChannelRow(list<AbstractNote*> *notes, list<AbstractParameter*> *parameters);
};

#endif
