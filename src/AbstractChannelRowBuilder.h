/**
 * @file AbstractChannelRowBuilder.h
 *
 * Interface for a channel row factory.
 */

#ifndef _ABSTRACT_CHANNEL_ROW_BUILDER_H_
#define _ABSTRACT_CHANNEL_ROW_BUILDER_H_

#include "AbstractNoteBuilder.h"
#include "AbstractParameterBuilder.h"
#include "AbstractChannelRow.h"

// Mock: template<class CHANNEL_ROW_CLASS>
/**
 * ChannelRow object facatory class.
 */
class AbstractChannelRowBuilder {

protected:

  /**
   * A pointer to a note builder object factory instance.
   */
  AbstractNoteBuilder *note_builder;


  /**
   * A pointer to a parameter builder object factory instance.
   */
  AbstractParameterBuilder *parameter_builder;

public:

  /**
   * Dummy constructor.
   */
  AbstractChannelRowBuilder() {}; // Mock

  /**
   * Constructor that takes pointers to the builders for notes and parameters.
   *
   * @param note_builder @copydoc AbstractChannelRowBuilder::note_builder
   * @param parameter_builder @copydoc AbstractChannelRowBuilder::parameter_builder.
   *
   */
  AbstractChannelRowBuilder(AbstractNoteBuilder *note_builder, // Mock
			    AbstractParameterBuilder *parameter_builder) {}; // Mock


  /**
   * Create a channel row with the specified number of notes and parameters.
   *
   * @param number_of_notes      The number of note instances to create.
   * @param number_of_parameters The number of parameter instances to create.
   *
   * @return A pointer to the new channel row.
   */
  virtual AbstractChannelRow *create(int number_of_notes,
				     int number_of_parameters) = 0;

  /**
   * Destroy the specified channel row.
   *
   * @param channel_row A pointer to the pointer variable referencing a channel
   *                    row instance.
   */
  virtual void destroy(AbstractChannelRow **channel_row) = 0;

};

#endif
