/**
 * @file effect.cc
 */
#include <iostream>
#include <iomanip>

#include "effect.hh"

Effect::Effect(int command, int value) : command(command), value(value) {}

Effect::~Effect() {}

int Effect::get_command() const { return command; }

void Effect::set_command(int command) { this->command = command; }

int Effect::get_value() const { return value; }

void Effect::set_value(int value) { this->value = value; }
