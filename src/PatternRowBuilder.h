/**
 * @file PatternRowBuilder.h
 *
 * Interface for a PatternRow factory.
 */

#ifndef _PATTERN_ROW_BUILDER_H_
#define _PATTERN_ROW_BUILDER_H_

#include "AbstractNote.h"
#include "AbstractParameter.h"
#include "ChannelRowBuilder.h"
#include "AbstractPatternRow.h"

template<class PATTERN_ROW_CLASS, class CHANNEL_ROW_CLASS, class NOTE_CLASS, class PARAMETER_CLASS>
/**
 * PatternRow object factory class.
 */
class PatternRowBuilder {
private:
  ChannelRowBuilder<CHANNEL_ROW_CLASS, NOTE_CLASS, PARAMETER_CLASS> channel_row_builder;
public:
  /**
   * Create a PatternRow object.
   *
   * @todo Replace the integer with the number of channels to allocate for
   *       each pattern row with a reference to the Project Channel list 
   *       object wich will contain all the information about all the channels
   *       in the project, including notes and parameters for each channel.
   *
   * @param number_of_channels The number of channels to create in the
   *                           PatternRow object.
   *
   * @return A pointer to the new PatternRow object.
   */
  PATTERN_ROW_CLASS *create(int number_of_channels) {
    PATTERN_ROW_CLASS *pattern_row;
    list<CHANNEL_ROW_CLASS*> *channels = new list<CHANNEL_ROW_CLASS*>();
    
    for (int i = 0; i < number_of_channels; i++) {
      /**
       * @todo Remove hardcoded number of notes and number of parameters!
       */
      channels->push_back(channel_row_builder.create(4, 5));
    }

    pattern_row = new PATTERN_ROW_CLASS(channels);

    return pattern_row;
  }

  /**
   * Destroy a PatternRow object.
   *
   * @param pattern_row A pointer to a PatternRow object to destroy.
   */
  void destroy(PATTERN_ROW_CLASS *pattern_row) {
    list<CHANNEL_ROW_CLASS*> *channels;
  }
};

#endif
