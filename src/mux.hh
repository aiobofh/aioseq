/**
 * @file mux.hh
 */
#ifndef _CLIENT_MUX_HH_
#define _CLIENT_MUX_HH_

#include <vector>

#include "client_primitive.hh"
#include "client.hh"
#include "sequencer.hh"

#include "error.hh"

#ifndef MUX_FRIENDS
#define MUX_FRIENDS
#endif

using namespace std;

/**
 * Pure virtual interface for a client mux.
 */
class MuxInterface : public virtual SequencerInterface {

public:

  /**
   * Default constructor.
   */
  MuxInterface() {}

  /**
   * Constructor taking a sequencer as argument with which to register as
   * client and communicate with.
   *
   * @param sequencer A reference to a sequencer which to to register as client
   */
  MuxInterface(SequencerInterface* sequencer) {}

  /**
   * Default destructor.
   */
  virtual ~MuxInterface() {}

};


/**
 * The Mux class is a client multiplexer that sends relevant information to the
 * clientes registered.
 */
class Mux : public virtual SequencerInterface {

  MUX_FRIENDS

protected:

  /// Convenience type to keep a list of registered clients.
  typedef vector<ClientPrimitiveInterface*> Clients;

  /// Internal storage of a list of references to clients.
  Clients clients;
  /// Internal storage of a reference to a sequencer.
  SequencerInterface *sequencer;

public:

  /// @copydoc MuxInterface::MuxInterface(SequencerInterface*)
  Mux(SequencerInterface* sequencer);
  /// @copydoc MuxInterface::~MuxInterface()
  virtual ~Mux();

  // -------------------------- SequencerInterface --------------------------

  /// @copydoc SequencerInterface::register_client(ClientPrimitiveInterface*)
  void register_client(ClientPrimitiveInterface* client);
  /// @copydoc SequencerInterface::unregister_client(ClientPrimitiveInterface*)
  void unregister_client(ClientPrimitiveInterface* client);
  /// @copydoc SequencerInterface::get_row()
  PatternRowInterface *get_row();
  /// @copydoc SequencerInterface::get_row(int)
  PatternRowInterface *get_row(int pattern_row_index);

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(unsigned int)
  void set_track_index(unsigned int track_index);

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(unsigned int)
  void set_pattern_index(unsigned int pattern_index);

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  void set_pattern_row_index(int pattern_row_index);

  /// @copydoc PatternClientInterface::set_pattern_length(unsigned int)
  void set_pattern_length(unsigned int pattern_length);

};

#endif
