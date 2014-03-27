/**
 * @file song.hh
 *
 * All track-entry related class declarations.
 *
 * @copydetails SongInterface
 *
 */
#ifndef _SONG_HH_
#define _SONG_HH_

#ifndef SONG_FRIENDS
/// No song friends if none are available.
#define SONG_FRIENDS
#endif

#ifndef PART_FACTORY_NEW
/// By default call the actual part factory method
#define PART_FACTORY_NEW PART_FACTORY::New
#endif

#ifndef PART_FACTORY_DELETE
/// By default call the actual part factory method
#define PART_FACTORY_DELETE PART_FACTORY::Delete
#endif

#include <string>

#include "part.hh"

#include "vector_of.hh"

using namespace std;


/**
 * Pure virtual interface class for a song.
 *
 * A song consists of several parts - For example an inteo, some chorus,
 * som verses, breaks, sticks and outro and such things. See the documentation
 * for the PartInterface for more information.
 *
 */
class SongInterface {

public:

  /**
   * Default constructor.
   */
  SongInterface() {};

  /**
   * Constructor taking a song name and a reference to a list of its parts
   * as arguments.
   *
   * @param name Name of the song.
   * @param parts A reference to the list of parts that make up the song.
   */
  SongInterface(string name, PartsInterface* parts) {};

  /**
   * Default destructor.
   */
  virtual ~SongInterface() {};

  /**
   * Get the name of the song.
   *
   * @retval A string containing the name of the song.
   */
  virtual string const& get_name() const = 0;

  /**
   * Set the name of the song.
   *
   * @param name A string containing the name of the song.
   */
  virtual void set_name(string& name) = 0;

  /**
   * Get the reference to the list of parts that make up the song.
   *
   * @return A reference to the list of parts that make up the song.
   */
  virtual PartsInterface* get_parts() const = 0;

};


/**
 * Template class with optimizable handling of the song parts.
 *
 * @copydetails SongInterface
 *
 */
class SongTemplate : public virtual SongInterface {

  SONG_FRIENDS

protected:

  /// Internal storage of the name of the song.
  string name;
  /// Internal storage of the list of parts that make up the song.
  PartsInterface* parts;

public:

  /// @copydoc SongInterface::SongInterface(string,PartsInterface*)
  SongTemplate(string name, PartsInterface* parts) :
    name(name), parts(parts) {}
  /// @copydoc SongInterface::~SongInterface()
  virtual ~SongTemplate() {}
  /// @copydoc SongInterface::get_name()
  string const& get_name() const { return name; }
  /// @copydoc SongInterface::set_name(string&)
  void set_name(string& name) { this->name = name; }
  /// @copydoc SongInterface::get_parts()
  PartsInterface* get_parts() const { return parts; }

};


/**
 * Song is a specialized class of the template SongTemplate for performance
 * and reuse.
 *
 * @copydetails SongInterface
 *
 */
class Song : public SongTemplate {

public:

  /// @copydoc SongInterface::SongInterface(string,PartsInterface*)
  Song(string name, PartsInterface* parts);
  /// @copydoc SongInterface::~SongInterface()
  ~Song();

};


/**
 * Create a specialized vector to reference Song objects.
 */
VECTOR_OF(Song, Songs, 256);


/**
 * Factory class to create song objects.
 */
template<class SONG,
         class PATTERNS,
         class PART,
         class PARTS,
         class PART_FACTORY>
class SongFactoryTemplate {

public:

  /**
   * Create a new instance of a song class with the provided name and the
   * list of parts, or default.
   *
   * @param project_patterns A reference to the list of all patterns available
   *                         which is needed by the part factory.
   * @param name             The name of the song, if none provided the song
   *                         will be named "New song".
   * @param parts            A reference to a list of parts that make up the
   *                         song, if none provided a list with one item will
   *                         be created.
   *
   * @return A reference to the new song.
   */
  static SONG* New(PATTERNS* project_patterns,
                   string name = "New song",
                   PARTS* parts = new PARTS()) {
    if (0 == parts->size()) {
      PART* part = PART_FACTORY_NEW(project_patterns);
      parts->push_back(part);
    }

    SONG* song = new SONG(name, parts);
    return song;
  }

  /**
   * Destroy a song class instance and everything related to it, setting the
   * reference to NULL when done.
   *
   * @param song A pointer to a reference to a song class instance to destroy.
   */
  static void Delete(SONG **song) {
    PARTS* parts = dynamic_cast<PARTS*>((*song)->get_parts());

    while ((NULL != parts) && (0 != parts->size())) {
      PART* part = dynamic_cast<PART*>(parts->back());
      PART_FACTORY_DELETE(&part);
      parts->pop_back();
    }

    if (NULL != parts) {
      delete parts;
    }

    delete (*song);
    *song = NULL;
  }

};


/**
 * Song factory to create parts with concrete implementations of the song
 * itself, pattern list, part and part list.
 */
typedef SongFactoryTemplate<Song,
                            Patterns,
                            Part,
                            Parts,
                            PartFactory> SongFactory;

#endif
