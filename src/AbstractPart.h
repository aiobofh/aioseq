/**
 * AbstractPart.h
 *
 * Interface for part objects.
 */

#ifndef _ABSTRACT_PART_H_
#define _ABSTRACT_PART_H_

#include <string>
#include "AbstractPatterns.h" // Include in mock

using namespace std;

/**
 * Represent a part.
 */
class AbstractPart  {

protected:

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
   * @param name A pointer to the name of the part.
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
   * @param name A pointer to the name of the part.
   * @param patterns @copydoc AbstractPart::patterns
   */
  AbstractPart(const string *name, AbstractPatterns *patterns) {}; // Mock


  /**
   * Get the list of patterns in the song part.
   *
   * @return @copydoc AbstractPart::patterns
   */
  virtual AbstractPatterns *get_patterns() = 0;


  /**
   * Add a pattern to the part.
   *
   * @param pattern A pointer to a pattern object to add to the part.
   */
  virtual void add_pattern(AbstractPattern *pattern) = 0;


  /**
   * Determine if a pattern is used in a part.
   *
   * @param pattern A pointer to the patterh to search for.
   *
   * @return Boolean true if it is used, false if it is not used.
   */
  virtual bool pattern_is_used(AbstractPattern *pattern) = 0;


  /**
   * Insert a pattern in the part.
   *
   * @param before A pointer to a pattern in the part to insert the new
   *               pattern before.
   * @param pattern A pointer to a pattern object to insert in the part.
   */
  virtual void insert_pattern(AbstractPattern *before,
                              AbstractPattern *pattern) = 0;


  /**
   * Delelete a pattern from the part.
   *
   * @param pattern A pointer to the pattern object to delete from the part.
   */
  virtual void delete_pattern(AbstractPattern *pattern) = 0;

};

#endif
