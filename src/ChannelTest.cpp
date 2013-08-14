/**
 * @file ChannelTest.cpp
 *
 * Test cases for Channel.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractHeaders.h"
#include "MockAbstractHeader.h"
#include "Channel.h"

/**
 * Test fixture containing fake list implementaions but
 * simplifies individual test cases by providing a correct
 * initialision of a Channel.
 */
class ChannelTest : public ::testing::Test {

protected:

  /**
   * Create a mock-up list of note headers.
   */
  MockAbstractHeaders note_headers;

  /**
   * Create a mock-up list of parameter headers.
   */
  MockAbstractHeaders parameter_headers;

  /**
   * Somewhare to store the sytem under test.
   */
  Channel *channel;


  /**
   * This is run before every test case using this fixture.
   */
  virtual void SetUp() {
    /**
     * Instantiate the system under test.
     */
    channel = new Channel(&note_headers, &parameter_headers);

    /**
     * Since the note header list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(note_headers, Die()).Times(1);

    /**
     * Since the parameter header list is a mock a call of the destructor shall
     * be expected when this test is done.
     */
    EXPECT_CALL(parameter_headers, Die()).Times(1);
  }

  /**
   * This is run after every test case using this fixture.
   */
  virtual void TearDown() {

    delete channel;

  }

};


/**
 * Test fixture containing real list implementaions but
 * simplifies individual test cases by providing a correct
 * initialision of a Channel.
 */
class ChannelTestWithRealLists : public ::testing::Test {

protected:

  /**
   * Create a mock-up list of note headers.
   */
  AbstractHeaders note_headers;

  /**
   * Create a mock-up list of parameter headers.
   */
  AbstractHeaders parameter_headers;

  /**
   * Somewhare to store the sytem under test.
   */
  Channel *channel;

  /**
   * This is run before every test case using this fixture.
   */
  virtual void SetUp() {
    /**
     * Instantiate the system under test.
     */
    channel = new Channel(&note_headers, &parameter_headers);

  }

  /**
   * This is run after every test case using this fixture.
   */
  virtual void TearDown() {

    delete channel;

  }

};


/**
 * @test Constructor with name, note and parameter headers shall construct a channel with name and note and parameter headers.
 */
TEST(Channel, Constructor_with_name_note_and_parameter_headers_shall_construct_a_channel_with_name_and_note_and_parameter_headers) {
  /**
   * Create a mock-up list of note headers.
   */
  AbstractHeaders note_headers;

  /**
   * Create a mock-up list of parameter headers.
   */
  AbstractHeaders parameter_headers;

  /**
   * Somewhare to store the sytem under test.
   */
  Channel *channel;


  /**
   * Instantiate the system under test.
   */
  channel = new Channel((string*)"Foobar", &note_headers, &parameter_headers);

  /**
   * Make sure that the new channel has a default name.
   */
  ASSERT_EQ((string)"Foobar", *(channel->get_name()));

  /**
   * Make sure that the new channel has a reference to the note headers.
   */
  ASSERT_EQ(&note_headers, channel->note_headers);

  /**
   * Make sure that the new channel has a reference to the parameter headers.
   */
  ASSERT_EQ(&parameter_headers, channel->parameter_headers);

  delete channel;
}


/**
 * @test Constructor with note and parameter headers shall construct a channel with default name and note and parameter headers.
 */
TEST_F(ChannelTest, Constructor_with_note_and_parameter_headers_shall_construct_a_channel_with_default_name_and_note_and_parameter_headers) {
  /**
   * Make sure that the new channel has a default name.
   */
  ASSERT_EQ((string)DEFAULT_CHANNEL_NAME, *(channel->get_name()));

  /**
   * Make sure that the new channel has a reference to the note headers.
   */
  ASSERT_EQ(&note_headers, channel->note_headers);

  /**
   * Make sure that the new channel has a reference to the parameter headers.
   */
  ASSERT_EQ(&parameter_headers, channel->parameter_headers);
}


/**
 * @test Get note headers shall return a poiner to a list of note headers.
 */
TEST_F(ChannelTest, Get_note_headers_shall_return_a_pointer_to_a_list_of_headers) {
  /**
   * Make sure that the tje get_note_headers() method returns a reference
   * to the note headers.
   */
  ASSERT_EQ(&note_headers, channel->get_note_headers());
}


/**
 * @test Get parameter headers shall return a poiner to a list of parameter
 *       headers.
 */
TEST_F(ChannelTest, Get_parameter_headers_shall_return_a_pointer_to_a_list_of_headers) {
  /**
   * Make sure that the the get_parameter_headers() method returns a reference
   * to the parameter headers.
   */
  ASSERT_EQ(&parameter_headers, channel->get_parameter_headers());
}


/**
 * @test Add note header shall append a note header to the end of the list of note headers.
 */
TEST_F(ChannelTestWithRealLists, Add_note_header_shall_append_a_note_header_to_the_end_of_the_list_of_note_headers) {
  /**
   * Instantiate a mock-ip of a header.
   */
  MockAbstractHeader header;

  EXPECT_CALL(header, Die()).Times(1);

  /**
   * Add the mock-ip to the list of note headers.
   */
  channel->add_note_header(&header);

  /**
   * Get the note header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_note_headers();

  /**
   * Make sure that there is one note header in the list.
   */
  ASSERT_EQ(1, headers_got->size());

  /**
   * Make sure that the one item is exactly the same header as the one that was
   * provided.
   */
  ASSERT_EQ(&header, headers_got->front());
}


/**
 * @test Adding several note header shall append several note headers to the end of the list of note headers.
 */
TEST_F(ChannelTestWithRealLists, Adding_several_note_headers_shall_append_several_note_headers_to_the_end_of_the_list_of_note_headers) {
  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the mock-ups to the list of note headers.
   */
  channel->add_note_header(&header1);
  channel->add_note_header(&header2);
  channel->add_note_header(&header3);

  /**
   * Get the note header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_note_headers();

  /**
   * Make sure that there is one note header in the list.
   */
  ASSERT_EQ(3, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header1, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}


/**
 * @test Inserting a note header shall insert a note header in the channel.
 */
TEST_F(ChannelTestWithRealLists, Inserting_a_note_header_shall_insert_a_note_header_in_the_channel) {

  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of note headers.
   */
  channel->add_note_header(&header1);
  channel->add_note_header(&header3);
  channel->insert_note_header(&header1, &header2);

  /**
   * Get the note header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_note_headers();

  /**
   * Make sure that there is one note header in the list.
   */
  ASSERT_EQ(3, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header2, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}


/**
 * @test Inserting a note header before an unknown note header shall append a note header to the channel.
 */
TEST_F(ChannelTestWithRealLists, Inserting_a_note_header_before_and_unknown_note_header_shall_append_a_note_header_to_the_channel) {

  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of note headers.
   */
  channel->add_note_header(&header1);
  channel->insert_note_header(&header3, &header2);

  /**
   * Get the note header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_note_headers();

  /**
   * Make sure that there is one note header in the list.
   */
  ASSERT_EQ(2, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header1, headers_got->front());
  ASSERT_EQ(&header2, headers_got->back());
}


/**
 * @test Deleting a note header shall delete a note header from the channel.
 */
TEST_F(ChannelTestWithRealLists, Deleting_a_note_header_shall_delete_a_note_header_from_the_channel) {
  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of note headers.
   */
  channel->add_note_header(&header1);
  channel->add_note_header(&header2);
  channel->add_note_header(&header3);

  channel->delete_note_header(&header1);

  /**
   * Get the note header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_note_headers();

  /**
   * Make sure that there is one note header in the list.
   */
  ASSERT_EQ(2, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header2, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}

/**
 * @test Add parameter header shall append a parameter header to the end of the list of parameter headers.
 */
TEST_F(ChannelTestWithRealLists, Add_parameter_header_shall_append_a_parameter_header_to_the_end_of_the_list_of_parameter_headers) {
  /**
   * Instantiate a mock-ip of a header.
   */
  MockAbstractHeader header;

  EXPECT_CALL(header, Die()).Times(1);

  /**
   * Add the mock-ip to the list of parameter headers.
   */
  channel->add_parameter_header(&header);

  /**
   * Get the parameter header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_parameter_headers();

  /**
   * Make sure that there is one parameter header in the list.
   */
  ASSERT_EQ(1, headers_got->size());

  /**
   * Make sure that the one item is exactly the same header as the one that was
   * provided.
   */
  ASSERT_EQ(&header, headers_got->front());
}


/**
 * @test Adding several parameter header shall append several parameter headers to the end of the list of parameter headers.
 */
TEST_F(ChannelTestWithRealLists, Adding_several_parameter_headers_shall_append_several_parameter_headers_to_the_end_of_the_list_of_parameter_headers) {
  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the mock-ups to the list of parameter headers.
   */
  channel->add_parameter_header(&header1);
  channel->add_parameter_header(&header2);
  channel->add_parameter_header(&header3);

  /**
   * Get the parameter header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_parameter_headers();

  /**
   * Make sure that there is one parameter header in the list.
   */
  ASSERT_EQ(3, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header1, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}


/**
 * @test Inserting a parameter header shall insert a parameter header in the channel.
 */
TEST_F(ChannelTestWithRealLists, Inserting_a_parameter_header_shall_insert_a_parameter_header_in_the_channel) {

  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of parameter headers.
   */
  channel->add_parameter_header(&header1);
  channel->add_parameter_header(&header3);
  channel->insert_parameter_header(&header1, &header2);

  /**
   * Get the parameter header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_parameter_headers();

  /**
   * Make sure that there is one parameter header in the list.
   */
  ASSERT_EQ(3, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header2, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}


/**
 * @test Inserting a parameter header before an unknown parameter header shall append a parameter header to the channel.
 */
TEST_F(ChannelTestWithRealLists, Inserting_a_parameter_header_before_and_unknown_parameter_header_shall_append_a_parameter_header_to_the_channel) {

  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of parameter headers.
   */
  channel->add_parameter_header(&header1);
  channel->insert_parameter_header(&header3, &header2);

  /**
   * Get the parameter header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_parameter_headers();

  /**
   * Make sure that there is one parameter header in the list.
   */
  ASSERT_EQ(2, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header1, headers_got->front());
  ASSERT_EQ(&header2, headers_got->back());
}


/**
 * @test Deleting a parameter header shall delete a parameter header from the channel.
 */
TEST_F(ChannelTestWithRealLists, Deleting_a_parameter_header_shall_delete_a_parameter_header_from_the_channel) {
  /**
   * Instantiate a mock-ups of a headers.
   */
  MockAbstractHeader header1;
  MockAbstractHeader header2;
  MockAbstractHeader header3;

  EXPECT_CALL(header1, Die()).Times(1);
  EXPECT_CALL(header2, Die()).Times(1);
  EXPECT_CALL(header3, Die()).Times(1);

  /**
   * Add the two of the mock-ups to the list of parameter headers.
   */
  channel->add_parameter_header(&header1);
  channel->add_parameter_header(&header2);
  channel->add_parameter_header(&header3);

  channel->delete_parameter_header(&header1);

  /**
   * Get the parameter header list from of channel.
   */
  AbstractHeaders *headers_got = channel->get_parameter_headers();

  /**
   * Make sure that there is one parameter header in the list.
   */
  ASSERT_EQ(2, headers_got->size());

  /**
   * Make sure that the one item is exactly the same headers as the ones that
   * was provided.
   */
  ASSERT_EQ(&header2, headers_got->front());
  ASSERT_EQ(&header3, headers_got->back());
}
