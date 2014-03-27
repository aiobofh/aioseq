/**
 * @file song_test.cc
 *
 * Test-suite for the Song class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Song class to be able to
 * inspect the protected or private variables.
 */
#define SONG_FRIENDS                            \
  friend_test(Song, Constructor)                \
  friend_test(Song, SettersAndGetters)          \
  friend_test(Song, Factory)                    \

#include "mock_part.hh"

/// Override the macro defined in song.hh for stubbed functionallity
#define PART_FACTORY_NEW(project_patterns) new MockPart(); \
  project_patterns = project_patterns
/// Override the macro defined in song.hh for stubbed functionallity
#define PART_FACTORY_DELETE(part) delete *(part); *part = NULL

#include "song.hh"

#include "mock_song.hh"


/**
 * @test Song constructor.
 *
 * Make sure that the constructor pass the name and the reference to the list
 * of parts that makes the song to the internal storage untouched.
 */
test_case(Song, Constructor) {
  string name("Foobar");
  MockParts parts;
  Song song(name, &parts);
  assert_eq(name, song.name);
  assert_eq(&parts, song.parts);
}


/**
 * @test Song setters and getters.
 *
 * Make sure that the name of the song is possible to set or fetch, and the
 * reference to the list of parts that makes is possible to fetch.
 */
test_case(Song, SettersAndGetters) {
  string name("Foobar");
  MockParts parts;
  Song song(name, &parts);

  string name_foo("Foo");
  string name_bar("Bar");

  assert_setter_and_getter_twice_eq(name_foo, name_bar, song, name);
  assert_eq(&parts, song.get_parts());
}


/**
 * @test SongFactory creates valid song.
 *
 * Make sure that the SongFactory is able to create a valid part, both with
 * part-list reference argument or name and without.
 */
test_case(Song, Factory) {
  MockPatterns project_patterns;

  Song *song = SongFactoryMock::New(&project_patterns);
  assert_eq("New song", song->name);
  assert_eq(1, dynamic_cast<MockParts*>(song->parts)->size());

  SongFactoryMock::Delete(&song);
  assert_eq(NULL, song);

}
