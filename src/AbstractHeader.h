/**
 * @file AbstractHeader.h
 *
 * Pure virtual class interface for representing a header.
 */

#ifndef _ABSTRACT_HEADER_H_
#define _ABSTRACT_HEADER_H_

#include "AbstractName.h"

/**
 * Represent a header with a mute and a solo control.
 */
class AbstractHeader {

public:

  /**
   * Dummy constructor.
   */
  AbstractHeader() {}; // Mock


  /**
   * Constructor taking the name of the header as argument.
   *
   * @param name Name of the header.
   * @param mute Boolean value to indicate that this header is muted.
   * @param solo Boolean value to indicate that this header is solo.
   */
  AbstractHeader(const string *name, bool mute, bool solo) {}; // Mock


  /**
   * Mute the header.
   */
  virtual void mute() = 0;


  /**
   * Un-mute the header.
   */
  virtual void unmute() = 0;


  /**
   * Solo the header.
   */
  virtual void solo() = 0;


  /**
   * Un-solo the header.
   */
  virtual void unsolo() = 0;

};


#endif /* _ABSTRACT_HEADER_H_ */
