/**
 * @file mux.cc
 */
#include "sequencer.hh"
#include "client_primitive.hh"
#include "mux.hh"
#include "debug.hh"

Mux::Mux(SequencerInterface *sequencer) : sequencer(sequencer) {}

Mux::~Mux() {}

// -------------------------- SequencerInterface --------------------------

void Mux::register_client(ClientPrimitiveInterface *client) {
  bool found = false;
  for (unsigned int i = 0; i < clients.size(); i++) {
    ClientPrimitiveInterface* f = clients.at(i);
    if (f == client) {
      found = true;
      break;
    }
  }
  if (true == found) {
    error("Trying to register an already client that is registered in MUX.");
    return;
  }
  clients.push_back(client);
}

void Mux::unregister_client(ClientPrimitiveInterface *client) {
  bool found = false;
  unsigned int i;
  for (i = 0; i < clients.size(); i++) {
    ClientPrimitiveInterface *f = clients.at(i);
    if (f == client) {
      found = true;
      break;
    }
  }
  if (false == found) {
    error("Trying to delete a client that is not registered in MUX.");
    return;
  }
  clients.erase(clients.begin() + i);
}

PatternRowInterface* Mux::get_row() {
  return sequencer->get_row();
}

PatternRowInterface* Mux::get_row(int row_index) {
  return sequencer->get_row(row_index);
}

TracksInterface* Mux::get_tracks() {
  return sequencer->get_tracks();
}

// ------------------------ TrackClientInterface --------------------------

void Mux::set_track_index(unsigned int track_index) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    TrackClientInterface* track_client =
      dynamic_cast<TrackClientInterface*>(clients.at(i));
    if (track_client) {
      debug("Calling set_track_index(" << track_index << ") in client[" << i
            << "].");
      track_client->set_track_index(track_index);
    }
  }
}

// ------------------------ PartClientInterface ---------------------------

void Mux::set_pattern_index(unsigned int pattern_index) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    PartClientInterface* part_client =
      dynamic_cast<PartClientInterface*>(clients.at(i));
    if (part_client) {
      debug("Calling set_pattern_index(" << pattern_index << ") in client [" <<
            i << "}.");
      part_client->set_pattern_index(pattern_index);
    }
  }
}

// ----------------------- PatternClientInterface -------------------------

void Mux::set_pattern_row_index(int pattern_row_index) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    PatternClientInterface* pattern_client =
      dynamic_cast<PatternClientInterface*>(clients.at(i));
    if (pattern_client) {
      debug("Calling set_row_index(" << pattern_row_index << ") in client["
            << i << "].");
      pattern_client->set_pattern_row_index(pattern_row_index);
    }
  }
}

void Mux::set_pattern_length(unsigned int pattern_length) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    PatternClientInterface* pattern_client =
      dynamic_cast<PatternClientInterface*>(clients.at(i));
    if (pattern_client) {
      debug("Calling set_pattern_length(" << pattern_length << ") in client["
            << i << "].");
      pattern_client->set_pattern_length(pattern_length);
    }
  }
}

void Mux::set_key(unsigned int track_index, unsigned int note_index, int key) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    PatternClientInterface* pattern_client =
      dynamic_cast<PatternClientInterface*>(clients.at(i));
    if (pattern_client) {
      debug("Calling set_key(" << track_index << ", " <<
            note_index << ", " << key << ") in client["
            << i << "].");
      pattern_client->set_key(track_index, note_index, key);
    }
  }
}
