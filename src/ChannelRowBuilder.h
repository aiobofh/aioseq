/**
 * @file ChannelRowBuilder.h
 *
 * Interface for a ChannelRow factory.
 */

#ifndef _CHANNEL_ROW_BUILDER_H_
#define _CHANNEL_ROW_BUILDER_H_

#include <cstddef>

#include "ChannelRow.h"
#include "AbstractChannelRow.h"

#include "AbstractChannelRowBuilder.h"

#include "AbstractParameterBuilder.h"
#include "AbstractNoteBuilder.h"


template<class CHANNEL_ROW_CLASS>
/**
 * @copydoc AbstractChannelRowBuilder
 */
class ChannelRowBuilder : AbstractChannelRowBuilder {

public:

  /**
   * @copydoc AbstractChannelRowBuilder::AbstractChannelRowBuilder(AbstractNoteBuilder *, AbstractParameterBuilder *)
   */
  ChannelRowBuilder(AbstractNoteBuilder* note_builder,
		    AbstractParameterBuilder* parameter_builder) {
    this->note_builder = note_builder;
    this->parameter_builder = parameter_builder;
  }


  /**
   * @copydoc AbstractChannelRowBuilder::create(int,int)
   */
  AbstractChannelRow *create(int number_of_notes, int number_of_parameters) {
    AbstractChannelRow *channel_row;

    list<AbstractNote*> *notes = new list<AbstractNote*>();
    list<AbstractParameter*> *parameters = new list<AbstractParameter*>();

    /**
     * Create the required number of note objects for this channel row.
     */
    for (int i = 0; i < number_of_notes; i++) {
      notes->push_back(note_builder->create());
    }

    /**
     * Conste the required number of parameter objects for this channel row.
     */
    for (int i = 0; i < number_of_parameters; i++) {
      parameters->push_back(parameter_builder->create());
    }

    /**
     * Construct the channel row object it-self and inject notes and
     * parameters.
     */
    channel_row = new CHANNEL_ROW_CLASS(notes, parameters);

    return channel_row;
  }


  /**
   * @copydoc AbstractChannelRowBuilder::destroy(AbstractChannelRow **);
   */
  void destroy(AbstractChannelRow **channel_row) {
    list<AbstractNote*> *notes = (*channel_row)->get_notes();
    list<AbstractParameter*> *parameters = (*channel_row)->get_parameters();

    /**
     * Destroy all the note objects on this channel row and remove them from
     * the list of notes.
     */
    while (!notes->empty()) {
      AbstractNote *note = notes->back();
      note_builder->destroy(&note);
      notes->pop_back();
    }

    /**
     * Destroy all the parameter objects on this channel row and remove them
     * from the list of parameters.
     */
    while (!parameters->empty()) {
      AbstractParameter *parameter = parameters->back();
      parameter_builder->destroy(&parameter);
      parameters->pop_back();
    }

    /**
     * Destruct the lists allocated in the create() method.
     */
    delete notes;
    delete parameters;

    /**
     * Destruct the channel row.
     */
    delete *channel_row;
    *channel_row = NULL;
  }

};

#endif
