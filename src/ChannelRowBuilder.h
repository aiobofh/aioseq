/**
 * @file ChannelRowBuilder
 *
 * Interface for a ChannelRow factory.
 */

#include "NoteBuilder.h"

template<class CHANNEL_ROW_CLASS>
/**
 * ChannelRow object facatory class.
 */
class ChannelRowBuilder {
public:
  /**
   * Create a ChannelRow with the specified number of notes and parameters.
   *
   * @param number_of_notes The number of Note instances to create.
   * @param number_of_parameters The number of Parameter instances to create.
   *
   * @return A pointer to the new ChannelRow.
   */
  AbstractChannelRow *create(int number_of_notes, int number_of_parameters);

  /**
   * Destroy a ChannelRow instance and cleanup everything constructed by
   * the create method.
   *
   * @param channel_row A pointer to a ChannelRow instance.
   */
  void destroy(AbstractChannelRow *channel_row);
};
