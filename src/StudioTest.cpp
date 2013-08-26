/**
 * @file StudioTest.cpp
 *
 * Test cases for Studio.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractInstruments.h"
#include "MockAbstractAudioInputs.h"
#include "MockAbstractAudioOutputs.h"

#include "Studio.h"

/**
 * Test fixture containing fake list implementations but
 * simplifies individual test cases by providing a correct
 * initalisation of a Studio.
 */
class StudioTest : public ::testing::Test {

protected:

  /**
   * Create a mock-up list of instruments.
   */
  MockAbstractInstruments instruments;

  /**
   * Create a mock-up list of audio outputs.
   */
  MockAbstractAudioOutputs audio_outputs;

  /**
   * Create a mock-up list of audio inputs.
   */
  MockAbstractAudioInputs audio_inputs;

  /**
   * Somewhere to store the system under test.
   */
  Studio *studio;

  /**
   * This is run before every test case using this fixture.
   */
  virtual void SetUp() {
    /**
     * Instantiate the system under test.
     */
    studio = new Studio(&instruments, &audio_outputs, &audio_inputs);

    /**
     * Since the instrument list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(instruments, Die()).Times(1);

    /**
     * Since the audio outputs list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(audio_outputs, Die()).Times(1);

    /**
     * Since the audio inputs list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(audio_inputs, Die()).Times(1);
  }

  /**
   * This is run after every test case using this fixture.
   */
  virtual void TearDown() {

    delete studio;

  }

};


/**
 * @test Constructor with name instruments audio outputs and inputs shall
 *       construct a studio with name audio outputs and inputs.
 */
TEST(Studio, Constructor_with_name_instruments_audio_outputs_and_inputs_shall_construct_a_studio_with_name_audio_outputs_and_inputs) {

  AbstractInstruments instruments;
  AbstractAudioOutputs audio_outputs;
  AbstractAudioInputs audio_inputs;

  Studio *studio;

  studio = new Studio((string*)"Foobar", &instruments, &audio_outputs, &audio_inputs);

  ASSERT_EQ((string)"Foobar", *(studio->get_name()));

  ASSERT_EQ(&instruments, studio->instruments);
  ASSERT_EQ(&audio_outputs, studio->audio_outputs);
  ASSERT_EQ(&audio_inputs, studio->audio_inputs);

  delete studio;
}


/**
 * @test Constructor without name instruments audio outputs and inputs shall
 *       construct a studio with default name audio outputs and inputs.
 */
TEST(Studio, Constructor_without_name_instruments_audio_outputs_and_inputs_shall_construct_a_studio_with_default_name_audio_outputs_and_inputs) {

  AbstractInstruments instruments;
  AbstractAudioOutputs audio_outputs;
  AbstractAudioInputs audio_inputs;

  Studio *studio;

  studio = new Studio(&instruments, &audio_outputs, &audio_inputs);

  ASSERT_EQ((string)DEFAULT_STUDIO_NAME, *(studio->get_name()));

  ASSERT_EQ(&instruments, studio->instruments);
  ASSERT_EQ(&audio_outputs, studio->audio_outputs);
  ASSERT_EQ(&audio_inputs, studio->audio_inputs);

  delete studio;
}
