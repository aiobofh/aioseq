/**
 * @file track.hh
 *
 * All track related class declarations.
 *
 */
#ifndef _TRACK_HH_
#define _TRACK_HH_

#ifndef TRACK_FRIENDS
#define TRACK_FRIENDS
#endif

#include <string>

#include "vector_of.hh"

using namespace std;


/**
 * Pure virtual interface class for a track.
 */
class TrackInterface {

public:

  /**
   * Default constructor.
   */
  TrackInterface() {};

  /**
   * Constructor taking the number of notes, effects and a name to set this
   * channel to.
   *
   * @param notes Number of notes to set this track to.
   * @param effects Number of effects to set this track to.
   * @param name Name of the track.
   */
  TrackInterface(int notes, int effects, string name) {};

  /**
   * Default destructor.
   */
  virtual ~TrackInterface() {};

  /**
   * Get the name of the track.
   *
   * @return A string with the name of the track.
   */
  virtual string const& get_name() = 0;

  /**
   * Set the name of the track.
   *
   * @param name A string with the name set on the track.
   */
  virtual void set_name(string name) = 0;

  /**
   * Get the number of notes in the track.
   *
   * @return The number of notes in the track.
   */
  virtual int get_notes() const = 0;

  /**
   * Set the number of notes in the track.
   *
   * @param notes The number of notes to set this track to.
   */
  virtual void set_notes(int notes) = 0;

  /**
   * Get the number of effects in the track.
   *
   * @return The number of effects in the track.
   */
  virtual int get_effects() const = 0;

  /**
   * Set the number of effects in the track.
   *
   * @param effects The number of effects to set this track to.
   */
  virtual void set_effects(int effects) = 0;

};


/**
 * Template class with optimizable handling of track entries in all patterns.
 *
 * @todo So far this is not a template. It might not become one in the future.
 */
class TrackTemplate : public virtual TrackInterface {

  TRACK_FRIENDS

protected:

  /// Internal storage for the number of notes in the track.
  int notes;
  /// Internal storage for the number of effects in the track.
  int effects;
  /// Internal storage for the name of the track.
  string name;

public:

  /// @copydoc TrackInterface::TrackInterface(int,int,string)
  TrackTemplate(int notes, int effects, string name) :
    notes(notes), effects(notes), name(name) {}
  /// @copydoc TrackInterface::~TrackInterface()
  virtual ~TrackTemplate() {}
  /// @copydoc TrackInterface::get_name()
  string const& get_name() { return name; }
  /// @copydoc TrackInterface::set_name(string)
  void set_name(string name) { this->name = name; }
  /// @copydoc TrackInterface::get_notes()
  int get_notes() const { return notes; };
  /// @copydoc TrackInterface::set_notes(int)
  void set_notes(int notes) { this->notes = notes; };
  /// @copydoc TrackInterface::get_effects()
  int get_effects() const { return effects; };
  /// @copydoc TrackInterface::set_effects(int)
  void set_effects(int effects) { this->effects = effects; };

};


/**
 * Track is a specialized class of the template TrackTemplate for performance
 * and reuse.
 */
class Track : public TrackTemplate {

public:

  /// @copydoc TrackInterface::TrackInterface(int,int,string)
  Track(int notes, int effects, string name);
  /// @copydoc TrackInterface::~TrackInterface()
  ~Track();

};



/**
 * Create a specialized vector to reference TrackEntry objects.
 */
VECTOR_OF(Track, Tracks, 256);


/**
 * Factory class to create track objects.
 */
template<class TRACK>
class TrackFactoryTemplate {

public:

  /**
   * Create a new instance of a track class with the configuration passed
   * to it, or defaults.
   *
   * @param notes   Number of notes to allocate in the new track, if not
   *                provided default is 1.
   * @param effects Number of effects to allocate in the new track, if not
   *                provided default is 1.
   * @param name    The name of the track, if none provided the track name
   *                will be "New track".
   *
   * @return A reference to the new track instance,
   */
  static TRACK* New(int notes = 1, int effects = 1,
                    string name = "New track") {
    TRACK* track = new TRACK(notes, effects, name);
    return track;
  }

  /**
   * Destroy a track class instance and everything related to it, setting the
   * reference to NULL when done.
   *
   * @param track A pointer to a reference to a track class instance to
   *              destroy.
   */
  static void Delete(TRACK **track) {
    delete (*track);
    *track = NULL;
  }

};


/**
 * Track factory to create tracks with concrete implementation of the track
 * itself.
 */
typedef TrackFactoryTemplate<Track> TrackFactory;

#endif
