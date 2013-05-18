/**
 * @file ChannelRowBuilder.h
 *
 * Interface for a ChannelRow factory.
 */

#ifndef _CHANNEL_ROW_BUILDER_H_
#define _CHANNEL_ROW_BUILDER_H_

#include "AbstractChannelRow.h"

#include "NoteBuilder.h"
#include "ParameterBuilder.h"

template<class CHANNEL_ROW_CLASS, class NOTE_CLASS, class PARAMETER_CLASS>
/**
 * ChannelRow object facatory class.
 */
class ChannelRowBuilder {
private:
  NoteBuilder<NOTE_CLASS> note_builder;
  ParameterBuilder<PARAMETER_CLASS> parameter_builder;
public:
  /**
   * Create a ChannelRow with the specified number of notes and parameters.
   *
   * @todo Replace the integers for number of notes and number of parameters
   *       with a references to a channel from the Project Channel list when
   *       that is implemented, and get the number of notes and parameters
   *       from there.
   *
   * @param number_of_notes The number of Note instances to create.
   * @param number_of_parameters The number of Parameter instances to create.
   *
   * @return A pointer to the new ChannelRow.
   */
  CHANNEL_ROW_CLASS *create(int number_of_notes, int number_of_parameters) {
    CHANNEL_ROW_CLASS *channel_row;
    list<NOTE_CLASS*> *notes = new list<NOTE_CLASS*>();
    list<PARAMETER_CLASS*> *parameters = new list<PARAMETER_CLASS*>();

    for (int i = 0; i < number_of_notes; i++) {
      notes->push_back(note_builder.create());
    }

    for (int i = 0; i < number_of_parameters; i++) {
      parameters->push_back(parameter_builder.create());
    }

    channel_row = new CHANNEL_ROW_CLASS(notes, parameters);

    return channel_row;
  }

  /**
   * Destroy a ChannelRow instance and cleanup everything constructed by
   * the create method.
   *
   * @param channel_row A pointer to a ChannelRow instance.
   */
  void destroy(CHANNEL_ROW_CLASS *channel_row) {
    list<NOTE_CLASS*> *notes;
    list<PARAMETER_CLASS*> *parameters;
    delete channel_row;
  }
};

#endif
