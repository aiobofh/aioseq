/**
 * @file PartBuilder.h
 *
 * Interface for a Part factory. Unfortunatly this file also contains the
 * class implementation, since it is a template class.
 */

#ifndef _PART_BUILDER_H_
#define _PART_BUILDER_H_

/**
 * AbstractPatterns is used as a pointer type containing a list of patterns in
 * the part.
 */
#include "AbstractPatterns.h"

/**
 * AbstractPart is used as a pointer returned when creating and for destroying
 * part objects.
 */
#include "AbstractPart.h"

/**
 * PartBuilder inherits from this class interface.
 */
#include "AbstractPartBuilder.h"

#include <cstddef>

template<class PART_CLASS>

/**
 * @copydoc AbstractPartBuilder
 */
class PartBuilder : AbstractPartBuilder {

public:

  /**
   * @copydoc AbstractPartBuilder::create(string *)
   */
  AbstractPart *create(string *name) {
    AbstractPatterns *patterns = new AbstractPatterns();
    return new PART_CLASS(name, patterns);
  }


  /**
   * @copydoc AbstractPartBuilder::destroy(AbstractPart **)
   */
  void destroy(AbstractPart **part) {
    AbstractPatterns *patterns = (*part)->get_patterns();
    delete patterns;
    delete *part;
    *part = NULL;
  }

};

#endif
