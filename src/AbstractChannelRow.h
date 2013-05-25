/**
 * @file AbstractChannelRow.h
 *
 * Pure virtual class interface for representing a channel row.
 */

#ifndef _ABSTRACT_CLASS_CHANNEL_ROW_H_
#define _ABSTRACT_CLASS_CHANNEL_ROW_H_

#include <list>

#include "AbstractNote.h" /* Include in mock */
#include "AbstractParameter.h" /* Include in mock */

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
  AbstractChannelRow(list<AbstractNote*> *notes, list<AbstractParameter*> *parameters) {}; // Mock


  /**
   * Get a std::list of pointers to Note objects from the channel row.
   *
   * @return A pointer to the sdl::list of notes on the channel row.
   */
  virtual list<AbstractNote*> *get_notes() = 0;


  /**
   * Get a std::list of pointers to Parameter obeject from the channel row.
   *
   * @return A pointer to the sdl::list of parameters on the channel row.
   */
  virtual list<AbstractParameter*> *get_parameters() = 0;

};

#endif
