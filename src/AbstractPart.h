/**
 * AbstractPart.h
 *
 * Interface for part objects.
 */

#ifndef _ABSTRACT_PART_H_
#define _ABSTRACT_PART_H_

#include <string>
#include "AbstractPatterns.h" // Mock

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


  /**
   * A pointer to a list of pointers to patterns building the part.
   */
  AbstractPatterns *patterns;

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
  AbstractPart(const string *name) {}; // Mock


  /**
   * Constructor.
   *
   * @param patterns @copydoc AbstractPart::patterns
   */
  AbstractPart(AbstractPatterns *patterns) {}; // Mock


  /**
   * Constructor.
   *
   * @param name @copydoc AbstractPart::name
   * @param patterns @copydoc AbstractPart::patterns
   */
  AbstractPart(const string *name, AbstractPatterns *patterns) {}; // Mock


  /**
   * Set the name of a song.
   *
   * @param name @copydoc AbstractPart::name
   */
  virtual void set_name(const string *name) = 0;


  /**
   * Get the name of a song part.
   *
   * @return @copydoc AbstractPart::name
   */
  virtual string *get_name() = 0;


  /**
   * Get the list of patterns in the song part.
   *
   * @return @copydoc AbstractPart::patterns
   */
  virtual AbstractPatterns *get_patterns() = 0;

};

#endif
