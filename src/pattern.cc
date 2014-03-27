/**
 * @file pattern.cc
 */
#include <iostream>

#include "pattern.hh"
#include "pattern_row.hh"

Pattern::Pattern(PatternRowsInterface* rows) : PatternTemplate<PatternRows>(rows) {}

Pattern::~Pattern() {}
