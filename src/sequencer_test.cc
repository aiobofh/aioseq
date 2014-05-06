/**
 * @file sequencer_test.cc
 *
 * Test-suite for the Sequencer class.
 *
 */
#include "test.hh"

class SequencerPatternLength;

/**
 * Make all relevant test cases friends with the Sequencer class to be able to
 * inspect the protected and private variables.
 */
#define SEQUENCER_FRIENDS                                               \
  friend_test(Sequencer, Constructor)                                   \
  friend_test(Sequencer, Getters)                                       \
  friend_test(Sequencer, Register_a_single_client_shall_not_produce_errors) \
  friend_test(Sequencer, Registering_multiple_clients_shall_produce_errors) \
  friend_test(Sequencer, Un_registering_the_client_shall_not_produce_errors) \
  friend_test(Sequencer, Un_registering_when_no_client_is_registered_shall_produce_error) \
  friend_test(Sequencer, Un_registering_wrong_client_shall_produce_error) \
  friend_test(Sequencer, Setting_track_index_shall_set_track_index) \
  friend_test(Sequencer, Setting_track_index_out_of_bounds_shall_output_error) \
  friend_test(Sequencer, Setting_track_index_shall_call_method_in_registered_client) \
  friend_test(Sequencer, Re_setting_same_track_index_shall_not_call_client) \
  friend_test(Sequencer, Setting_pattern_index_shall_set_pattern_index) \
  friend_test(Sequencer, Setting_pattern_row_index_shall_set_pattern_row_index) \
  friend_test(Sequencer, Setting_pattern_row_index_out_of_bounds_shall_wrap_around_to_the_other_end_of_the_pattern) \
  friend_test(Sequencer, Re_setting_same_pattern_row_index_shall_not_call_client) \
  friend_test(Sequencer, Setting_pattern_index_out_of_bounds_shall_output_error) \
  friend_test(Sequencer, Setting_pattern_index_shall_call_method_in_registered_client_and_update_its_pattern_representation) \
  friend_test(Sequencer, Re_setting_same_pattern_index_shall_not_call_client) \
  friend_test(Sequencer, Setting_pattern_length_shall_shorten_or_extend_the_list_of_pattern_rows) \
  friend_test(Sequencer, Re_setting_same_pattern_length_shall_not_call_client) \
  friend_test(Sequencer, Setting_pattern_length_out_of_bounds_shall_output_and_error_and_leave_the_length_unchanged)

#include "mock_pattern_row.hh"

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_ROW_FACTORY_NEW(tracks) \
  new MockPatternRow()

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_ROW_FACTORY_DELETE(pattern_row) delete *(pattern_row); \
  *pattern_row = NULL

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_FACTORY_ADD_ROWS(tracks, pattern, pattern_rows_to_add) \
  pattern_length = pattern_length

/// Override the macro defined in pattern.hh for stubbed functionallity
#define PATTERN_FACTORY_REMOVE_ROWS(pattern, pattern_rows_to_remove) \
  pattern_length = pattern_length

#include "mock_client_primitive.hh"
#include "mock_project.hh"
#include "mock_pattern_row.hh"
#include "mock_pattern.hh"
#include "mock_track_entry.hh"
#include "mock_track.hh"
#include "mock_part.hh"
#include "mock_song.hh"
#include "mock_note.hh"
#include "mock_effect.hh"
#include "mock_client.hh"
#include "mock_pattern_client.hh"
#include "mock_track_client.hh"

#include "sequencer.hh"

#include "mock_sequencer.hh"

/**
 * @test Sequencer - Constructor
 *
 * Make sure that the constructor passes the project reference to internal
 * storage untouched and set-up default pattern, row and track indices.
 */
test_case(Sequencer, Constructor) {
  MockProject project;
  Sequencer sequencer(&project);

  assert_eq(&project, sequencer.project);
  assert_eq(NULL, sequencer.client);
  assert_eq(static_cast<unsigned int>(0), sequencer.track_index);
  assert_eq(static_cast<unsigned int>(0), sequencer.pattern_row_index);
  assert_eq(static_cast<unsigned int>(0), sequencer.pattern_index);
}


/**
 * @test Sequencer - Getters
 *
 * Make sure that all getters are getting the correct internal content.
 */
test_case(Sequencer, Getters) {
  MockPatternRow row;
  MockPatternRow row_hitme;
  MockPatternRows rows;
  MockPattern pattern;
  MockPatterns patterns;
  MockTrack track;
  MockTracks tracks;
  MockProject project;
  SequencerTemplate<MockProject,
                    MockTrack,
                    MockTracks,
                    MockPattern,
                    MockPatterns,
                    MockPatternFactory,
                    MockPatternRow,
                    MockPatternRows,
                    MockPatternRowFactory,
                    MockTrackEntries,
                    MockTrackEntry,
                    MockNotes,
                    MockNote> sequencer(&project);

  sequencer.pattern_row_index = 2;

  patterns.push_back(&pattern);
  patterns.push_back(&pattern);
  patterns.push_back(&pattern);

  tracks.push_back(&track);
  tracks.push_back(&track);
  tracks.push_back(&track);
  tracks.push_back(&track);

  rows.push_back(&row);
  rows.push_back(&row);
  rows.push_back(&row_hitme);
  rows.push_back(&row);
  rows.push_back(&row);

  expect_call_times_will_return(project, get_patterns(), 5, &patterns);
  expect_call_times_will_return(project, get_tracks(), 1, &tracks);
  expect_call_times_will_return(pattern, get_pattern_rows(), 4, &rows);

  assert_eq(3, sequencer.get_pattern_count());
  assert_eq(4, sequencer.get_track_count());
  assert_eq(5, sequencer.get_pattern_row_count());

  assert_eq(&row, sequencer.get_row(0));
  assert_eq(&row_hitme, sequencer.get_row(2));

  assert_eq(&row_hitme, sequencer.get_row());
}

/**
 * @test Seqyencer - Register a single client shall not produce errors.
 *
 * Make sure that it is possible to register a client in the sequencer and
 * that no text is outputed to the standard error output.
 */
test_case(Sequencer, Register_a_single_client_shall_not_produce_errors) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 10);

  /*
   * Make sure that the client is called for setting its pattern length.
   */
  expect_call_times(client, set_pattern_length(Eq(static_cast<unsigned int>(10))), 1);
  assert_stderr_eq("", sequencer.Sequencer::register_client(&client));
  assert_eq(&client, sequencer.Sequencer::client);
}


/**
 * @test Sequencer - Registering multiple clients shall produce errors.
 *
 * Make sure that errors are produced when trying to register more than one
 * client and that the internal storage of the reference to the lcient is
 * unchanged.
 */
test_case(Sequencer, Registering_multiple_clients_shall_produce_errors) {
  MockClient client1;
  MockClient client2;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 10);
  /*
   * Make sure that the client is called for setting its pattern length.
   */
  expect_call_times(client1, set_pattern_length(Eq(static_cast<unsigned int>(10))), 1);

  // Register client1

  sequencer.Sequencer::register_client(&client1);

  // Try to register client1

  assert_stderr_eq("ERROR: Trying to register a client where one is already "
                   "registered.\n",
                   sequencer.Sequencer::register_client(&client2));

  // Internal storage shall still refer to client1

  assert_eq(&client1, sequencer.Sequencer::client);
}


/**
 * @test Sequencer - Un-registering the client shall not produce errors.
 *
 * Make sure that it is possible to unregister the registerd client without
 * any output to the standard error output and the internally stored reference
 * shall be NULL.
 */
test_case(Sequencer, Un_registering_the_client_shall_not_produce_errors) {
  MockClient client;
  MockProject project;
  Sequencer sequencer(&project);

  // Set-up internal state referencing the client.

  sequencer.client = &client;

  // Unregister client.

  assert_stderr_eq("", sequencer.unregister_client(&client));

  assert_eq(NULL, sequencer.client);
}


/**
 * @test Sequencer - Un-registering when no client is registered shall
 *                   produce error.
 *
 * Make sure that an error is produced when truing to unregister a client if
 * no client is registered in internal storage, and that the storage remains
 * NULL.
 */
test_case(Sequencer,
          Un_registering_when_no_client_is_registered_shall_produce_error) {
  MockClient client;
  MockProject project;
  Sequencer sequencer(&project);

  // Try to un-register the client without having it registered in internally.

  assert_stderr_eq("ERROR: Can not unregister client, none registered.\n",
                   sequencer.unregister_client(&client));

  assert_eq(NULL, sequencer.client);
}


/**
 * @test Sequencer - Un-registering wrong client shall produce error.
 *
 * Make sure that an error is produced when trying to un-register a client
 * that is not the registed one, and that the reference stored internally
 * remains unchanged.
 */
test_case(Sequencer, Un_registering_wrong_client_shall_produce_error) {
  MockClient client1;
  MockClient client2;
  MockProject project;
  Sequencer sequencer(&project);

  sequencer.client = &client1;

  assert_stderr_eq("ERROR: Trying to unregister wrong client.\n",
                   sequencer.unregister_client(&client2));

  assert_eq(&client1, sequencer.client);
}

// ------------------------- TrackClientInterface ---------------------------


/**
 * @test Sequencer - Setting track index shall set track index.
 *
 * Make sure that the track index can be set.
 */
test_case(Sequencer, Setting_track_index_shall_set_track_index) {
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_track_count(), 1, 7);

  sequencer.Sequencer::set_track_index(static_cast<unsigned int>(1));
  assert_eq(static_cast<unsigned int>(1), sequencer.Sequencer::track_index);
}


/**
 * @test Seqeuncer - Setting track index out of bounds shall output error.
 *
 * Make sure that error messages are outputed to standard error output if
 * index is out of bounds. Also make sure that the index is set to the limit
 * value itself.
 */
test_case(Sequencer, Setting_track_index_out_of_bounds_shall_output_error)
{
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_track_count(), 1, 7);

  assert_stderr_eq("ERROR: "
                   "Track index 8 is invalid. Setting track index to 7\n",
                   sequencer.Sequencer::set_track_index(8));
  assert_eq(static_cast<unsigned int>(7), sequencer.Sequencer::track_index);
}


/**
 * @test Sequencer - Setting track index shall call method in registered
 *                   client.
 *
 * Make sure that the registered client is informed when a new track index is
 * set.
 */
test_case(Sequencer,
          Setting_track_index_shall_call_method_in_registered_client) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_track_count(), 1, 7);
  expect_call_times(client, set_track_index(Eq(static_cast<unsigned int>(3))), 1);

  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_track_index(3);
}


/**
 * @test Sequencer - Re-setting same track index shall not call client.
 *
 * Make sure that the registered client is NOT informed when the track index
 * is set to what ever it already was set to.
 */
test_case(Sequencer, Re_setting_same_track_index_shall_not_call_client) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_track_count(), 1, 7);
  expect_call_times(client, set_track_index(Eq(static_cast<unsigned int>(3))), 0);

  sequencer.Sequencer::track_index = 3;
  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_track_index(3);
}


// -------------------------- PartClientInterface ---------------------------

/**
 * @test Sequencer - Setting pattern index shall set pattern index.
 *
 * Make sure that the track index can be set.
 */
test_case(Sequencer, Setting_pattern_index_shall_set_pattern_index) {
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_count(), 1, 7);

  sequencer.Sequencer::set_pattern_index(1);
  assert_eq(static_cast<unsigned int>(1), sequencer.Sequencer::pattern_index);
}


/**
 * @test Seqeuncer - Setting pattern index out of bounds shall output error.
 *
 * Make sure that error messages are outputed to standard error output if
 * index is out of bounds. Also make sure that the index is set to the limit
 * value itself.
 */
test_case(Sequencer, Setting_pattern_index_out_of_bounds_shall_output_error)
{
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_count(), 1, 7);

  assert_stderr_eq("ERROR: "
                   "Pattern index 8 is invalid. Setting pattern index to 7\n",
                   sequencer.Sequencer::set_pattern_index(8));
  assert_eq(static_cast<unsigned int>(7), sequencer.Sequencer::pattern_index);
}


/**
 * @test Sequencer - Setting pattern index shall call method in registered
 *                   client and update its pattern representation
 *
 * Make sure that the registered client is informed when a new pattern index
 * is set.
 */
test_case(Sequencer,
          Setting_pattern_index_shall_call_method_in_registered_client_and_update_its_pattern_representation) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_count(), 1, 7);
  expect_call_times(client, set_pattern_index(Eq(static_cast<unsigned int>(3))), 1);

  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_pattern_index(3);
}


/**
 * @test Sequencer - Re-setting same pattern index shall not call client.
 *
 * Make sure that the registered client is NOT informed when the pattern index
 * is set to what ever it already was set to.
 */
test_case(Sequencer, Re_setting_same_pattern_index_shall_not_call_client) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_count(), 1, 7);
  expect_call_times(client, set_pattern_index(Eq(static_cast<unsigned int>(3))), 0);

  sequencer.Sequencer::pattern_index = 3;
  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_pattern_index(3);
}

// ----------------------- PatternClientInterface -------------------------

/**
 * @test Sequencer - Setting pattern row index shall set pattern row index
 *                   and inform all clients about it.
 *
 * Make sure that the pattern row index can be set and that the corresponding
 * method is called in all clients.
 */
test_case(Sequencer, Setting_pattern_row_index_shall_set_pattern_row_index) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 7);
  expect_call_times(client, set_pattern_row_index(Eq(1)), 1);

  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_pattern_row_index(1);
  assert_eq(static_cast<unsigned int>(1), sequencer.Sequencer::pattern_row_index);
}


/**
 * @test Seqeuncer - Setting pattern row index out of bounds shall wrap
 *                   around to the other end of the pattern.
 *
 * Make sure that the pattern row index is wrapped around to the other end
 * of the pattern.
 */
test_case(Sequencer, Setting_pattern_row_index_out_of_bounds_shall_wrap_around_to_the_other_end_of_the_pattern)
{
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 2, 7);

  sequencer.Sequencer::set_pattern_row_index(7);
  assert_eq(static_cast<unsigned int>(0), sequencer.Sequencer::pattern_row_index);

  sequencer.Sequencer::set_pattern_row_index(-1);
  assert_eq(static_cast<unsigned int>(6), sequencer.Sequencer::pattern_row_index);
}


/**
 * @test Sequencer - Re-setting same pattern_row index shall not call client.
 *
 * Make sure that the registered client is NOT informed when the pattern index
 * is set to what ever it already was set to.
 */
test_case(Sequencer, Re_setting_same_pattern_row_index_shall_not_call_client) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 7);
  expect_call_times(client, set_pattern_row_index(Eq(3)), 0);

  sequencer.Sequencer::pattern_row_index = 3;
  sequencer.Sequencer::client = &client;

  sequencer.Sequencer::set_pattern_row_index(3);
}

/**
 * A test-version of the sequencer, but with only one mocked method.
 *
 * This is needed in the
 * Setting_pattern_length_shall_shorten_or_extend_the_list_of_pattern_rows
 * test.
 */
class SequencerSemiMock : public SequencerTemplate<MockProject,
                                                   MockTrack,
                                                   MockTracks,
                                                   MockPattern,
                                                   MockPatterns,
                                                   MockPatternFactory,
                                                   MockPatternRow,
                                                   MockPatternRows,
                                                   PatternRowFactoryMock,
                                                   MockTrackEntries,
                                                   MockTrackEntry,
                                                   MockNotes,
                                                   MockNote> {
public:
  /**
   * Default constructor propagating argument to the SequencerTemplate
   * constructor.
   *
   * @param project A reference to the project to use for testing.
   */
  SequencerSemiMock(ProjectInterface *project) : SequencerTemplate(project){
  }

  /**
   * Mocked version of the get_pattern_row_count method to be controlled
   * from within the tests.
   */
  MOCK_METHOD0(get_pattern_row_count,
               int());
};

/**
 * @test Sequencer - Setting pattern length to less shall shorten the list of
 *                   pattern rows.
 *
 * Make sure that it is possible to make a pattern longer or shorter, and that
 * the clients are informed about the change.
 */
test_case(Sequencer, Setting_pattern_length_shall_shorten_or_extend_the_list_of_pattern_rows) {
  MockClient client;
  MockProject project;
  MockPattern pattern;
  MockPatterns patterns;
  MockTracks tracks;

  patterns.push_back(&pattern);

  expect_call_times_will_return(project, get_tracks(), 2, &tracks);
  expect_call_times_will_return(project, get_patterns(), 2, &patterns);

  SequencerSemiMock sequencer(&project);

  sequencer.client = &client;
  sequencer.pattern_index = 0;

  // Make the pattern longer.
  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 7);
  expect_call_times(client, set_pattern_length(Eq(static_cast<unsigned int>(10))), 1);

  sequencer.SequencerTemplate::set_pattern_length(static_cast<unsigned int>(10));

  // And shorter.
  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 10);
  expect_call_times(client, set_pattern_length(Eq(static_cast<unsigned int>(7))), 1);

  sequencer.SequencerTemplate::set_pattern_length(static_cast<unsigned int>(7));
}


/**
 * @test Sequencer - Re-setting same pattern length shall not call client.
 *
 * Make sure that the registered client is NOT informed when the pattern
 * length is set to what ever it already was set to.
 */
test_case(Sequencer, Re_setting_same_pattern_length_shall_not_call_client) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  sequencer.client = &client;

  // Make the pattern the same length as before.
  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 7);
  // The client shall not be informed about the change, since none happened.
  expect_call_times(client, set_pattern_length(_), 0);

  sequencer.Sequencer::set_pattern_length(7);
}

/**
 * @test Sequencer - Setting pattern length out of bounds shall output an
 *                   error and leave the length unchanged.
 *
 * Make sure that an error is produced for the user if the pattern length is
 * set to an invalid length and the clients shall NOT be informed about the
 * change, since none occured.
 */
test_case(Sequencer, Setting_pattern_length_out_of_bounds_shall_output_and_error_and_leave_the_length_unchanged) {
  MockClient client;
  MockProject project;
  FakeSequencer sequencer(&project);

  sequencer.client = &client;

  expect_call_times_will_return(sequencer, get_pattern_row_count(), 1, 7);
  expect_call_times(client, set_pattern_length(_), 0);

  // Make the pattern too long.
  assert_stderr_eq("ERROR: Pattern length more than 255 is not permitted.\n",
                   sequencer.Sequencer::set_pattern_length(256));
}
