/**
 * @file AbstractChannel.h
 *
 * Pure virtual class interface for representing a channel.
 */

#ifndef _ABSTRACT_CHANNEL_H_
#define _ABSTRACT_CHANNEL_H_

#include <string>
#include "AbstractHeadres.h"
#include "AbstractHeader.h"

/**
 * Represent a channel with note and parameter headers.
 */
class AbstractChannel : protected AbstractHeader {

protected:

  /**
   * A pointer to the list of pointers to note headers within a channel.
   */
  AbstractHeaders *note_headers;


  /**
   * A pointer to the list of pointers to parameter headers within the channel.
   */
  AbstractHeaders *parameter_headers;

public:

  /**
   * Dummy constructor.
   */
  AbstractChannel() {}; // Mock


  /**
   * Constructor taking a list of note headers and a list of parameer headers
   * as arguments.
   *
   * @param note_headers @copydoc AbstractChannel::note_headers
   * @param parameter_headers @copydoc AbstractChannel::parameter_headers
   */
  AbstractChannel(NoteHeaders *note_headers, ParameterHeaders *parameter_headers) {};


  /**
   * Constructor taking a list of note headers and a list of parameer headers
   * as arguments.
   *
   * @param name @copydoc AbstractChannel::name
   * @param note_headers @copydoc AbstractChannel::note_headers
   * @param parameter_headers @copydoc AbstractChannel::parameter_headers
   */
  AbstractChannel(const string *name, NoteHeaders *note_headers, ParameterHeaders *parameter_headers) {};


  /**
   * Get the list of note headers within a channel.
   *
   * @return @copydoc AbstractChannel::note_headers
   */
  virtual AbstractHeaders *get_note_headers() = 0;


  /**
   * Get the list of parameter headers within a channel.
   *
   * @return @copydoc AbstractChannel::parameter_headers
   */
  virtual AbstractHeaders *get_parameter_headers() = 0;


  /**
   * Add a note header to the channel.
   *
   * @param note_header A pointer to the note header object to add.
   */
  virtual void add_note_header(AbstractHeader *note_header) = 0;


  /**
   * Determine if a note header is used anywhere in the channel or not.
   *
   * @param note_header A pointer to the note header object to search for.
   *
   * @return Boolean true if it is used, false if it is not used.
   */
  virtual bool note_header_is_used(AbstractHeader *note_header) = 0;


  /**
   * Insert a note header in the channel.
   *
   * @param before A pointer to a note header object in the channel to insert
   *               the new note header before.
   * @param note_header A pointer to a note header object to insert in the channel.
   */
  virtual void insert_note_header(AbstractHeader *before, AbstractHeader *note_header) = 0;


  /**
   * Delete an existing note header from the channel.
   *
   * @param note_header A pointer to the note header object to delate.
   */
  virtual void delete_note_header(AbstractHeader *note_header) = 0;


  /**
   * Add a parameter header to the channel.
   *
   * @param parameter_header A pointer to the parameter header object to add to the channel.
   */
  virtual void add_parameter_header(AbstractHeader *parameter_header) = 0;


  /**
   * Determine if a parameter header is used anywhere in the channel or not.
   *
   * @param parameter_header A pointer to the parameter header object to
   *                         search for.
   *
   * @return Boolean rue if it is used, false if it is not used.
   */
  virtual bool parameter_header_is_used(AbstractHeader *parameter_header) = 0;


  /**
   * Insert a parameter header in the channel.
   *
   * @param before A pointer to a parameter header in the channel to insert the
   *               new parameter header before.
   * @param parameter_header A pointer to a parameter header oject to insert in the channel.
   */
  virtual void insert_parameter_header(AbstractHeader *before, AbstractHeader *parameter_header) = 0;


  /**
   * Delete an existing parameter header from the channel.
   *
   * @param parameter_header A pointer to the parameter object to delete.
   */
  virtual void delete_parameter_header(AbstractHeader *parameter_header);

};
