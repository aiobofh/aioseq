/**
 * @file project.hh
 *
 * All project related class declarations.
 *
 */
#ifndef _PROJECT_HH_
#define _PROJECT_HH_

#include <string>

#include "pattern.hh"
#include "song.hh"

#include "vector_of.hh"

using namespace std;

#ifndef PROJECT_FRIENDS
/// No project friends if none are available.
#define PROJECT_FRIENDS
#endif

#ifndef TRACK_FACTORY_NEW
/// By default call the actual track factory method
#define TRACK_FACTORY_NEW TRACK_FACTORY::New
#endif

#ifndef PATTERN_FACTORY_NEW
/// By default call the actual pattern factory method
#define PATTERN_FACTORY_NEW PATTERN_FACTORY::New
#endif

#ifndef SONG_FACTORY_NEW
/// By default call the actual song factory method
#define SONG_FACTORY_NEW SONG_FACTORY::New
#endif

#ifndef TRACK_FACTORY_DELETE
/// By default call the actual track factory method
#define TRACK_FACTORY_DELETE TRACK_FACTORY::Delete
#endif

#ifndef PATTERN_FACTORY_DELETE
/// By default call the actual pattern factory method
#define PATTERN_FACTORY_DELETE PATTERN_FACTORY::Delete
#endif

#ifndef SONG_FACTORY_DELETE
/// By default call the actual song factory method
#define SONG_FACTORY_DELETE SONG_FACTORY::Delete
#endif



/**
 * Pure virtual interface class for a project.
 *
 * A project contains tracks, patterns, songs and all their parts.
 *
 */
class ProjectInterface {

public:

  /**
   * Default constructor.
   */
  ProjectInterface() {};

  /**
   * Constructor taking a name, a reference to a list of tracks, a
   * reference to a list of patterns and a reference to a list of songs.
   *
   * @param name     A string containing the name of the project.
   * @param tracks   A reference to a list of tracks.
   * @param patterns A reference to a list of patterns.
   * @param songs    A reference to a list of songs.
   */
  ProjectInterface(string name, TracksInterface* tracks, PatternsInterface* patterns, SongsInterface* songs) {};

  /**
   * Default destructor.
   */
  virtual ~ProjectInterface() {};

  /**
   * Get the name of the project.
   *
   * @return A string containing the name of the project.
   */
  virtual string const& get_name() = 0;

  /**
   * Set the name of the project.
   *
   * @param name A string containing the name of the project.
   */
  virtual void set_name(string name) = 0;

  /**
   * Get the reference to the list of tracks within the project.
   *
   * @return A reference to the list of tracks within the project.
   */
  virtual TracksInterface* get_tracks() const = 0;

  /**
   * Get the reference to the list of patterns within the project.
   *
   * @return A reference to the list of patterns within the project.
   */
  virtual PatternsInterface* get_patterns() const = 0;

  /**
   * Get the reference to the list of songs within the project.
   *
   * @return A reference to the list of songs within the project.
   */
  virtual SongsInterface *get_songs() const = 0;

};


/**
 * Template class with optmizable handling of a project with tracks,
 * patterns and songs.
 *
 * @copydetails ProjectInterface
 */
class ProjectTemplate : public virtual ProjectInterface {

  PROJECT_FRIENDS

protected:

  /// Internal storage of the name of the project.
  string name;
  /// Internal storage of the reference to the list of tracks in the project.
  TracksInterface* tracks;
  /// Internal storage of the reference to the list of patterns in the project.
  PatternsInterface* patterns;
  /// Internal storage of the reference to the list of songs in the project.
  SongsInterface *songs;

public:

  /// @copydoc ProjectInterface::ProjectInterface(string,TracksInterface*,PatternsInterface*,SongsInterface*)
  ProjectTemplate(string name,
                  TracksInterface* tracks,
                  PatternsInterface* patterns,
                  SongsInterface* songs) :
    name(name), tracks(tracks), patterns(patterns), songs(songs) {}
  /// @copydoc ProjectInterface::~ProjectInterface()
  virtual ~ProjectTemplate() {}
  /// @copydoc ProjectInterface::get_name()
  string const& get_name() { return name; }
  /// @copydoc ProjectInterface::set_name(string)
  void set_name(string name) { this->name = name; }
  /// @copydoc ProjectInterface::get_tracks()
  TracksInterface* get_tracks() const { return tracks; }
  /// @copydoc ProjectInterface::get_patterns()
  PatternsInterface* get_patterns() const { return patterns; }
  /// @copydoc ProjectInterface::get_songs()
  SongsInterface* get_songs() const { return songs; }

};


/**
 * Project is a specialized class of the template ProjectTemplate for
 * performance and reuse.
 *
 * @copydetails ProjectInterface
 */
class Project : public ProjectTemplate {

public:

  /// @copydoc ProjectInterface::ProjectInterface(string,TracksInterface*,PatternsInterface*,SongsInterface*)
  Project(string name, TracksInterface* tracks, PatternsInterface* patterns, SongsInterface* songs);
  /// @copydoc ProjectInterface::~ProjectInterface()
  ~Project();

};


/**
 * Create a specialized vector to reference Project objects.
 */
VECTOR_OF(Project, Projects, 256);


/**
 * Factory class to create project objects.
 */
template<class PROJECT,
         class TRACK,
         class TRACKS,
         class TRACK_FACTORY,
         class PATTERN,
         class PATTERNS,
         class PATTERN_FACTORY,
         class SONG,
         class SONGS,
         class SONG_FACTORY>
class ProjectFactoryTemplate {

public:

  /**
   * Create a new instance of a project class with the configuration of the
   * parameters, or default.
   *
   * @param name     Name of the project, if not provided default is "New
   *                 project".
   * @param tracks   A reference to a list of tracks, if not provided a list
   *                 containing one track will be created.
   * @param patterns A reference to a list of patterns, if not provided a
   *                 list containing one pattern will be created.
   * @param songs    A reference to a list of songs, if not provided a list
   *                 containing one song will be created.
   *
   * @return A reference to the new project instance.
   */
  static PROJECT* New(string name = "New project",
                      TRACKS* tracks = new TRACKS(),
                      SONGS* songs = new SONGS(),
                      PATTERNS* patterns = new PATTERNS()) {
    if (0 == tracks->size()) {
      TRACK* track = TRACK_FACTORY_NEW();
      tracks->push_back(track);
    }
    if (0 == patterns->size()) {
      PATTERN* pattern = PATTERN_FACTORY_NEW(tracks);
      patterns->push_back(pattern);
    }
    if (0 == songs->size()) {
      SONG* song = SONG_FACTORY_NEW(patterns);
      songs->push_back(song);
    }

    PROJECT* project = new PROJECT(name, tracks, patterns, songs);
    return project;
  }

  /**
   * Destroy a project instance, destroying all the patterns, songs and tracks
   * and set the reference to NULL when done.
   *
   * @param project A pointer to a reference to a project class instance to
   *                destroy.
   */
  static void Delete(PROJECT **project) {
    TRACKS* tracks = dynamic_cast<TRACKS*>((*project)->get_tracks());
    PATTERNS* patterns = dynamic_cast<PATTERNS*>((*project)->get_patterns());
    SONGS* songs = dynamic_cast<SONGS*>((*project)->get_songs());

    while ((NULL != songs) && (0 != songs->size())) {
      SONG* song = dynamic_cast<SONG*>(songs->back());
      SONG_FACTORY_DELETE(&song);
      songs->pop_back();
    }

    while ((NULL != patterns) && (0 != patterns->size())) {
      PATTERN* pattern = dynamic_cast<PATTERN*>(patterns->back());
      PATTERN_FACTORY_DELETE(&pattern);
      patterns->pop_back();
    }

    while ((NULL != tracks) && (0 != tracks->size())) {
      TRACK* track = dynamic_cast<TRACK*>(tracks->back());
      TRACK_FACTORY_DELETE(&track);
      tracks->pop_back();
    }

    if (NULL != tracks) {
      delete tracks;
    }
    if (NULL != patterns) {
      delete patterns;
    }
    if (NULL != songs) {
      delete songs;
    }

    delete (*project);
    *project = NULL;
  }

};


/**
 * Project factory create project with concrete implementations of the project
 * itself, track, pattern and song classes.
 */
typedef ProjectFactoryTemplate<Project,
                               Track,
                               Tracks,
                               TrackFactory,
                               Pattern,
                               Patterns,
                               PatternFactory,
                               Song,
                               Songs,
                               SongFactory> ProjectFactory;

#endif
