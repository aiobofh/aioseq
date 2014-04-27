/**
 * @file mux_test.cc
 *
 * Test-suite for the Mux class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Mux class to be able to
 * inspect the protected or private variables.
 */
#define MUX_FRIENDS                                             \
  friend_test(Mux, Constructor)                                 \
  friend_test(Mux, Register_client_shall_register_client)       \
  friend_test(Mux, Registering_a_client_twice_shall_produce_an_error_and_abort) \
  friend_test(Mux, Unregistering_a_client_that_is_not_registered_shall_produce_error_and_abort) \
  friend_test(Mux, Unregister_client_shall_unregister_client) \

#include "mux.hh"

#include "mock_sequencer.hh"
#include "mock_client.hh"


/**
 * @test Mux - Constructor
 *
 * Make sure that the sequencer reference is passed to internal storage
 * untouched.
 */
test_case(Mux, Constructor) {
  MockSequencer sequencer;
  Mux mux(&sequencer);

  assert_eq(&sequencer, mux.sequencer);
  assert_true(mux.clients.empty());
}


/**
 * @test Mux - Register client shall register client.
 *
 * Make sure that it is possible to register multiple clients in the mux.
 */
test_case(Mux, Register_client_shall_register_client) {
  MockSequencer sequencer;
  MockPatternClient pattern_client;
  MockTrackClient track_client;
  MockClient client;
  Mux mux(&sequencer);
  mux.register_client(&pattern_client);
  mux.register_client(&track_client);
  mux.register_client(&client);
  assert_eq(static_cast<unsigned int>(3), mux.clients.size());
  assert_eq(&pattern_client, mux.clients.at(0));
  assert_eq(&track_client, mux.clients.at(1));
  assert_eq(&client, mux.clients.at(2));
}


/**
 * @test Mux - Unregister client shall unregister clients.
 *
 * Make sure that it is possible to unregister a registered client in the mux.
 */
test_case(Mux, Unregister_client_shall_unregister_client) {
  MockSequencer sequencer;
  MockPatternClient pattern_client;
  MockTrackClient track_client;
  MockClient client;
  Mux mux(&sequencer);
  mux.clients.push_back(&pattern_client);
  mux.clients.push_back(&track_client);
  mux.clients.push_back(&client);
  mux.unregister_client(&pattern_client);
  // Make sure that the pattern_client is not still in the list.
  assert_eq(mux.clients.end(),
            find(mux.clients.begin(), mux.clients.end(), &pattern_client));
  // ... and that the length of the list is reduced.
  assert_eq(static_cast<unsigned int>(2), mux.clients.size());
  mux.unregister_client(&track_client);
  // Make sure that the track_client is not still in the list.
  assert_eq(mux.clients.end(),
            find(mux.clients.begin(), mux.clients.end(), &track_client));
  // ... and that the length of the list is reduced.
  assert_eq(static_cast<unsigned int>(1), mux.clients.size());
  // Make sure that the client is not still in the list.
  mux.unregister_client(&client);
  assert_eq(mux.clients.end(),
            find(mux.clients.begin(), mux.clients.end(), &client));
  // ... and that the length of the list is reduced.
  assert_eq(static_cast<unsigned int>(0), mux.clients.size());
}


/**
 * @test Mux - Registering a client twice shall produce an error and abort.
 *
 * Make sure that the same client can not be registered twice in the mux,
 * producing an error message to the standard error output.
 */
test_case(Mux, Registering_a_client_twice_shall_produce_an_error_and_abort) {
  MockSequencer sequencer;
  MockPatternClient pattern_client;
  MockClient client;
  Mux mux(&sequencer);

  mux.clients.push_back(&pattern_client);

  assert_stderr_eq("ERROR: Trying to register an already client that is "
                   "registered in MUX.\n",
                   mux.register_client(&pattern_client));

  // Make sure that it was not added anyway.
  assert_eq(static_cast<unsigned int>(1), mux.clients.size());

  mux.clients.pop_back();
}


/**
 * @test Mux - Unregistering a client that is not registered shall produce
 *             error and abort.
 *
 * Make sure that a client that is not registered can be unregistered,
 * producing an error message to the standard error output.
 */
test_case(Mux, Unregistering_a_client_that_is_not_registered_shall_produce_error_and_abort) {
  MockSequencer sequencer;
  MockClient client;
  Mux mux(&sequencer);
  //  mux.unregister_client(&client);
  assert_stderr_eq("ERROR: Trying to delete a client that is not registered "
                   "in MUX.\n",
                   mux.unregister_client(&client));
}


/**
 * @test Mux - Setters and getters shall mux to client objects of correct
 *             type.
 *
 * Make sure that a call from the sequencer to the mux is distributed to the
 * connected clients. Only clients implementing the various client interfaces
 * shall be the destination for the propagation of method calls.
 */
test_case(Mux, SettersAndGettersShallMuxToClientObjectsOfCorrectType) {
  MockPatternRow pattern_row;
  MockSequencer sequencer;
  MockPatternClient pattern_client;
  MockTrackClient track_client;
  MockPartClient part_client;
  MockClient client;
  Mux mux(&sequencer);

  mux.register_client(&pattern_client);
  mux.register_client(&track_client);
  mux.register_client(&part_client);
  mux.register_client(&client);

  expect_call_times(pattern_client, set_pattern_row_index(Eq(4)), 1);
  expect_call_times(client, set_pattern_row_index(Eq(4)), 1);

  expect_call_times(track_client, set_track_index(Eq(static_cast<unsigned int>(5))), 1);
  expect_call_times(client, set_track_index(Eq(static_cast<unsigned int>(5))), 1);

  expect_call_times(part_client, set_pattern_index(Eq(static_cast<unsigned int>(6))), 1);
  expect_call_times(client, set_pattern_index(Eq(static_cast<unsigned int>(6))), 1);

  expect_call_times_will_return(sequencer, get_row(), 1, &pattern_row);
  expect_call_times_will_return(sequencer, get_row(Eq(7)), 1, &pattern_row);

  expect_call_times(pattern_client, set_pattern_length(8), 1);
  expect_call_times(client, set_pattern_length(8), 1);

  mux.set_pattern_row_index(4);
  mux.set_track_index(5);
  mux.set_pattern_index(6);
  mux.set_pattern_length(8);

  assert_eq(&pattern_row, mux.get_row());
  assert_eq(&pattern_row, mux.get_row(7));
}
