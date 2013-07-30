/**
 * @file AbstractHeader.h
 *
 * Pure virtual class interface for representing a header.
 */

#ifndef _ABSTRACT_HEADER_H_
#define _ABSTRACT_HEADER_H_

/**
 * Represent a header with a mute and a solo control.
 */
class AbstractHeader : protected AbstractName {

protected:

  /**
   * Boolean value to indicate that this header is solo.
   */
  bool solo;


  /**
   * Boolean value to indicate that this header is muted.
   */
  bool mute;

public:

  /**
   * Dummy constructor.
   */
  AbstractHeader() {}; // Mock


  /**
   * Constructor taking the name of the header as argument.
   *
   * @param name @copydoc AbstractName::name
   * @param mute @copydoc AbstractHeader::mute
   * @param solo @copydoc AbstractHeader::solo
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
