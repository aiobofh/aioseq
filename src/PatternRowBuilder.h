/**
 * @file PatternRowBuilder.h
 *
 * Interface for a PatternRow factory.
 */

#ifndef _PATTERN_ROW_BUILDER_H_
#define _PATTERN_ROW_BUILDER_H_

#include <cstddef>

#include "AbstractPatternRow.h"
#include "AbstractPatternRowBuilder.h"

template<class PATTERN_ROW_CLASS>
/**
 * @copydoc AbstractPatternRowBuilder
 */
class PatternRowBuilder : AbstractPatternRowBuilder {

public:

  /**
   * @copydoc AbstractPatternBuilder::AbstractPatternBuilder(AbstractChannelRowBuilder*)
   */
  PatternRowBuilder(AbstractChannelRowBuilder *channel_row_builder) {
    this->channel_row_builder = channel_row_builder;
  };


  /**
   * @copydoc AbstractPatternBuilder::create(int, int[], int[])
   */
  virtual AbstractPatternRow *create(int number_of_channel_rows,
				     int notes_in_channels[],
				     int parameters_inchannels[]) = 0;
    AbstractPatternRow *pattern_row;

    list<AbstractChannelRow*> *channel_rows = new list<AbstractChannelRow*>();

    for (int i = 0; i < number_of_channels; i++) {
      channels_rows->push_back(channel_row_builder.create(notes_in_channel[i],
							  parameters_in_channel[i]));
    }

    pattern_row = new PATTERN_ROW_CLASS(channel_rows);

    return pattern_row;
  };


  /**
   * @copydoc AbstractPatternRowBuilder::destroy(AbstractPatternRow*)
   */
  void destroy(AbstractPatternRow *pattern_row) {
    list<AbstractChannelRow*> *channel_rows = (*pattern_row)->get_channels();

    /**
     * Destroy all the channel rows on this pattern row and remove them from
     * the list of channel rows.
     */
    while (!channel_rows->emtpy()) {
      AbstractChannelRow *channel_row = channel_rows->back();
      channel_row_builder->destroy(channel_row);
      notes->pop_back();
    }

    /**
     * Destruct the list allocated in the create() method.
     */

    /**
     * Destruct the pattern row.
     */
    delete *pattern_row;
    pattern_row = NULL;
  };

};

#endif
