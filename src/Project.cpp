/**
 * @file Project.cpp
 *
 * Project implementation.
 */

#include "Project.h"

#include "AbstractPattern.h"
#include "PatternBuilder.h"

Project::Project() {
  channels = 0;
  default_pattern_length = 16;
}

AbstractPattern *Project::add_pattern() {
}
