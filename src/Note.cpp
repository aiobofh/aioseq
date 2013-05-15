/**
 * @file Note.cpp
 *
 * Note implementation.
 *
 * A Note is a simple entity that can contain a note-number and a velocity
 * at which the note was hit on a keyboard.
 */

#include "Note.h"

Note::Note() {
  this->note = this->velocity = 0;
}

Note::Note(int note, int velocity) {
  this->note = note;
  this->velocity = velocity;
}

void Note::set_note(int note)  {
  this->note = note;
}

int Note::get_note() {
  return this->note;
}

void Note::set_velocity(int velocity) {
  this->velocity = velocity;
}

int Note::get_velocity() {
  return this->velocity;
}
