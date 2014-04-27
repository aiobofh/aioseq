/**
 * @file track_client.hh
 */
#ifndef _TRACK_CLIENT_HH_
#define _TRACK_CLIENT_HH_

#include "client_primitive.hh"

/**
 * Pure virtual interface class for a track client.
 */
class TrackClientInterface : public virtual ClientPrimitiveInterface {

public:

  /**
   * Default constructor.
   */
  TrackClientInterface() {}

  /**
   * Default destructor.
   */
  virtual ~TrackClientInterface() {}

  /**
   * Set the current track index to the specified one.
   *
   * @param track_index The index to set.
   */
  virtual void set_track_index(unsigned int track_index) = 0;

};

#endif
