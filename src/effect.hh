/**
 * @file effect.hh
 *
 * All effect-related class declarations.
 *
 */
#ifndef _EFFECT_HH_
#define _EFFECT_HH_

#ifndef EFFECT_FRIENDS
/// No effect friends if none are available.
#define EFFECT_FRIENDS
#endif

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "vector_of.hh"


/**
 * Pure virtual interface class for an effect.
 */
class EffectInterface {

public:

  /**
   * Default constructor.
   */
  EffectInterface() {};

  /**
   * Constructor taking a command and its value as arguments.
   *
   * @param command Command of the event.
   * @param value Value to the command.
   */
  EffectInterface(int command, int value) {};

  /**
   * Default destructor.
   */
  virtual ~EffectInterface() {};

  /**
   * Get the command number of the effect.
   *
   * @return A numeric value representing a command.
   */
  virtual int get_command() const = 0;

  /**
   * Set the command number of the effect.
   *
   * @param command A numeric value representing a command.
   */
  virtual void set_command(int command) = 0;

  /**
   * Get the value in a command of the effect.
   *
   * @return A numeric value parameter to the command.
   */
  virtual int get_value() const = 0;

  /**
   * Set the value in a command of the effect.
   *
   * @param value A numeric value parameter to the command.
   */
  virtual void set_value(int value) = 0;

};

/**
 * This Effect class represent an effect event with a command number and its
 * value.
 */
class Effect : virtual public EffectInterface {

  EFFECT_FRIENDS

protected:

  /// Internal storage of the command of the effect.
  int command;
  /// Internal storage of the value to the command.
  int value;

public:

  /// @copydoc EffectInterface::EffectInterface(int,int)
  Effect(int command = 0, int value = 0);
  /// @copydoc EffectInterface::~EffectInterface()
  ~Effect();
  /// @copydoc EffectInterface::get_command()
  virtual int get_command() const;
  /// @copydoc EffectInterface::set_command(int)
  virtual void set_command(int command);
  /// @copydoc EffectInterface::get_value()
  virtual int get_value() const;
  /// @copydoc EffectInterface::set_value(int)
  virtual void set_value(int value);

};

/**
 * Create a specialized vector to reference Effect objects.
 */
VECTOR_OF(Effect, Effects, 255);

/**
 * Factory class to create effect objects.
 */
template<class EFFECT>
class EffectFactoryTemplate {

public:

  /**
   * Create a new instance of an effect class with provided command and value
   * or default.
   *
   * If no arguments are provided the effect will have the command 0 with the
   * value 0.
   *
   * @param command Command (default 0).
   * @param value Value of the command (default 0).
   *
   * @return A reference to the new effect instance.
   */
  static EFFECT* New(int command = 0, int value = 0) {
    EFFECT* effect = new EFFECT(command, value);
    return effect;
  }

  /**
   * Destroy an effect class instance, setting the reference to NULL when
   * done.
   *
   * @param effect A pointer to a reference to an effect class instance to
   *               destroy.
   */
  static void Delete(EFFECT** effect) {
    delete static_cast<EFFECT*>(*effect);
    *effect = NULL;
  }

};


/**
 * Effect factory to create effects with concrete implementation of the
 * effect itself.
 */
typedef EffectFactoryTemplate<Effect> EffectFactory;

#endif
