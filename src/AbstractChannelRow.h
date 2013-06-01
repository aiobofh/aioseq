/**
 * @file AbstractChannelRow.h
 *
 * Pure virtual class interface for representing a channel row.
 */

#ifndef _ABSTRACT_CLASS_CHANNEL_ROW_H_
#define _ABSTRACT_CLASS_CHANNEL_ROW_H_

#include "AbstractNotes.h" /* Include in mock */
#include "AbstractParameters.h" /* Include in mock */

/**
 * Pure virtual class to represent a ChannelRow.
 */
class AbstractChannelRow {

protected:
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
   * Dummy constructor.
   */
  AbstractChannelRow() {}; // Mock


  /**
   * Constructor
   *
   * @param notes @copydoc AbstractChannelRow::notes
   * @param parameters @copydoc AbstractChannelRow::parameters
   */
  AbstractChannelRow(AbstractNotes *notes, AbstractParameters *parameters) {}; // Mock


  /**
   * Get a list of pointers to note objects from the channel row.
   *
   * @return @copydoc AbstractChannelRow::notes
   */
  virtual AbstractNotes *get_notes() = 0;


  /**
   * Get a list of pointers to parameter objects from the channel row.
   *
   * @return @copydoc AbstractChannelRow::parameters
   */
  virtual AbstractParameters *get_parameters() = 0;

};

#endif
