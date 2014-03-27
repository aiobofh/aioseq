/**
 * @file pattern.hh
 *
 * All pattern retlated class declarations.
 *
 */
#ifndef _PATTERN_HH_
#define _PATTERN_HH_

#ifndef PATTERN_FRIENDS
/// No pattern friends if none are available.
#define PATTERN_FRIENDS
#endif

#include "pattern_row.hh"

#ifndef PATTERN_ROW_FACTORY_NEW
/// By default call the actual pattern row factory method
#define PATTERN_ROW_FACTORY_NEW PATTERN_ROW_FACTORY::New
#endif

#ifndef PATTERN_ROW_FACTORY_DELETE
/// By default call the actual pattern row factory method
#define PATTERN_ROW_FACTORY_DELETE PATTERN_ROW_FACTORY::Delete
#endif

#include "vector_of.hh"

/**
 * Pure virtual interface class for a pattern.
 *
 * @todo Implement a name string for the pattern class.
 */
class PatternInterface {

public:

  /**
   * Default constructor.
   */
  PatternInterface() {};

  /**
   * Constructor taking a reference to the list of pattern rows to assign to
   * this pattern.
   *
   * @param pattern_rows A reference to the list of pattern rows to assign to
   *                     this pattern.
   */
  PatternInterface(PatternRowsInterface* pattern_rows) {};

  /**
   * Default destructor.
   */
  virtual ~PatternInterface() {};

  /**
   * Get the reference to the list of pattern rows in this pattern.
   *
   * @return A reference to the list of pattern rows in this pattern.
   */
  virtual PatternRowsInterface* get_pattern_rows() = 0;

};


/**
 * Template class with optimizalbe handling of a pattern with pattern rows.
 *
 * @todo The Pattern does not have to be a template, since no optimizations
 *       are needed.
 */
template<class PATTERN_ROWS>
class PatternTemplate : virtual public PatternInterface {

  PATTERN_FRIENDS

protected:

  /**
   * Internal storage of the reference to the list of pattern rows in the
   * pattern.
   */
  PatternRowsInterface* pattern_rows;

public:

  /// @copydoc PatternInterface::PatternInterface(PatternRowsInterface*)
  PatternTemplate(PatternRowsInterface* pattern_rows) :
  pattern_rows(pattern_rows) {}
  /// @copydoc PatternInterface::~PatternInterface()
  virtual ~PatternTemplate() {};
  /// @copydoc PatternInterface::get_pattern_rows()
  PatternRowsInterface* get_pattern_rows() { return pattern_rows; };

};


/**
 * Pattern is a specialized class of the template PatternTemplate for
 * performance and reuse.
 */
class Pattern : public PatternTemplate<PatternRows> {

public:

  /// @copydoc PatternInterface::PatternInterface(PatternRowsInterface*)
  Pattern(PatternRowsInterface* pattern_rows);
  /// @copydoc PatternInterface::~PatternInterface()
  ~Pattern();

};


/**
 * Create a specialized vector to reference Pattern objects.
 */
VECTOR_OF(Pattern, Patterns, 256);


/**
 * Factory class to create pattern objects.
 */
template<class PATTERN,
         class TRACK,
         class TRACKS,
         class PATTERN_ROW,
         class PATTERN_ROWS,
         class PATTERN_ROW_FACTORY>
class PatternFactoryTemplate {

public:

  /**
   * Create a new instance of a pattern class with the configuration of the
   * referenced tracks parameter and the number of rows to set the correclty
   * configured pattern rows, or default.
   *
   * @param tracks        A reference to a list of tracks providing the
   *                      configuration of each pattern_row, if not provided.
   * @param rows          The number of pattern rows to create if no
   *                      pattern_rows list reference is provided.
   * @param pattern_rows  A reference to a list of patterns rows which have
   *                      to have the exact same properties as the
   *                      corresponding tracks list reference and rows numer.
   *                      If this parameter is not provided the patter rows
   *                      will be allocated and configrued with default
   *                      settings.
   *
   * @return A reference to the new pattern.
   */
  static PATTERN* New(TRACKS *tracks,
                      unsigned int rows = 64,
                      PATTERN_ROWS* pattern_rows = new PATTERN_ROWS()) {
    if ((0 != pattern_rows->size()) &&
        (rows != pattern_rows->size())) {
      error("The number of pattern rows provided does not match the number "
            << "of pattern rows configruationprovided to this factory.");
      return NULL;
    }

    for (unsigned int i = 0; i < rows; i++) {
      // Eliminate compiler warnings if macro is not using the argument.
      tracks = tracks;
      PATTERN_ROW* pattern_row = PATTERN_ROW_FACTORY_NEW(tracks);
      pattern_rows->push_back(pattern_row);
    }

    // Construct the pattern
    PATTERN *pattern = new PATTERN(pattern_rows);
    return pattern;
  }

  /**
   * Add rows to an existing pattern using the configuration of the tracks
   * to allocate everything needed for a number of rows.
   *
   * @param tracks              A reference to a list of references to
   *                            track instances.
   * @param pattern             The pattern to add rows to.
   * @param pattern_rows_to_add The number of rows to add to the pattern.
   */
  static void AddRows(TRACKS* tracks, PATTERN* pattern,
                      unsigned int pattern_rows_to_add) {
    PATTERN_ROWS* pattern_rows =
      dynamic_cast<PATTERN_ROWS*>(pattern->get_pattern_rows());
    while (pattern_rows_to_add-- > 0) {
      PATTERN_ROW* pattern_row = PATTERN_ROW_FACTORY_NEW(tracks);
      pattern_rows->push_back(pattern_row);
    }
  }

  /**
   * Remove rows from an existing pattern.
   *
   * @param pattern                The pattern to remove rows from.
   * @param pattern_rows_to_remove The number of rows to remove from the end
   *                               of the pattern.
   */
  static void RemoveRows(PATTERN* pattern,
                         unsigned int pattern_rows_to_remove) {
    PATTERN_ROWS* pattern_rows =
      dynamic_cast<PATTERN_ROWS*>(pattern->get_pattern_rows());
    while (pattern_rows_to_remove-- > 0) {
      PATTERN_ROW* pattern_row = pattern_rows->back();
      PATTERN_ROW_FACTORY_DELETE(&pattern_row);
      pattern_rows->pop_back();
    }
  }

  /**
   * Destroy a pattern instance, destroying all the pattern rows and set the
   * reference to NULL when done.
   *
   * @param pattern A pointer to a reference to a pattern class instance to
   *                destroy.
   */
  static void Delete(PATTERN** pattern) {
    PATTERN_ROWS* pattern_rows =
      dynamic_cast<PATTERN_ROWS*>((*pattern)->get_pattern_rows());

    while ((NULL != pattern_rows) && (0 != pattern_rows->size())) {
      PATTERN_ROW* pattern_row =
        dynamic_cast<PATTERN_ROW*>(pattern_rows->back());
      PATTERN_ROW_FACTORY_DELETE(&pattern_row);
      pattern_rows->pop_back();
    }

    if (NULL != pattern_rows) {
      delete pattern_rows;
    }

    delete (*pattern);
    *pattern = NULL;
  }

};


/**
 * Pattern factory to create pattern rows with a concrete implementation
 * of the pattern itself and the pattern row class.
 */
typedef PatternFactoryTemplate<Pattern,
                               Track,
                               Tracks,
                               PatternRow,
                               PatternRows,
                               PatternRowFactory> PatternFactory;

#endif
