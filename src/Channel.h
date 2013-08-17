/**
 * @file Channel.h
 *
 * The interface declaration for the Channel class.
 *
 * @copydetails AbstractChannel.h
 *
 */

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <gtest/gtest_prod.h>

#include "AbstractChannel.h"
#include "AbstractHeaders.h"

#include "Name.h"
#include "Header.h"

/**
 * The default channel name used if nothing is provided.
 */
#define DEFAULT_CHANNEL_NAME "New channel"

/**
 * Represent a channel according to the constraints of AbstractChannel.
 */
class Channel : public AbstractChannel, public Header {

  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(ChannelTest, Constructor_with_note_and_parameter_headers_shall_construct_a_channel_with_default_name_and_note_and_parameter_headers);
  /**
   * Make the test-case classes friends with this implementation.
   */
  FRIEND_TEST(Channel, Constructor_with_name_note_and_parameter_headers_shall_construct_a_channel_with_name_and_note_and_parameter_headers);

private:

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
   * @copydoc AbstractChannel::AbstractChannel(AbstractHeaders *, AbstractHeaders *)
   */
  Channel(AbstractHeaders *note_headers, AbstractHeaders *parameter_headers);


  /**
   * @copydoc AbstractChannel::AbstractChannel(const string *, AbstractHeaders *, AbstractHeaders *)
   */
  Channel(const string *name, AbstractHeaders *note_headers, AbstractHeaders *parameter_headers);


  /**
   * @copydoc AbstractChannel::get_note_headers
   */
  AbstractHeaders *get_note_headers();


  /**
   * @copydoc AbstractChannel::get_parameter_headers
   */
  AbstractHeaders *get_parameter_headers();


  /**
   * @copydoc AbstractChannel::add_note_header
   */
  void add_note_header(AbstractHeader *note_header);


  /**
   * @copydoc AbstractChannel::note_header_is_used
   */
  bool note_header_is_used(AbstractHeader *note_header);


  /**
   * @copydoc AbstractChannel::insert_note_header
   */
  void insert_note_header(AbstractHeader *before, AbstractHeader *note_header);


  /**
   * @copydoc AbstractChannel::delete_note_header
   */
  void delete_note_header(AbstractHeader *note_header);


  /**
   * @copydoc AbstractChannel::add_parameter_header
   */
  void add_parameter_header(AbstractHeader *parameter_header);


  /**
   * @copydoc AbstractChannel::parameter_header_is_used
   */
  bool parameter_header_is_used(AbstractHeader *parameter_header);


  /**
   * @copydoc AbstractChannel::insert_parameter_header
   */
  void insert_parameter_header(AbstractHeader *before, AbstractHeader *parameter_header);


  /**
   * @copydoc AbstractChannel::delete_parameter_header
   */
  void delete_parameter_header(AbstractHeader *parameter_header);

};

#endif
