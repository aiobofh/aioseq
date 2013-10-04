/**
 * @file MidiDeviceTest.cpp
 *
 * Test cases for MidiDevice
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockAbstractMidiInput.h"
#include "MockAbstractMidiOutput.h"
#include "MockAbstractAudioInputs.h"
#include "MockAbstractAudioOutputs.h"

#include "MidiDevice.h"

using namespace std;


/**
 * @test Constructor without arguments shall create a MIDI device with a
 *       default name and no inputs or outputs.
 */
TEST(MidiDevice, Constructor_without_arguments_shall_create_a_MIDI_device_with_a_default_name_and_no_inputs_or_outputs) {
  /**
   * Constrcut a new MIDI device without any arguments.
   */
  MidiDevice midi_device;
  /**
   * Make sure that the new MIDI device has a default name.
   */
  ASSERT_EQ((string)DEFAULT_MIDI_DEVICE_NAME, *(midi_device.get_name()));
  /**
   * Make sure that the MIDI device has no MIDI input.
   */
  ASSERT_EQ(NULL, midi_device.midi_input);
  /**
   * Make sure that the MIDI device has no MIDI output.
   */
  ASSERT_EQ(NULL, midi_device.midi_output);
  /**
   * Make sure that the MIDI device has no audio inputs.
   */
  ASSERT_EQ(NULL, midi_device.audio_inputs);
  /**
   * Make sure that the MIDI device has no audio outputs.
   */
  ASSERT_EQ(NULL, midi_device.audio_outputs);
}


/**
 * @test Constrcutor with arguments shall create a MIDI device with the
 *       correct contents.
 */
TEST(MidiDevice, Constructor_with_arguments_shall_create_a_MIDI_device_with_the_correct_contents) {
  /**
   * Create a mock-up of a MIDI input.
   */
  MockAbstractMidiInput midi_input;

  /**
   * Create a mock-up of a MIDI output.
   */
  MockAbstractMidiOutput midi_output;

  /**
   * Create a mock-up of an audio inputs list.
   */
  MockAbstractAudioInputs audio_inputs;

  /**
   * Create a mock-up of an audio outputs list.
   */
  MockAbstractAudioOutputs audio_outputs;

  /**
   * Constrcut a new MIDI device with a name and the mock-ups for inputs and
   * outputs.
   */
  MidiDevice midi_device((const string*)"Foobar",
                         &midi_input,
                         &midi_output,
                         &audio_inputs,
                         &audio_outputs);
  /**
   * Exepect calls to destructors of the mocks.
   */
  EXPECT_CALL(midi_input, Die()).Times(1);
  EXPECT_CALL(midi_output, Die()).Times(1);
  EXPECT_CALL(audio_inputs, Die()).Times(1);
  EXPECT_CALL(audio_outputs, Die()).Times(1);
  /**
   * Make sure that the new MIDI device has a default name.
   */
  ASSERT_EQ((string)"Foobar", *(midi_device.get_name()));
  /**
   * Make sure that the MIDI device has no MIDI input.
   */
  ASSERT_EQ(&midi_input, midi_device.midi_input);
  /**
   * Make sure that the MIDI device has no MIDI output.
   */
  ASSERT_EQ(&midi_output, midi_device.midi_output);
  /**
   * Make sure that the MIDI device has no audio inputs.
   */
  ASSERT_EQ(&audio_inputs, midi_device.audio_inputs);
  /**
   * Make sure that the MIDI device has no audio outputs.
   */
  ASSERT_EQ(&audio_outputs, midi_device.audio_outputs);

}

