/**
 * AbstractPart.h
 *
 * Interface for part objects.
 */

#ifndef _ABSTRACT_PART_H_
#define _ABSTRACT_PART_H_

#include <string>

using namespace std;

/**
 * Represent a part.
 */
class AbstractPart {

protected:

  /**
   * A pointer to a string containing the name of the part.
   */
  string *name;

public:

  /**
   * Dummy constructor.
   */
  AbstractPart() {}; // Mock

  /**
   * Constructor.
   *
   * @param name @copydoc AbstractPart::name
   */
  AbstractPart(string *name) {};

  /**
   * Set the name of a song.
   *
   * @param name @copydoc AbstractPart::name
   */
  virtual void set_name(string *name) = 0;

  /**
   * Get the name of a song part.
   *
   * @return @copydoc AbstractPart::name
   */
  virtual string *get_name() = 0;

};

#endif
