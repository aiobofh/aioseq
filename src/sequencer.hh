/**
 * @file sequencer.hh
 *
 * + - -  - +     +-----------------+-----------+
 * | Client |<----| ClientInterface | Sequencer |
 * + - -  - +     +-----------------+           |
 *                |                             |
 *                |                             |
 *                +-----------------------------+
 */
#ifndef _SEQUENCER_HH_
#define _SEQUENCER_HH_

#include <string.h>
#include "project.hh"
#include "track.hh"
#include "pattern.hh"
#include "pattern_row.hh"

#ifndef PATTERN_ROW_FACTORY_NEW
/// By default call the actual pattern row factory method
#define PATTERN_ROW_FACTORY_NEW PATTERN_ROW_FACTORY::New
#endif

#ifndef PATTERN_ROW_FACTORY_DELETE
/// By default call the actual pattern row factory method
#define PATTERN_ROW_FACTORY_DELETE PATTERN_ROW_FACTORY::Delete
#endif

#ifndef PATTERN_FACTORY_ADD_ROWS
/// By default call the actual pattern row factory method
#define PATTERN_FACTORY_ADD_ROWS PATTERN_FACTORY::AddRows
#endif

#ifndef PATTERN_FACTORY_REMOVE_ROWS
/// By default call the actual pattern row factory method
#define PATTERN_FACTORY_REMOVE_ROWS PATTERN_FACTORY::RemoveRows
#endif

#include "client.hh"

#include "error.hh"

#ifndef SEQUENCER_FRIENDS
/// By default the sequencer have no friend classes.
#define SEQUENCER_FRIENDS
#endif

using namespace std;

/**
 * Pure virtual interface class for a sequencer.
 */
class SequencerInterface : public virtual ClientInterface {

public:

  /**
   * Default constructor.
   */
  SequencerInterface() {}

  /**
   * Constructor taking a reference to a project instance.
   *
   * @param project A reference to a project instance.
   */
  SequencerInterface(ProjectInterface* project) {}

  /**
   * Default destructor.
   */
  virtual ~SequencerInterface() {}

  /**
   * Register the client to which to communicate state changes.
   *
   * @param client A reference to a client to communicate state changes.
   */
  virtual void register_client(ClientPrimitiveInterface* client) = 0;

  /**
   * Un-register the client to which to stop communicate state changes to.
   *
   * @param client A reference to a client to stop communicate state changes
   *               to.
   */
  virtual void unregister_client(ClientPrimitiveInterface* client) = 0;

  /**
   * Get the reference to the currently selected pattern row.
   *
   * @return A reference to a pattern row.
   */
  virtual PatternRowInterface *get_row() = 0;

  /**
   * Get the reference to the pattern row at the specified index.
   *
   * @param pattern_row_index
   *
   * @return A reference to a pattern row.
   */
  virtual PatternRowInterface *get_row(int pattern_row_index) = 0;

};


/**
 * Template class with optimizable handling of a sequencer with a project.
 *
 * @copydetails SequencerInterface
 */
template<class PROJECT,
         class TRACK,
         class TRACKS,
         class PATTERN,
         class PATTERNS,
         class PATTERN_FACTORY,
         class PATTERN_ROW,
         class PATTERN_ROWS,
         class PATTERN_ROW_FACTORY>
class SequencerTemplate : public SequencerInterface {

  /// List of friend classes (used for testing).
  SEQUENCER_FRIENDS

protected:

  /// Internal storage of the reference to a project.
  ProjectInterface* project;
  /// Internal storage of the reference to a client - not set by constructor.
  ClientInterface* client;
  /// Internal storage of the current pattern index.
  int pattern_index;
  /// Internal storage of the current track index.
  int track_index;
  /// Internal storage of the current pattern row index.
  int pattern_row_index;

  /**
   * Get the current number of patterns in the pattern list.
   *
   * @return The current number of patterns in the pattern list.
   */
  virtual int get_pattern_count() {
    PATTERNS* patterns =
      dynamic_cast<PATTERNS*>(project->get_patterns());
    return patterns->size();
  }

  /**
   * Get the current number of tracks in the track list.
   *
   * @return The current number of tracks in the track list.
   */
  virtual int get_track_count() {
    TRACKS* tracks =
      dynamic_cast<TRACKS*>(project->get_tracks());
    return tracks->size();
  }

  /**
   * Get the current number of pattern rows ni the current pattern's pattern
   * row list.
   *
   * @return The current number of pattern rows.
   */
  virtual int get_pattern_row_count() {
    PATTERNS* patterns =
      dynamic_cast<PATTERNS*>(project->get_patterns());
    PATTERN* pattern =
      dynamic_cast<PATTERN*>(patterns->at(pattern_index));
    PATTERN_ROWS* pattern_rows =
      dynamic_cast<PATTERN_ROWS*>(pattern->get_pattern_rows());
    return pattern_rows->size();
  }

public:

  /// @copydoc SequencerInterface::SequencerInterface(ProjectInterface*)
  SequencerTemplate(ProjectInterface* project) : project(project),
                                                 client(NULL),
                                                 pattern_index(0),
                                                 track_index(0),
                                                 pattern_row_index(0) {}
  /// @copydoc SequencerInterface::~SequencerInterface()
  ~SequencerTemplate() {}

  // -------------------------- SequencerInterface --------------------------

  /// @copydoc SequencerInterface::register_client(ClientPrimitiveInterface*)
  void register_client(ClientPrimitiveInterface* client) {
    if (NULL != this->client) {
      error("Trying to register a client where one is already registered.");
      return;
    }
    this->client = dynamic_cast<ClientInterface*>(client);
  }
  /// @copydoc SequencerInterface::unregister_client(ClientPrimitiveInterface*)
  void unregister_client(ClientPrimitiveInterface* client) {
    if (NULL == this->client) {
      error("Can not unregister client, none registered.");
      return;
    }
    if (this->client != client) {
      error("Trying to unregister wrong client.");
      return;
    }
    this->client = NULL;
  }

  /// @copydoc SequencerInterface::get_row()
  PatternRowInterface *get_row() {
    return get_row(pattern_row_index);
  }

  /// @copydoc SequencerInterface::get_row(int)
  PatternRowInterface *get_row(int pattern_row_index) {
    PATTERNS* patterns =
      dynamic_cast<PATTERNS*>(project->get_patterns());
    PATTERN* pattern =
      dynamic_cast<PATTERN*>(patterns->at(pattern_index));
    PATTERN_ROWS* pattern_rows =
      dynamic_cast<PATTERN_ROWS*>(pattern->get_pattern_rows());
    PATTERN_ROW* pattern_row =
      dynamic_cast<PATTERN_ROW*>(pattern_rows->at(pattern_row_index));
    return pattern_row;
  }

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(int)
  virtual void set_track_index(int track_index) {
    int track_count = get_track_count();

    if (track_index < 0) {
      error("Track index " << track_index << " is invalid. Setting track "
            << "index to 0");
      track_index = 0;
    }
    if (track_index > track_count) {
      error("Track index " << track_index << " is invalid. Setting track "
            << "index to " << track_count);
      track_index = track_count;
    }

    if (track_index == this->track_index) {
      return;
    }

    this->track_index = track_index;

    if (NULL != client) {
      client->set_track_index(this->track_index);
    }
  }

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(int)
  virtual void set_pattern_index(int pattern_index) {
    int pattern_count = get_pattern_count();

    if (pattern_index < 0) {
      error("Pattern index " << pattern_index << " is invalid. Setting "
            << "pattern index to 0");
      pattern_index = 0;
    }
    if (pattern_index > pattern_count) {
      error("Pattern index " << pattern_index << " is invalid. Setting "
            << "pattern index to " << pattern_count);
      pattern_index = pattern_count;
    }

    if (pattern_index == this->pattern_index) {
      return;
    }

    this->pattern_index = pattern_index;

    if (NULL != client) {
      client->set_pattern_index(this->pattern_index);
    }
  }

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  virtual void set_pattern_row_index(int pattern_row_index) {
    int row_count = get_pattern_row_count();

    if (pattern_row_index < 0) {
      pattern_row_index = row_count;
    }
    if (pattern_row_index > row_count) {
      pattern_row_index = 0;
    }

    if (pattern_row_index == this->pattern_row_index) {
      return;
    }

    this->pattern_row_index = pattern_row_index;
    if (NULL != client) {
      cout << "Foo: " << this->pattern_row_index << endl;
      client->set_pattern_row_index(this->pattern_row_index);
    }
  }

  /// @copydoc PatternClientInterface::set_pattern_length(int)
  virtual void set_pattern_length(int pattern_length) {
    int current_length = get_pattern_row_count();
    if (pattern_length < 0) {
      error("Pattern length less than 0 is not permitted.");
      return;
    }

    if (pattern_length > 255) {
      error("Pattern length more than 255 is not permitted.");
      return;
    }

    if (pattern_length == current_length) {
      return;
    }

    TRACKS* tracks =
      dynamic_cast<TRACKS*>(project->get_tracks());
    PATTERNS* patterns =
      dynamic_cast<PATTERNS*>(project->get_patterns());
    PATTERN* pattern =
      dynamic_cast<PATTERN*>(patterns->at(pattern_index));

    tracks = tracks; // Force remove compiler warning when in test.
    pattern = pattern; // Force remove compiler warning when in test.

    if (pattern_length > current_length) {
      PATTERN_FACTORY_ADD_ROWS(tracks, pattern, pattern_length - current_length);
    }
    else {
      PATTERN_FACTORY_REMOVE_ROWS(pattern, current_length - pattern_length);
    }
    if (NULL != client) {
      client->set_pattern_length(pattern_length);
    }

  }
};


/**
 * Seqencer is a specialized class of the template SequencerTemplate for
 * performance and reuse.
 *
 * @copydetails SequencerInterface
 */
class Sequencer : public SequencerTemplate<Project,
                                           Track,
                                           Tracks,
                                           Pattern,
                                           Patterns,
                                           PatternFactory,
                                           PatternRow,
                                           PatternRows,
                                           PatternRowFactory> {

public:

  /// @copydoc SequencerInterface::SequencerInterface(ProjectInterface*)
  Sequencer(ProjectInterface* project);
  /// @copydoc SequencerInterface::~SequencerInterface()
  ~Sequencer();

};

#endif
