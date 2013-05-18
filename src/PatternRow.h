/**
 * @file PatternRow.h
 *
 * Interface for manipulationg a PatternRow instance.
 */

#ifndef _PATTERN_ROW_H_
#define _PATTERN_ROW_H_

#include <gtest/gtest_prod.h>

#include "AbstractPatternRow.h"

#include "AbstractChannelRow.h"

using std::list;

/**
 * Represent a pattern row.
 */
class PatternRow : public AbstractPatternRow {
private:
  FRIEND_TEST(PatternRow, Constructor_shall_create_all_channels);

  list<AbstractChannelRow*> *channels;
public:
  /**
   * Constructor.
   *
   * @param channels A pointer to a list of ChannelRow objects.
   */
  PatternRow(list<AbstractChannelRow*> *channels);
};

#endif
