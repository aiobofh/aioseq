/**
 * @file part.hh
 *
 * All pert related class declarations.
 *
 * @copydetails PartInterface
 *
 */
#ifndef _PART_HH_
#define _PART_HH_

#ifndef PART_FRIENDS
/// No part friends are available
#define PART_FRIENDS
#endif

#include <string>

#include "pattern.hh"

#include "vector_of.hh"

using namespace std;


/**
 * Pure virtual interface class for a part.
 *
 * A part contains a list of patterns and a name such as "intro", "chorus" or
 * similar to group the patterns into a context.
 */
class PartInterface {

public:

  /**
   * Default constructor.
   */
  PartInterface() {};

  /**
   * Constructor taking a name and a reference to a list of patterns for the
   * part.
   *
   * @param name Name of the part.
   * @param patterns A reference to a list of patterns.
   */
  PartInterface(string& name, PatternsInterface* patterns) {};

  /**
   * Default destructor.
   */
  virtual ~PartInterface() {};

  /**
   * Get the name of this part.
   *
   * @return A string containing the name of the part.
   */
  virtual string const& get_name() const = 0;

  /**
   * Set the name of this part.
   *
   * @param name A string containing the name of the part.
   */
  virtual void set_name(string& name) = 0;

  /**
   * Get the reference to the list of patterns that make up the part.
   *
   * @return A reference to the list of patterns that make up the part.
   */
  virtual PatternsInterface *get_patterns() const = 0;

};


/**
 * Template class with optimizable handling of a part with patterns.
 *
 * @copydetails PartInterface
 */
class PartTemplate : public virtual PartInterface {

  PART_FRIENDS

protected:

  /// Internal storage of the name of the part.
  string name;
  /// Internal storage of the pattern list for the part.
  PatternsInterface* patterns;

public:

  /// @copydoc PartInterface::PartInterface(string&,PatternsInterface*)
  PartTemplate(string& name, PatternsInterface* patterns) :
    name(name), patterns(patterns) {};
  /// @copydoc PartInterface::~PartInterface()
  virtual ~PartTemplate() {}
  /// @copydoc PartInterface::get_name()
  string const& get_name() const { return name; }
  /// @copydoc PartInterface::set_name(string&)
  void set_name(string& name) { this->name = name; }
  /// @copydoc PartInterface::get_patterns()
  PatternsInterface* get_patterns() const { return patterns; }

};


/**
 * Part is a specialized class of the template PartTemplate for performance
 * and reuse.
 *
 * @copydetails PartInterface
 *
 */
class Part : public PartTemplate {

public:

  /// @copydoc PartInterface::PartInterface(string&,PatternsInterface*)
  Part(string& name, PatternsInterface* patterns);
  /// @copydoc PartInterface::~PartInterface()
  ~Part();

};


/**
 * Create a specialized vector to reference Part objects.
 */
VECTOR_OF(Part, Parts, 256);


/**
 * Factory class to create part objects.
 */
template<class PART,
         class PATTERNS>
class PartFactoryTemplate {

public:

  /**
   * Create a new instance of a part class with the provided name,
   * and the provided list of patterns, or default.
   *
   * @param project_patterns A reference to the list of all patterns
   *                         available, the first entry will be used as
   *                         default for the one entry created if the patterns
   *                         parameters is not provided.
   * @param name             The name of the part, if none provided the part
   *                         will be named "New part".
   * @param patterns         A reference to a list of patterns that make up
   *                         the part, if none provided a list with one item
   *                         referencing the first pattern in the
   *                         project_patterns list will be created.
   *
   * @return A reference to the new part.
   */
  static PART* New(PATTERNS* project_patterns,
                   string name = "New part",
                   PATTERNS* patterns = new PATTERNS()) {
    if ((0 == patterns->size()) && (0 != project_patterns->size())) {
      patterns->push_back(project_patterns->front());
    }
    PART* part = new PART(name, patterns);
    return part;
  }

  /**
   * Destroy a part class instance and everything related to it, setting the
   * reference to NULL when done.
   *
   * @param part A pointer to a reference to a part class instance to destroy,
   */
  static void Delete(PART** part) {
    PATTERNS* patterns = dynamic_cast<PATTERNS*>((*part)->get_patterns());

    while ((NULL != patterns) && (0 != patterns->size())) {
      patterns->pop_back();
    }

    if (NULL != patterns) {
      delete patterns;
    }

    delete (*part);
    *part = NULL;
  }

};


/**
 * Part factory to create parts with concrete implementations of the part
 * itself and the pattern list.
 */
typedef PartFactoryTemplate<Part,
                            Patterns> PartFactory;

#endif
