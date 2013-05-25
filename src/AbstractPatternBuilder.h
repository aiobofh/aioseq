/**
 * @file AbstractPatternBuilder.h
 *
 * Interface for a pattern factory.
 */

#ifndef _ABSTRACT_PATTERN_BUILDER_H_
#define _ABSTRACT_PATTERN_BUILDER_H_

#include "AbstractPatternRowBuilder.h"
#include "AbstractPattern.h"

// Mock: template<class PATTERN_CLASS>
/**
 * Pattern object factory class.
 */
class AbstractPatternBuilder {

private:

  /**
   * A pointer to a pattern row builder object factory.
   */
  AbstractPatternRowBuilder *pattern_row_builder;

public:

  /**
   * Dummy constructor.
   */
  AbstractPatternBuilder() {};


  /**
   * Constructor with arguments.
   *
   * @param pattern_row_builder @copydoc AbstractPatternBuilder::pattern_row_builder
   */
  AbstractPatternBuilder(AbstractPatternRowBuilder *pattern_row_builder) {};


  /**
   * Create a new pattern object.
   *
   * @param number_of_channel_rows The number of channel rows the pattern
   *                               shall have.
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
   * @param number_of_pattern_rows The number of rows of which the pattern
   *                               consist.
   *
   * @return A pointer to the new pattern object.
   */
  virtual AbstractPattern *create(int number_of_channel_rows,
                                  int notes_in_channels[],
                                  int parameters_in_channels[],
                                  int number_of_pattern_rows) = 0;


  /**
   * Destroy a pattern object.
   *
   * @param pattern A pointer to a pointer referencing a pattern object to
   *        destroy.
   */
  virtual void destroy(AbstractPattern **pattern) = 0;

};

#endif
