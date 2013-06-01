/**
 * @file ChannelRowBuilder.h
 *
 * Interface for a ChannelRow factory. Unfortunatly this file also contains the
 * class implementation, since it is a template class.
 */

#ifndef _CHANNEL_ROW_BUILDER_H_
#define _CHANNEL_ROW_BUILDER_H_

/**
 * AbstractNote is used as pointer type for destroying note objects.
 */
#include "AbstractNote.h"

/**
 * AbstractNotes list type is used for allocating, and iterrating through the
 * list of pointers to note objects withing a channel row.
 */
#include "AbstractNotes.h"

/**
 * AbstractParameter is used as pointer type for destroying parameter objects.
 */
#include "AbstractParameter.h"

/**
 * AbstractParameters list type is used for allocating, and iterrating through
 * the list of pointers to note objects withing a channel row.
 */
#include "AbstractParameters.h"

/**
 * AbsrtactChannelRow is used as interface for pointers returned from the
 * create method, and as argument to the destroy method.
 */
#include "AbstractChannelRow.h"

/**
 * AbsractParameterBuilder interface is passed a pointer to an object factory
 * to create parameter objects needed by the ChannelRowBuilder object factory.
 */
#include "AbstractParameterBuilder.h"

/**
 * AbsractNoteBuilder interface is passed a pointer to an object factory
 * to create parameter objects needed by the ChannelRowBuilder object factory.
 */
#include "AbstractNoteBuilder.h"

/**
 * ChannelRowBuilder inerhits from this class interface.
 */
#include "AbstractChannelRowBuilder.h"

#include <cstddef>

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

    AbstractNotes *notes = new AbstractNotes();
    AbstractParameters *parameters = new AbstractParameters();

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
    AbstractNotes *notes = (*channel_row)->get_notes();
    AbstractParameters *parameters = (*channel_row)->get_parameters();

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
