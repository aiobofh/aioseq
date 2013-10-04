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
#include "MockAbstractMidiInputs.h"
#include "MockAbstractMidiOutputs.h"
#include "MockAbstractMidiDevices.h"

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
   * Create a mock-up list of MIDI outputs.
   */
  MockAbstractMidiOutputs midi_outputs;

  /**
   * Create a mock-up list of MIDI inputs.
   */
  MockAbstractMidiInputs midi_inputs;

  /**
   * Create a mock-up list of MIDI devices.
   */
  MockAbstractMidiDevices midi_devices;

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
    studio = new Studio(&instruments,
                        &audio_outputs,
                        &audio_inputs,
                        &midi_outputs,
                        &midi_inputs,
                        &midi_devices);

    /**
     * Since the instrument list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(instruments, Die()).Times(1);

    /**
     * Since the audio outputs list is a mock a call of the destructor shall
     * be expected when this test is done.
     */
    EXPECT_CALL(audio_outputs, Die()).Times(1);

    /**
     * Since the audio inputs list is a mock a call of the destructor shall
     * be expected when this test is done.
     */
    EXPECT_CALL(audio_inputs, Die()).Times(1);

    /**
     * Since the MIDI outputs list is a mock a call of the destructor shall
     * be expected when this test is done.
     */
    EXPECT_CALL(midi_outputs, Die()).Times(1);

    /**
     * Since the MIDI inputs list is a mock a call of the destructor shall be
     * expected when this test is done.
     */
    EXPECT_CALL(midi_inputs, Die()).Times(1);

    /**
     * Since the MIDI devices list is a mcok a call of the destructor shall
     * be expected whn this test is done.
     */
    EXPECT_CALL(midi_devices, Die()).Times(1);
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

  MockAbstractInstruments instruments;
  MockAbstractAudioOutputs audio_outputs;
  MockAbstractAudioInputs audio_inputs;
  MockAbstractMidiOutputs midi_outputs;
  MockAbstractMidiInputs midi_inputs;
  MockAbstractMidiDevices midi_devices;

  Studio *studio;

  studio = new Studio((string*)"Foobar",
                      &instruments,
                      &audio_outputs,
                      &audio_inputs,
                      &midi_outputs,
                      &midi_inputs,
                      &midi_devices);

  ASSERT_EQ((string)"Foobar", *(studio->get_name()));

  ASSERT_EQ(&instruments, studio->instruments);
  ASSERT_EQ(&audio_outputs, studio->audio_outputs);
  ASSERT_EQ(&audio_inputs, studio->audio_inputs);
  ASSERT_EQ(&midi_outputs, studio->midi_outputs);
  ASSERT_EQ(&midi_inputs, studio->midi_inputs);
  ASSERT_EQ(&midi_devices, studio->midi_devices);

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

  /**
   * Since the MIDI outputs list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(midi_outputs, Die()).Times(1);

  /**
   * Since the MIDI inputs list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(midi_inputs, Die()).Times(1);

  /**
   * Since the MIDI devices list is a mcok a call of the destructor shall
   * be expected whn this test is done.
   */
  EXPECT_CALL(midi_devices, Die()).Times(1);

  delete studio;
}


/**
 * @test Constructor without name instruments audio outputs and inputs shall
 *       construct a studio with default name audio outputs and inputs.
 */
TEST(Studio, Constructor_without_name_instruments_audio_outputs_and_inputs_shall_construct_a_studio_with_default_name_audio_outputs_and_inputs) {

  MockAbstractInstruments instruments;
  MockAbstractAudioOutputs audio_outputs;
  MockAbstractAudioInputs audio_inputs;
  MockAbstractMidiOutputs midi_outputs;
  MockAbstractMidiInputs midi_inputs;
  MockAbstractMidiDevices midi_devices;

  Studio *studio;

  studio = new Studio(&instruments,
                      &audio_outputs,
                      &audio_inputs,
                      &midi_outputs,
                      &midi_inputs,
                      &midi_devices);

  ASSERT_EQ((string)DEFAULT_STUDIO_NAME, *(studio->get_name()));

  ASSERT_EQ(&instruments, studio->instruments);
  ASSERT_EQ(&audio_outputs, studio->audio_outputs);
  ASSERT_EQ(&audio_inputs, studio->audio_inputs);
  ASSERT_EQ(&midi_outputs, studio->midi_outputs);
  ASSERT_EQ(&midi_inputs, studio->midi_inputs);
  ASSERT_EQ(&midi_devices, studio->midi_devices);

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

  /**
   * Since the MIDI outputs list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(midi_outputs, Die()).Times(1);

  /**
   * Since the MIDI inputs list is a mock a call of the destructor shall be
   * expected when this test is done.
   */
  EXPECT_CALL(midi_inputs, Die()).Times(1);

  /**
   * Since the MIDI devices list is a mcok a call of the destructor shall
   * be expected whn this test is done.
   */
  EXPECT_CALL(midi_devices, Die()).Times(1);

  delete studio;
}
