/**
 * @file PatternRow.h
 *
 * @copydoc AbstractPatternRow.h
 */

#ifndef _PATTERN_ROW_H_
#define _PATTERN_ROW_H_

#include <gtest/gtest_prod.h>

#include "AbstractPatternRow.h"

#include "AbstractChannelRows.h"

using std::list;

/**
 * @copydoc AbstractPatternRow
 */
class PatternRow : public AbstractPatternRow {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(PatternRow, Constructor_shall_create_a_pattern_row);

private:

  /**
   * A pointer to a list of poinrters to channel row objects.
   */
  AbstractChannelRows *channel_rows;

public:

  /**
   * @copydoc AbstractPatternRow::AbstractPatternRow(AbstractChannelRows *)
   */
  PatternRow(AbstractChannelRows *channel_rows);


  /**
   * @copydoc AbstractPatternRow::get_channel_rows()
   */
  AbstractChannelRows *get_channel_rows();

};

#endif
