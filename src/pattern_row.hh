/**
 * @file pattern_row.hh
 *
 * All pattern row related class declarations.
 *
 */
#ifndef _PATTERN_ROW_HH_
#define _PATTERN_ROW_HH_

#ifndef PATTERN_ROW_FRIENDS
/// No pattern row friends if none are available.
#define PATTERN_ROW_FRIENDS
#endif

#ifndef TRACK_ENTRY_FACTORY_NEW
/// By default call the actual track entry factory method
#define TRACK_ENTRY_FACTORY_NEW TRACK_ENTRY_FACTORY::New
#endif

#ifndef TRACK_ENTRY_FACTORY_ADD
/// By default call the actual track entry factory method
#define TRACK_ENTRY_FACTORY_ADD TRACK_ENTRY_FACTORY::Add
#endif

#ifndef TRACK_ENTRY_FACTORY_REMOVE
/// By default call the actual track entry factory method
#define TRACK_ENTRY_FACTORY_REMOVE TRACK_ENTRY_FACTORY::Remove
#endif

#ifndef TRACK_ENTRY_FACTORY_DELETE
/// By default call the actual track entry factory method
#define TRACK_ENTRY_FACTORY_DELETE TRACK_ENTRY_FACTORY::Delete
#endif

#include "vector_of.hh"

#include "track_entry.hh"


/**
 * Pure virtual interface class for a pattern row.
 */
class PatternRowInterface {

public:

  /**
   * Default constructor.
   */
  PatternRowInterface() {};

  /**
   * Constructor taking a reference to the list of track entries for this
   * pattern row.
   *
   * @param track_entries A reference to the list of track entries for this
   *                      pattern row.
   */
  PatternRowInterface(TrackEntriesInterface* track_entries) {};

  /**
   * Default destructor.
   */
  virtual ~PatternRowInterface() {};

  /**
   * Get the reference to the list of track entries of this pattern row.
   *
   * @return A reference to the list of track entries of this pattern row.
   */
  virtual TrackEntriesInterface* get_track_entries() = 0;

};


/**
 * Template class with optimizable handling of track entries.
 *
 * @todo The PatternRow does not have to be a template, since no optimizations
 *       are needed. It's just a container class.
 */
template<class TRACK_ENTRIES>
class PatternRowTemplate : virtual public PatternRowInterface {

  PATTERN_ROW_FRIENDS

protected:

  /**
   * Internal storage of the reference to the list of track entries on the
   * pattern_row.
   */
  TrackEntriesInterface* track_entries;

public:

  /// @copydoc PatternRowInterface::PatternRowInterface(TrackEntriesInterface*)
  PatternRowTemplate(TrackEntriesInterface* track_entries) :
    track_entries(track_entries) {}
  /// @copydoc PatternRowInterface::~PatternRowInterface()
  ~PatternRowTemplate() {}
  /// @copydoc PatternRowInterface::get_track_entries()
  TrackEntriesInterface *get_track_entries() { return track_entries; };

};


/**
 * PatternRow is a specialized class of the template PatternRowTemplate for
 * performance and reuse.
 */
class PatternRow : public PatternRowTemplate<TrackEntries> {

public:

  /// @copydoc PatternRowInterface::PatternRowInterface(TrackEntriesInterface*)
  PatternRow(TrackEntriesInterface *track_entries);
  /// @copydoc PatternRowInterface::~PatternRowInterface()
  ~PatternRow();

};


/**
 * Create a specialized vector to reference PatternRow objects.
 */
VECTOR_OF(PatternRow, PatternRows, 256);


/**
 * Factory class to create pattern row objects.
 */
template<class PATTERN_ROW,
         class PATTERN_ROWS,
         class TRACK,
         class TRACKS,
         class TRACK_ENTRY,
         class TRACK_ENTRIES,
         class TRACK_ENTRY_FACTORY>
class PatternRowFactoryTemplate {

public:

  /**
   * Create a new instance of a pattern row class with the configuration of
   * the references tracks parameter to set the correctly configured track
   * entries, or default.
   *
   * @param tracks        A reference to a list of tracks providing the
   *                      configuration of each track entry if not provided.
   * @param track_entries A reference to a list of track entries which have to
   *                      have the exact same properties as the corresponding
   *                      track list entry specifies. If this parameter is not
   *                      provided a track entry will be allocated and
   *                      configured with default settings.
   *
   * @return A reference to the new pattern row.
   */
  static PATTERN_ROW* New(TRACKS* tracks,
                          TRACK_ENTRIES* track_entries= new TRACK_ENTRIES()) {
    if ((0 != track_entries->size()) &&
        (track_entries->size() != tracks->size())) {
      error("The number of track entity entries provided does not match the "
            << "number of track entries.");
      return NULL;
    }

    for (int i = track_entries->size(); i < tracks->size(); i++) {
      TRACK *track = tracks->at(i);
      TRACK_ENTRY* track_entry = TRACK_ENTRY_FACTORY_NEW(track);
      track_entries->push_back(track_entry);
    }

    // Construct the pattern row
    PATTERN_ROW *pattern_row = new PATTERN_ROW(track_entries);
    return pattern_row;
  }

  /**
   * Destroy a pattern row instance, destroying all the track entries and
   * sets the reference to NULL when done.
   *
   * @param pattern_row A pointer to a reference to a pattern row class
   *                    instance to destroy.
   */
  static void Delete(PATTERN_ROW** pattern_row) {
    TRACK_ENTRIES* track_entries =
      dynamic_cast<TRACK_ENTRIES*>((*pattern_row)->get_track_entries());

    while ((NULL != track_entries) && (0 != track_entries->size())) {
      TRACK_ENTRY* track_entry =
        dynamic_cast<TRACK_ENTRY*>(track_entries->back());
      TRACK_ENTRY_FACTORY_DELETE(&track_entry);
      track_entries->pop_back();
    }

    if (NULL != track_entries) {
      delete track_entries;
    }

    delete static_cast<PATTERN_ROW*>(*pattern_row);
    *pattern_row = NULL;
  }

};


/**
 * PatternRow factory to create pattern rows with a concrete implementation
 * of the pattern row itself and the track entry class.
 */
typedef PatternRowFactoryTemplate<PatternRow,
                                  PatternRows,
                                  Track,
                                  Tracks,
                                  TrackEntry,
                                  TrackEntries,
                                  TrackEntryFactory> PatternRowFactory;

#endif

