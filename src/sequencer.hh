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

  /**
   * Get the reference to the tracks in the current project.
   *
   * @return A reference to the tracks of the current project.
   */
  virtual TracksInterface *get_tracks() = 0;
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
         class PATTERN_ROW_FACTORY,
         class TRACK_ENTRIES,
         class TRACK_ENTRY,
         class NOTES,
         class NOTE,
         class EFFECTS,
         class EFFECT>
class SequencerTemplate : public SequencerInterface {

  /// List of friend classes (used for testing).
  SEQUENCER_FRIENDS

protected:

  /// Internal storage of the reference to a project.
  ProjectInterface* project;
  /// Internal storage of the reference to a client - not set by constructor.
  ClientPrimitiveInterface* client;
  /// Internal storage of the current pattern index.
  unsigned int pattern_index;
  /// Internal storage of the current track index.
  unsigned int track_index;
  /// Internal storage of the current pattern row index.
  unsigned int pattern_row_index;

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


  /**
   * Get the specified track entry object by indices.
   *
   * @param pattern_index     Pattern index.
   * @param track_index       Track index.
   * @param pattern_row_index Pattern row index.
   *
   * @return A reference to the found track entry object.
   */
  virtual TrackEntryInterface *get_track_entry(unsigned int pattern_index, unsigned int track_index, unsigned int pattern_row_index) {
    PATTERNS* patterns =
      dynamic_cast<PATTERNS*>(project->get_patterns());
    PATTERN* pattern =
      dynamic_cast<PATTERN*>(patterns->at(pattern_index));
    PATTERN_ROWS* rows =
      dynamic_cast<PATTERN_ROWS*>(pattern->get_pattern_rows());
    PATTERN_ROW* row =
      dynamic_cast<PATTERN_ROW*>(rows->at(pattern_row_index));
    TRACK_ENTRIES* track_entries =
      dynamic_cast<TRACK_ENTRIES*>(row->get_track_entries());
    TRACK_ENTRY* track_entry =
      dynamic_cast<TRACK_ENTRY*>(track_entries->at(track_index));
    return track_entry;
  }


  /**
   * Get the specified note object by indices.
   *
   * @param pattern_index     Pattern index.
   * @param track_index       Track index.
   * @param pattern_row_index Pattern row index.
   * @param note_index        Note index.
   *
   * @return A reference to a note at the specified pattern, track, row and
   *         note indices.
   */
  virtual NoteInterface* get_note(unsigned int pattern_index, unsigned int track_index, unsigned int pattern_row_index, unsigned int note_index) {
    TRACK_ENTRY* track_entry =
      dynamic_cast<TRACK_ENTRY*>(get_track_entry(pattern_index,
                                                 track_index,
                                                 pattern_row_index));
    NOTES* notes =
      dynamic_cast<NOTES*>(track_entry->get_notes());
    NOTE* note =
      dynamic_cast<NOTE*>(notes->at(note_index));
    return note;
  }


  /**
   * Get the specified effect object by indices.
   *
   * @param pattern_index     Pattern index.
   * @param track_index       Track index.
   * @param pattern_row_index Pattern row index.
   * @param effect_index      Effect index.
   *
   * @return A reference to an effect at the specified pattern, track, row and
   *         effect indices.
   */
  virtual EffectInterface* get_effect(unsigned int pattern_index, unsigned int track_index, unsigned int pattern_row_index, unsigned int effect_index) {
    TRACK_ENTRY* track_entry =
      dynamic_cast<TRACK_ENTRY*>(get_track_entry(pattern_index,
                                                 track_index,
                                                 pattern_row_index));
    EFFECTS* effects =
      dynamic_cast<EFFECTS*>(track_entry->get_effects());
    EFFECT* effect =
      dynamic_cast<EFFECT*>(effects->at(effect_index));
    return effect;
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
    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);
    if (NULL != pattern_client) {
      pattern_client->set_pattern_length(get_pattern_row_count());
    }
    //this->client = dynamic_cast<ClientInterface*>(client);
    this->client = client;
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

  /// @copydoc SequencerInterface::get_tracks()
  TracksInterface *get_tracks() {
    return dynamic_cast<TRACKS*>(project->get_tracks());
  }

  // ------------------------ TrackClientInterface --------------------------

  /// @copydoc TrackClientInterface::set_track_index(unsigned int)
  virtual void set_track_index(unsigned int track_index) {
    int track_count = get_track_count();

    if (track_index > static_cast<unsigned int>(track_count)) {
      error("Track index " << track_index << " is invalid. Setting track "
            << "index to " << track_count);
      track_index = static_cast<unsigned int>(track_count);
    }

    if (track_index == this->track_index) {
      return;
    }

    this->track_index = track_index;

    TrackClientInterface *track_client =
      dynamic_cast<TrackClientInterface*>(client);
    if (NULL != track_client) {
      track_client->set_track_index(this->track_index);
    }
  }

  // ------------------------ PartClientInterface ---------------------------

  /// @copydoc PartClientInterface::set_pattern_index(unsigned int)
  virtual void set_pattern_index(unsigned int pattern_index) {
    int pattern_count = get_pattern_count();

    if (pattern_index > static_cast<unsigned int>(pattern_count)) {
      error("Pattern index " << pattern_index << " is invalid. Setting "
            << "pattern index to " << pattern_count);
      pattern_index = static_cast<unsigned int>(pattern_count);
    }

    if (pattern_index == this->pattern_index) {
      return;
    }

    this->pattern_index = pattern_index;

    PartClientInterface *part_client =
      dynamic_cast<PartClientInterface*>(client);
    if (NULL != part_client) {
      part_client->set_pattern_index(this->pattern_index);
    }
  }

  // ----------------------- PatternClientInterface -------------------------

  /// @copydoc PatternClientInterface::set_pattern_row_index(int)
  virtual void set_pattern_row_index(int pattern_row_index) {
    int row_count = get_pattern_row_count();

    if (pattern_row_index < 0) {
      pattern_row_index = row_count - 1;
    }
    if (pattern_row_index >= row_count) {
      pattern_row_index = 0;
    }

    if (static_cast<unsigned int>(pattern_row_index) == this->pattern_row_index) {
      return;
    }

    this->pattern_row_index = pattern_row_index;
    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);
    if (NULL != pattern_client) {
      pattern_client->set_pattern_row_index(this->pattern_row_index);
    }
  }

  /// @copydoc PatternClientInterface::set_pattern_length(unsigned int)
  virtual void set_pattern_length(unsigned int pattern_length) {
    int current_length = get_pattern_row_count();

    if (pattern_length > 255) {
      error("Pattern length more than 255 is not permitted.");
      return;
    }

    if (pattern_length == static_cast<unsigned int>(current_length)) {
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

    if (pattern_length > static_cast<unsigned int>(current_length)) {
      PATTERN_FACTORY_ADD_ROWS(tracks, pattern,
                               pattern_length - static_cast<unsigned int>(current_length));
    }
    else {
      PATTERN_FACTORY_REMOVE_ROWS(pattern,
                                  static_cast<unsigned int>(current_length) - pattern_length);
    }

    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);

    if (NULL != pattern_client) {
      pattern_client->set_pattern_length(pattern_length);
    }

  }

  /// @copydoc PatternClientInterface::set_key(unsigned int, unsigned int, int)
  void set_key(unsigned int track_index, unsigned int note_index,
               int key) {
    NOTE* note = dynamic_cast<NOTE*>(get_note(pattern_index,
                                              track_index,
                                              pattern_row_index,
                                              note_index));
    note->set_key(key);

    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);

    if (NULL != pattern_client) {
      pattern_client->set_key(track_index, note_index, key);
    }
  }


  /// @copydoc PatternClientInterface::set_velocity(unsigned int, unsigned int, int, bool)
  void set_velocity(unsigned int track_index, unsigned int note_index, int velocity, bool high_nibble) {
    NOTE* note = dynamic_cast<NOTE*>(get_note(pattern_index,
                                              track_index,
                                              pattern_row_index,
                                              note_index));
    velocity = velocity & 0x0f;
    int v;
    if (true == high_nibble) {
      v = ((velocity << 4) & 0xf0) | (note->get_velocity() & 0x0f);
    }
    else {
      v = velocity | (note->get_velocity() & 0xf0);
    }

    note->set_velocity(v);

    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);

    if (NULL != pattern_client) {
      pattern_client->set_velocity(track_index, note_index, velocity, high_nibble);
    }
  }


  /// @copydoc PatternClientInterface::set_command(unsigned int, unsigned int, int, bool)
  void set_command(unsigned int track_index, unsigned int effect_index, int command, bool high_nibble) {
    EFFECT* effect = dynamic_cast<EFFECT*>(get_effect(pattern_index,
                                                    track_index,
                                                    pattern_row_index,
                                                    effect_index));
    command = command & 0x0f;
    int v;
    if (true == high_nibble) {
      v = ((command << 4) & 0xf0) | (effect->get_command() & 0x0f);
    }
    else {
      v = command | (effect->get_command() & 0xf0);
    }

    effect->set_command(v);

    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);

    if (NULL != pattern_client) {
      pattern_client->set_command(track_index, effect_index, command, high_nibble);
    }
  }


  /// @copydoc PatternClientInterface::set_value(unsigned int, unsigned int, int, bool)
  void set_value(unsigned int track_index, unsigned int effect_index, int value, bool high_nibble) {
    EFFECT* effect = dynamic_cast<EFFECT*>(get_effect(pattern_index,
                                                    track_index,
                                                    pattern_row_index,
                                                    effect_index));
    value = value & 0x0f;
    int v;
    if (true == high_nibble) {
      v = ((value << 4) & 0xf0) | (effect->get_value() & 0x0f);
    }
    else {
      v = value | (effect->get_value() & 0xf0);
    }

    effect->set_value(v);

    PatternClientInterface *pattern_client =
      dynamic_cast<PatternClientInterface*>(client);

    if (NULL != pattern_client) {
      pattern_client->set_value(track_index, effect_index, value, high_nibble);
    }
  }

};


/**
 * Seqencer is a specialized class of the template SequencerTemplate for
 * performance and reuse.
 *
 * @copydetails SequencerInterface
 */
typedef SequencerTemplate<Project,
                          Track,
                          Tracks,
                          Pattern,
                          Patterns,
                          PatternFactory,
                          PatternRow,
                          PatternRows,
                          PatternRowFactory,
                          TrackEntries,
                          TrackEntry,
                          Notes,
                          Note,
                          Effects,
                          Effect> Sequencer;


#endif
