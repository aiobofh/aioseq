/**
 * @file part.cc
 *
 * Implementation of the Part class.
 *
 */
#include "pattern.hh"
#include "part.hh"

Part::Part(string& name, PatternsInterface* patterns) : PartTemplate(name, patterns) {}

Part::~Part() {}
