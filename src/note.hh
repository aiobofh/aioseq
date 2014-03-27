/**
 * @file note.hh
 *
 * All note related class declarations.
 *
 * @copydetails NoteInterface
 *
 */
#ifndef _NOTE_HH_
#define _NOTE_HH_

#ifndef NOTE_FRIENDS
/// No note friends of none are available.
#define NOTE_FRIENDS
#endif

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "vector_of.hh"


/**
 * Pure virtual interface class for a note.
 *
 * A note is a musical note represented by a key on a keyboard, and the
 * velocity at which it was hit. If the key is a 0 (zero) the note event
 * does not involve any musical note key.
 *
 */
class NoteInterface {

public:

  /**
   * Default constructor.
   */
  NoteInterface() {};

  /**
   * Constructor taking a key and a velocity at which it was hit as arguments.
   *
   * @param key Key note.
   * @param velocity Velocity at which the key was hit.
   */
  NoteInterface(int key, int velocity) {};

  /**
   * Default destructor.
   */
  virtual ~NoteInterface() {};

  /**
   * Get the key stored in a note.
   *
   * @return Key value.
   */
  virtual int get_key() const = 0;

  /**
   * Set the key in a note.
   *
   * @param key Key note.
   */
  virtual void set_key(int key) = 0;

  /**
   * Get the velocity stored in a note.
   *
   * @return Velocity at which the key of the note was hit.
   */
  virtual int get_velocity() const = 0;

  /**
   * Set the velocity at which the key in a note was hit.
   *
   * @param velocity Velocity at which the key of the note was hit.
   */
  virtual void set_velocity(int velocity) = 0;

};

/**
 * The Note class represent a note event with a musical note and a velocity
 * at which it was hit or released.
 *
 * @copydetails NoteInterface
 *
 */
class Note : virtual public NoteInterface {

  NOTE_FRIENDS

protected:

  /// Internal storage of the key value of the note
  int key;
  /// Internal storage of the velocity at which the key was hit.
  int velocity;

public:

  /// Hightest posible key value
  static const int MAX_NOTE = 127;
  /// @copydoc NoteInterface::NoteInterface(int,int)
  Note(int key = 0, int velocity = 0);
  /// @copydoc NoteInterface::~NoteInterface()
  virtual ~Note();
  /// @copydoc NoteInterface::get_key()
  virtual int get_key() const;
  /// @copydoc NoteInterface::set_key(int)
  virtual void set_key(int key);
  /// @copydoc NoteInterface::get_velocity()
  virtual int get_velocity() const;
  /// @copydoc NoteInterface::set_velocity(int)
  virtual void set_velocity(int velocity);

};

/**
 * Create a specialized vector to reference Note objects.
 */
VECTOR_OF(Note, Notes, 16);

/**
 * Factory class to create note objects.
 */
template<class NOTE>
class NoteFactoryTemplate {

public:

  /**
   * Create a new instance of a note class with the provided key and
   * velocity or default.
   *
   * If no arguments are provided the note will have the key value 0 and the
   * velocity value 0.
   *
   * @param key Key note (default 0).
   * @param velocity Velocity at which the key was hit (default 0).
   *
   * @return A reference to the new note instance.
   */
  static NOTE* New(int key = 0, int velocity = 0) {
    NOTE* note = new NOTE(key, velocity);
    return note;
  }

  /**
   * Destroy a note class instance and everything related to it, setting the
   * reference to NULL when done.
   *
   * @param note A pointer to a reference to a note class instance to destroy.
   */
  static void Delete(NOTE** note) {
    delete static_cast<NOTE*>(*note);
    *note = NULL;
  }

};


/**
 * Note factory to create notes with concrete implementation of the note
 * itself.
 */
typedef NoteFactoryTemplate<Note> NoteFactory;

#endif
