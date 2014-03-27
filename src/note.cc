/**
 * @file note.cc
 */

#include <iostream>
#include "note.hh"

using namespace std;

Note::Note(int key, int velocity) : key(key), velocity(velocity) {}

Note::~Note() {}

int Note::get_key() const { return key; }

void Note::set_key(int key) { this->key = key; }

int Note::get_velocity() const { return velocity; }

void Note::set_velocity(int note) { this->velocity = note; }
