/**
 * @file part_client.hh
 */
#ifndef _PART_CLIENT_HH_
#define _PART_CLIENT_HH_

#include "client_primitive.hh"

/**
 * Pure virtual interface for part clients.
 */
class PartClientInterface : public virtual ClientPrimitiveInterface {

public:

  /**
   * Default constructor.
   */
  PartClientInterface() {}

  /**
   * Default destructor.
   */
  virtual ~PartClientInterface() {}

  /**
   * Set the current pattern index to the specified one.
   *
   * @param pattern_index Index to set.
   */
  virtual void set_pattern_index(int pattern_index) = 0;

};

#endif
