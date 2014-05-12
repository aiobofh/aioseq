/**
 * @file pattern_client.hh
 */
#ifndef _PATTERN_CLIENT_HH_
#define _PATTERN_CLIENT_HH_

#include "client_primitive.hh"

/**
 * Pure virtual interface class for a pattern client.
 */
class PatternClientInterface : public virtual ClientPrimitiveInterface {

public:

  /**
   * Default constructor.
   */
  PatternClientInterface() {}

  /**
   * Default destructor.
   */
  virtual ~PatternClientInterface() {}

  /**
   * Set the current pattern row index to the spcified one.
   *
   * @param pattern_row_index The index to set.
   */
  virtual void set_pattern_row_index(int pattern_row_index) = 0;

  /**
   * Set the current pattern length.
   *
   * @param pattern_length Number of lines to set the pattern to.
   */
  virtual void set_pattern_length(unsigned int pattern_length) = 0;

  /**
   * Set the note key of the specified track and note index of the current
   * row.
   *
   * @param track_index Track index to modify.
   * @param note_index  Note index to modify.
   * @param key         Key value to set.
   */
  virtual void set_key(unsigned int track_index, unsigned int note_index, int key) = 0;

  /**
   * Set the note velocity of the specified track and note index of the current
   * row.
   *
   * @param track_index Track index to modify.
   * @param note_index  Note index to modify.
   * @param velocity    Key value to set.
   * @param high_nibble True if the upper 8 bits are to be set, otherwise the
   *                    lower 8 bits will be set.
   */
  virtual void set_velocity(unsigned int track_index, unsigned int note_index, int velocity, bool high_nibble) = 0;

  /**
   * Set the effect command of the specified track and note index of the
   * current row.
   *
   * @param track_index   Track index to modify.
   * @param effect_index  Effect index to modify.
   * @param command       Key value to set.
   * @param high_nibble   True if the upper 8 bits are to be set, otherwise the
   *                      lower 8 bits will be set.
   */
  virtual void set_command(unsigned int track_index, unsigned int effect_index, int command, bool high_nibble) = 0;


  /**
   * Set the effect value of the specified track and note index of the
   * current row.
   *
   * @param track_index   Track index to modify.
   * @param effect_index  Effect index to modify.
   * @param value         Key value to set.
   * @param high_nibble   True if the upper 8 bits are to be set, otherwise the
   *                      lower 8 bits will be set.
   */
  virtual void set_value(unsigned int track_index, unsigned int effect_index, int value, bool high_nibble) = 0;

};

#endif
