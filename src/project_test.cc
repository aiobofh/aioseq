/**
 * @file project_test.cc
 *
 * Test-suite for the Project class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Pattern class to be able to
 * inspect the protected or private variables.
 */
#define PROJECT_FRIENDS                         \
  friend_test(Project, Constructor)             \
  friend_test(Project, SettersAndGetters)       \
  friend_test(Project, Factory)                 \

#include "mock_track.hh"
#include "mock_pattern.hh"
#include "mock_song.hh"

/// Override the macro defined in project.hh for stubbed functionallity
#define SONG_FACTROY_NEW new MockSong
/// Override the macro defined in project.hh for stubbed functionallity
#define SONG_FACTORY_DELETE(song) delete *(song); *song = NULL

/// Override the macro defined in project.hh for stubbed functionallity
#define TRACK_FACTORY_NEW new MockTrack
/// Override the macro defined in project.hh for stubbed functionallity
#define TRACK_FACTORY_DELETE(track) delete *(track); *track = NULL

/// Override the macro defined in project.hh for stubbed functionallity
#define PATTERN_FACTORY_NEW(tracks) new MockPattern(NULL); tracks = tracks
/// Override the macro defined in project.hh for stubbed functionallity
#define PATTERN_FACTORY_DELETE(pattern) delete *(pattern); *pattern = NULL

#include "project.hh"

#include "mock_project.hh"


/**
 * @test Project constructor.
 *
 * Make sure that the constructor passes the name, track-, pattern- and song-
 * references to internal storage untouched.
 */
test_case(Project, Constructor) {
  MockTracks tracks;
  MockPatterns patterns;
  MockSongs songs;
  Project project("Foobar", &tracks, &patterns, &songs);
  assert_eq("Foobar", project.name);
  assert_eq(&tracks, project.tracks);
  assert_eq(&patterns, project.patterns);
  assert_eq(&songs, project.songs);
}


/**
 * @test Project setters and getters.
 *
 * Make sure that the name is possible to set or fetch, and that the tracks-,
 * patterns- and songs object references are able to be fetched correctly.
 */
test_case(Project, SettersAndGetters) {
  MockTracks tracks;
  MockPatterns patterns;
  MockSongs songs;
  Project project("Foobar", &tracks, &patterns, &songs);

  assert_setter_and_getter_twice_eq("Foo", "Bar", project, name);
  assert_eq(&tracks, project.get_tracks());
  assert_eq(&songs, project.get_songs());
  assert_eq(&patterns, project.get_patterns());
}


/**
 * @test ProjectFactory creates valid project.
 *
 * Make sure that the ProjectFactory is able to create a valid project both
 * with name, songs-, tracks- and patterns-list eference arguments and without.
 */
test_case(Project, Factory) {
  Project *project = ProjectFactoryMock::New();

  assert_eq("New project", project->name);
  assert_eq(static_cast<unsigned int>(1),
            dynamic_cast<MockSongs*>(project->songs)->size());
  assert_eq(static_cast<unsigned int>(1),
            dynamic_cast<MockPatterns*>(project->patterns)->size());
  assert_eq(static_cast<unsigned int>(1),
            dynamic_cast<MockTracks*>(project->tracks)->size());

  ProjectFactoryMock::Delete(&project);

  assert_eq(NULL, project);
}
