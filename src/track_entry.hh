/**
 * @file track_entry.hh
 *
 * All track-entry related class declarations.
 *
 * @copydetails TrackEntryInterface
 *
 */
#ifndef _TRACK_ENTRY_HH_
#define _TRACK_ENTRY_HH_

#ifndef TRACK_ENTRY_FRIENDS
/// No track entry friends if none are available.
#define TRACK_ENTRY_FRIENDS
#endif

#ifndef NOTE_FACTORY_NEW
/// By default call the actual note factory method
#define NOTE_FACTORY_NEW NOTE_FACTORY::New
#endif

#ifndef EFFECT_FACTORY_NEW
/// By default call the actual effect factory method
#define EFFECT_FACTORY_NEW EFFECT_FACTORY::New
#endif

#ifndef NOTE_FACTORY_DELETE
/// By default call the actual note factory method
#define NOTE_FACTORY_DELETE NOTE_FACTORY::Delete
#endif

#ifndef EFFECT_FACTORY_DELETE
/// By default call the actual effect factory method
#define EFFECT_FACTORY_DELETE EFFECT_FACTORY::Delete
#endif

#include "note.hh"
#include "effect.hh"
#include "track.hh"

#include "vector_of.hh"
#include "error.hh"


/**
 * Pure virtual interface class for a track entry.
 *
 * A track entry consists of zero or more notes and zero or more effects, see
 * the documentation for the NoteInterface and EffectInterface for more
 * information.
 *
 */
class TrackEntryInterface {

public:

  /**
   * Default constructor.
   */
  TrackEntryInterface() {};

  /**
   * Constructor taking a reference to a list of notes, a reference to a
   * list of effects and a reference to a specific track.
   *
   * @param notes A reference to a list of notes for this track entry.
   * @param effects A reference to a list of effects for this track entry.
   * @param track A reference to a track to which to bind this track entry.
   */
  TrackEntryInterface(NotesInterface* notes, EffectsInterface* effects, TrackInterface* track) {};

  /**
   * Default destructor.
   */
  virtual ~TrackEntryInterface() {};

  /**
   * Get the reference to the list of notes in this track entry.
   *
   * @return A reference to the list of notes in this track entry.
   */
  virtual NotesInterface* get_notes() const = 0;

  /**
   * Get the reference to the list of effects in this track entry.
   *
   * @return A reference to the list of effects in this track entry.
   */
  virtual EffectsInterface* get_effects() const = 0;

  /**
   * Get the reference to the track to which this track entry is bound.
   *
   * @return A reference to the track to which this track entry is bound.
   */
  virtual TrackInterface* get_track() const = 0;

};


/**
 * Template class with optimizable handling of a track entry with notes and
 * effects.
 *
 * @copydetails TrackEntryInterface
 *
 * @todo The TrackEntry does not have to be a template, since no optimizations
 *       are needed. It is just a container class.
 */
template<class NOTES, class EFFECTS, class TRACK>
class TrackEntryTemplate : virtual public TrackEntryInterface {

  TRACK_ENTRY_FRIENDS

protected:

  /// Internal storage of the note list for the track entry.
  NotesInterface* notes;
  /// Internal storage of the effect list for the track entry.
  EffectsInterface* effects;
  /// Internal storage of the referencing track object.
  TrackInterface* track;

public:

  /// @copydoc TrackEntryInterface::TrackEntryInterface(NotesInterface*,EffectsInterface*,TrackInterface*)
  TrackEntryTemplate(NotesInterface* notes, EffectsInterface* effects,
                     TrackInterface* track) :
    notes(notes), effects(effects), track(track) {}
  /// @copydoc TrackEntryInterface::~TrackEntryInterface()
  virtual ~TrackEntryTemplate() {}
  /// @copydoc TrackEntryInterface::get_notes()
  NotesInterface* get_notes() const { return notes; }
  /// @copydoc TrackEntryInterface::get_effects()
  EffectsInterface* get_effects() const { return effects; }
  /// @copydoc TrackEntryInterface::get_track()
  TrackInterface* get_track() const { return track; }

};


/**
 * TrackEntry is a specialized class of the template TrackEntryTemplate for
 * performance and reuse.
 *
 * @copydetails TrackEntryInterface
 *
 */
class TrackEntry : public TrackEntryTemplate<Notes, Effects, Track> {

public:

  /// @copydoc TrackEntryInterface::TrackEntryInterface(NotesInterface*, EffectsInterface*,TrackInterface*)
  TrackEntry(NotesInterface* notes = NULL, EffectsInterface* effects = NULL, TrackInterface* track = NULL);
  /// @copydoc TrackEntryInterface::~TrackEntryInterface()
  ~TrackEntry();

};


/**
 * Create a specialized vector to reference TrackEntry objects.
 */
VECTOR_OF(TrackEntry, TrackEntries, 256);


/**
 * Factory class to create track entry objects.
 */
template<class TRACK_ENTRY,
         class TRACK,
         class NOTE,
         class EFFECT,
         class NOTES,
         class EFFECTS,
         class NOTE_FACTORY,
         class EFFECT_FACTORY>
class TrackEntryFactoryTemplate {

public:

  /**
   * Create a new instance of an track entry class with the configuration of
   * referenced track parameter to set the correct number of notes and effects
   * or default.
   *
   * @param track   A reference to a track instance providing number of notes
   *                and effects to add if no notes_list and no effects_list is
   *                not provided.
   * @param notes   A reference to a list of notes which have to have the
   *                exact same number of entries as the track object says,
   *                if this parameter is not provided a new list will be
   *                allocated and be filled with the number of notes as the
   *                referenced track specifies.
   * @param effects A reference to a list of effects which have to have the
   *                exact same number of entries a the track object says, or
   *                if this parameter is not provided a new list will be
   *                allocated and be filled with the number of effects as the
   *                referenced track specifies.
   *
   * @return A rference to the new track entry instance.
   */
  static TRACK_ENTRY* New(TRACK* track,
                          NOTES* notes = new NOTES(),
                          EFFECTS* effects = new EFFECTS()) {
    unsigned char number_of_notes;
    unsigned char number_of_effects;

    number_of_notes = track->TRACK::get_notes();
    number_of_effects = track->TRACK::get_effects();

    if ((0 != notes->size()) && (notes->size() != number_of_notes)) {
      error("The notes list is not the same length as track specifies.");
      return NULL;
    }

    if ((0 != effects->size()) && (effects->size() != number_of_effects)) {
      error("The effects list is not the same length as track specifies.");
      return NULL;
    }

    for (int i = notes->size(); i < number_of_notes; i++) {
      NOTE* note = NOTE_FACTORY_NEW();
      notes->push_back(note);
    }

    for (int i = effects->size(); i < number_of_effects; i++) {
      EFFECT* effect = EFFECT_FACTORY_NEW();
      effects->push_back(effect);
    }

    TRACK_ENTRY* track_entry = new TRACK_ENTRY(notes, effects, track);

    return track_entry;
  };

  /**
   * Destroy a track entry instance, destroying all notes and effects, and
   * setting the reference to NULL when done.
   *
   * @param track_entry A pointer to a reference to a track entry class
   *                    instance to destroy.
   */
  static void Delete(TRACK_ENTRY** track_entry) {
    NOTES* notes = dynamic_cast<NOTES*>((*track_entry)->get_notes());
    EFFECTS* effects = dynamic_cast<EFFECTS*>((*track_entry)->get_effects());

    while ((NULL != notes) && (0 != notes->size())) {
      NOTE *note = dynamic_cast<NOTE*>(notes->back());
      NOTE_FACTORY_DELETE(&note);
      notes->pop_back();
    }

    while ((NULL != effects) && (0 != effects->size())) {
      EFFECT *effect = dynamic_cast<EFFECT*>(effects->back());
      EFFECT_FACTORY_DELETE(&effect);
      effects->pop_back();
    }

    if (NULL != notes) {
      delete notes;
    }
    if (NULL != effects) {
      delete effects;
    }

    delete static_cast<TRACK_ENTRY*>(*track_entry);
    *track_entry = NULL;
  }

};


/**
 * TrackEntry factory create track entries with concrete implementations of
 * the track entry itself and the note and effect classes.
 */
typedef TrackEntryFactoryTemplate<TrackEntry,
                                  Track,
                                  Note,
                                  Effect,
                                  Notes,
                                  Effects,
                                  NoteFactory,
                                  EffectFactory> TrackEntryFactory;


#endif
