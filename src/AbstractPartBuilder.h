/**
 * @file AbstractPartBuilder.h
 *
 * Interface for a part factory.
 */

#ifndef _ABSTRACT_PART_BUILDER_H_
#define _ABSTRACT_PART_BUILDER_H_

#include "AbstractPart.h"

// Mock: template<class PART_CLASS>
/**
 * Part object factory class.
 */
class AbstractPartBuilder {

public:

  /**
   * Dummy constructor.
   */
  AbstractPartBuilder() {};


  /**
   * Create a new part object.
   *
   * @param name Name of the song to create.
   *
   * @return A pointer to the new object.
   */
  virtual AbstractPart *create(string *name) = 0;


  /**
   * Destroy a part object.
   *
   * @param part Pointer to a pointer referencing the part object to destroy.
   */
  virtual void destroy(AbstractPart **part) = 0;

};

#endif
