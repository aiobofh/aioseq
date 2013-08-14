/**
 * @file Channel.cpp
 *
 * Channel implementation.
 *
 */

#include "AbstractHeaders.h"
#include "AbstractHeader.h"
#include "Channel.h"
#include "Name.h"

Channel::Channel(AbstractHeaders *note_headers, AbstractHeaders *parameter_headers) : Header((const string*)DEFAULT_CHANNEL_NAME, false, false) {
  this->note_headers = note_headers;
  this->parameter_headers = parameter_headers;
}


Channel::Channel(const string *name, AbstractHeaders *note_headers, AbstractHeaders *parameter_headers) : Header(name, false, false) {
  this->note_headers = note_headers;
  this->parameter_headers = parameter_headers;
}

AbstractHeaders *Channel::get_note_headers() {
  return note_headers;
}

AbstractHeaders *Channel::get_parameter_headers() {
  return parameter_headers;
}

void Channel::add_note_header(AbstractHeader *note_header) {
  note_headers->push_back(note_header);
}

bool Channel::note_header_is_used(AbstractHeader *note_header) {
  /**
   * @todo Traverse through all patterns to find out if any data is
   *       stored on this channel and note position in any of the patterns.
   */
  return false;
}

void Channel::insert_note_header(AbstractHeader *before, AbstractHeader *note_header) {
  AbstractHeaders::iterator i;
  for (i = this->note_headers->begin(); i != this->note_headers->end(); ++i) {
    if (*i == before) {
      this->note_headers->insert(i, note_header);
      return;
    }
  }
  add_note_header(note_header);
}

void Channel::delete_note_header(AbstractHeader *note_header) {
  note_headers->remove(note_header);
}

void Channel::add_parameter_header(AbstractHeader *parameter_header) {
  parameter_headers->push_back(parameter_header);
}

bool Channel::parameter_header_is_used(AbstractHeader *parameter_header) {
  /**
   * @todo Traverse through all patterns to find out if any data is
   *       stored on this channel and parameter position in any of the patterns.
   */
  return false;
}

void Channel::insert_parameter_header(AbstractHeader *before, AbstractHeader *parameter_header) {
  AbstractHeaders::iterator i;
  for (i = this->parameter_headers->begin(); i != this->parameter_headers->end(); ++i) {
    if (*i == before) {
      this->parameter_headers->insert(i, parameter_header);
      return;
    }
  }
  add_parameter_header(parameter_header);
}

void Channel::delete_parameter_header(AbstractHeader *parameter_header) {
  parameter_headers->remove(parameter_header);
}

