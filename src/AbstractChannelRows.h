/**
 * @file AbstractChannelRows.h
 */

#ifndef _ABSTRACT_CHANNEL_ROWS_H_
#define _ABSTRACT_CHANNEL_ROWS_H_

#include "AbstractChannelRow.h"

#include <list>

using std::list;

/**
 * List wrapper class to store pointers to channel rows.
 */
class AbstractChannelRows : public list<AbstractChannelRow*> {
public:
  AbstractChannelRows() : list<AbstractChannelRow*>() {};
};

#endif
