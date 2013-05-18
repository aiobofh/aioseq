/**
 * @file AbstractPatternRow.h
 *
 * Pure virtual class interface for representing a pattern row.
 */

#ifndef _ABSTRACT_PATTERN_ROW_H_
#define _ABSTRACT_PATTERN_ROW_H_

#include <list>

#include "AbstractChannelRow.h"

using std::list;

/**
 * Pure virtual class to represent a PatternRow.
 */
class AbstractPatternRow {
public:
  /**
   * Dummy constructor.
   */
  AbstractPatternRow() {};

  /**
   * Dummy constructor with arguments.
   *
   * @param channels Pointer to a list of ChannelRow objects.
   */
  AbstractPatternRow(list<AbstractChannelRow*> *channels) {};
};

#endif
