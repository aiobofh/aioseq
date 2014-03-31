/**
 * @file pattern_client.hh
 */
#ifndef _PATTERN_CLIENT_HH_
#define _PATTERN_CLIENT_HH_

#include "client_primitive.hh"

/**
 * Pure virtual interface class for a pattern client.
 */
class PatternClientInterface : public virtual ClientPrimitiveInterface {

public:

  /**
   * Default constructor.
   */
  PatternClientInterface() {}

  /**
   * Default destructor.
   */
  virtual ~PatternClientInterface() {}

  /**
   * Set the current pattern row index to the spcified one.
   *
   * @param pattern_row_index The index to set.
   */
  virtual void set_pattern_row_index(int pattern_row_index) = 0;

  /**
   * Set the current pattern length.
   *
   * @param pattern_length Number of lines to set the pattern to.
   */
  virtual void set_pattern_length(unsigned int pattern_length) = 0;

};

#endif
