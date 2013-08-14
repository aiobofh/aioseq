/**
 * @file Header.cpp
 *
 * Header implementation.
 */

#include "Header.h"
#include "Name.h"

Header::Header() : Name(DEFAULT_HEADER_NAME) {
  soloed = muted = false;
}

Header::Header(const string *name, bool mute, bool solo) : Name(name) {
  soloed = solo;
  muted = mute;
}

void Header::mute() {
  muted = true;
}

void Header::unmute() {
  muted = false;
}

void Header::solo() {
  soloed = true;
}

void Header::unsolo() {
  soloed = false;
}
