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

private:

  /**
   * A pointer to a channel row builder object factory instance.
   */
  AbstractChannelRowBuilder *channel_row_builder;

public:

  /**
   * @copydoc AbstractPatternRowBuilder::AbstractPatternRowBuilder(AbstractChannelRowBuilder *)
   */
  PatternRowBuilder(AbstractChannelRowBuilder *channel_row_builder) {
    this->channel_row_builder = channel_row_builder;
  };


  /**
   * @copydoc AbstractPatternRowBuilder::create(int,int [],int [])
   */
  virtual AbstractPatternRow *create(int number_of_channel_rows,
				     int notes_in_channels[],
				     int parameters_in_channels[]) {
    AbstractPatternRow *pattern_row;

    AbstractChannelRows *channel_rows = new AbstractChannelRows();

    for (int i = 0; i < number_of_channel_rows; i++) {
      channel_rows->push_back(channel_row_builder->create(notes_in_channels[i],
                                                          parameters_in_channels[i]));
    }

    pattern_row = new PATTERN_ROW_CLASS(channel_rows);

    return pattern_row;
  };


  /**
   * @copydoc AbstractPatternRowBuilder::destroy(AbstractPatternRow **)
   */
  void destroy(AbstractPatternRow **pattern_row) {
    AbstractChannelRows *channel_rows = (*pattern_row)->get_channel_rows();

    /**
     * Destroy all the channel rows on this pattern row and remove them from
     * the list of channel rows.
     */
    while (!channel_rows->empty()) {
      AbstractChannelRow *channel_row = channel_rows->back();
      channel_row_builder->destroy(&channel_row);
      channel_rows->pop_back();
    }

    /**
     * Destruct the list allocated in the create() method.
     */
    delete channel_rows;

    /**
     * Destruct the pattern row.
     */
    delete *pattern_row;
    *pattern_row = NULL;
  };

};

#endif
