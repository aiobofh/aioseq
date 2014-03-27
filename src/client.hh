/**
 * @file client.hh
 */
#ifndef _CLIENT_HH_
#define _CLIENT_HH_

#include <string>

#include "pattern_client.hh"
#include "track_client.hh"
#include "part_client.hh"

/**
 * Pure virtual interface for client.
 */
class ClientInterface : public virtual PatternClientInterface,
                        public virtual TrackClientInterface,
                        public virtual PartClientInterface {
public:

  /**
   * Default constructor.
   */
  ClientInterface() {}

  /**
   * Default destructor.
   */
  virtual ~ClientInterface() {}

};

#endif
