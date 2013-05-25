/**
 * @file AbstractPatternRow.h
 *
 * Interface for pattern row objects.
 */

#ifndef _ABSTRACT_PATTERN_ROW_H_
#define _ABSTRACT_PATTERN_ROW_H_

#include <list>

#include "AbstractChannelRow.h" /* Include in mock */

using std::list;


/**
 * Represent a pattern row.
 */
class AbstractPatternRow {

protected:

  /**
   * A pointer to a list of poinrters to channel row objects.
   */
  list<AbstractChannelRow*> *channel_rows;

public:

  /**
   * Dummy constructor.
   */
  AbstractPatternRow() {};


  /**
   * Constructor with argument that creates a pattern row containing any
   * number of channel rows.
   *
   * @param channel_rows @copydoc AbstractPatternRow::channel_rows
   */
  AbstractPatternRow(list<AbstractChannelRow*> *channel_rows) {}; // Mock


  /**
   * Get a list of pointers to channel row obejcts in the pattern row.
   *
   * @return A list of channels rows (channels) for the pattern row.
   */
  virtual list<AbstractChannelRow*> *get_channel_rows() = 0;

};

#endif
