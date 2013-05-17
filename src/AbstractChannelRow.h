/**
 * @file AbstractChannelRow.h
 *
 * Pure virtual class interface for representing a channel row.
 */

#ifndef _ABSTRACT_CLASS_CHANNEL_ROW_H_
#define _ABSTRACT_CLASS_CHANNEL_ROW_H_

#include <list>

#include "AbstractNote.h"
#include "AbstractParameter.h"

using std::list;

/**
 * Pure virtual class to represent a ChannelRow.
 */
class AbstractChannelRow {
public:
  /**
   * Dummy constructor.
   */
  AbstractChannelRow() {};
  /**
   * Dummy consntructor.
   *
   * @param notes Pointer to a std::list of pointer to Note objects.
   * @param parameters Pointer to a std::list of pointers to Parameter objects.
   */
  AbstractChannelRow(list<AbstractNote*> *notes, list<AbstractParameter*> *parameters) {};
};

#endif
