/**
 * @file AbstractPatternRowBuilder.h
 *
 * Interface for a pattern row factory.
 */

#ifndef _ABSTRACT_PATTERN_ROW_BUILDER_
#define _ABSTRACT_PATTERN_ROW_BUILDER_

#include "AbstractPatternRow.h" /* Include in mock */
#include "AbstractChannelRowBuilder.h" /* Include in mock */

// Mock: template<class PATTERN_ROW_CLASS>
/**
 * PatternRow object factory class.
 */
class AbstractPatternRowBuilder {

public:

  /**
   * Dummy constructor.
   */
  AbstractPatternRowBuilder() {};


  /**
   * Constructor that takes a pointer to the channel row builder.
   *
   * @param channel_row_builder A pointer to a channel row builder object factory instance.
   */
  AbstractPatternRowBuilder(AbstractChannelRowBuilder *channel_row_builder) {};


  /**
   * Create a pattern row with the specified number of channel rows, and two
   * arrays descibing the contetns, where the first tells the factory the
   * number of notes every channel shall have, and the second the number of
   * parameters every channel shall have.
   *
   * @param number_of_channel_rows The number of channel rows to create.
   *
   * @param notes_in_channels      An pointer to an array of the same length
   *                               as the number of channels, cintaining the
   *                               number of notes to create in each channel
   *                               row.
   *
   * @param parameters_in_channels A pointer to an array of the same length as
   *                               the number of channels to create,
   *                               containing the number of parameters to
   *                               create in each channel row.
   *
   * @return A pointer to the new object.
   */
  virtual AbstractPatternRow *create(int number_of_channel_rows,
				     int notes_in_channels[],
				     int parameters_in_channels[]) = 0;


  /**
   * Destroy the specified pattern row.
   *
   * @param pattern_row A pointer to the pointer variable referencing a
   *                    pattern row.
   */
  virtual void destroy(AbstractPatternRow **pattern_row) = 0;

};

#endif
