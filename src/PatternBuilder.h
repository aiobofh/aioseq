/**
 * @file PatternBuilder.h
 *
 * @copydoc AbstractPatternBuilder.h
 *
 */

#ifndef _PATTERN_BUILDER_H_
#define _PATTERN_BUILDER_H_

#include <cstddef>

#include "AbstractPatternBuilder.h"

#include "AbstractPattern.h"
#include "AbstractPatternRowBuilder.h"

template<class PATTERN_CLASS>
/**
 * @copydoc AbstractPatternBuilder
 */
class PatternBuilder : AbstractPatternBuilder {

public:

  /**
   * @copydoc AbstractPatternBuilder::AbstractPatternBuilder(AbstractPatternRowBuilder*)
   */
  PatternBuilder(AbstractPatternRowBuilder *pattern_row_builder) {
    this->pattern_row_builder;
  }

  /**
   * @copydoc AbstractPatternBuilder::AbstractPatternBuilder(list<AbstractPatternRow*>*)
   */
  AbstractPattern *create(int number_of_channel_rows,
			  int notes_in_channels[],
			  int parameters_in_channels[],
			  int number_of_pattern_rows) {

    list<AbstractPatternRow*> *pattern_rows = new list<AbstractPatternRow*>;

    for (int i = 0; i < number_of_pattern_rows; i++) {
      pattern_rows->push_back(pattern_row_builder->create(number_of_channel_rows,
							  notes_in_channels,
							  parameters_in_channels));
    }

    AbstractPattern *pattern = new PATTERN_CLASS(pattern_rows);

    return pattern;
  }

  /**
   * @copydoc AbstractPatternBuilder::destroy
   */
  void destroy(AbstractPattern **pattern) {
    list<AbstractPatternRow*> *pattern_rows = (*pattern)->get_pattern_rows();

    /**
     * Destroy all the pattern row objects in this pattern and remove them
     * from the list of pattern rows.
     */
    while (!pattern_rows->empty()) {
      AbstractPatternRow *pattern_row = pattern_rows->back();
      pattern_row_builder->destroy(&pattern_row);
      pattern_rows->pop_back();
    }

    /**
     * Destruct the list allocated in the create() method.
     */
    delete pattern_rows;

    /**
     * Destruct the pattern row.
     */
    delete(*pattern);
    *pattern = NULL;
  }

};

#endif
