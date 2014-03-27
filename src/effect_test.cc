/**
 * @file effect_test.cc
 *
 * Test-suite for the Effect class.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Effect class to be able to
 * inspect the protected or private variables.
 */
#define EFFECT_FRIENDS                          \
  friend_test(Effect, Constructor)              \
  friend_test(Effect, SettersAndGetters)        \
  friend_test(Effect, Factory)                  \

#include "effect.hh"
#include "mock_effect.hh"

/**
 * @test Effect constructors.
 *
 * Make sure that the constructor sets the command and value to 0 if no
 * arguments are provided, otherwise it shall pass the command and velociy to
 * the internal storage untouched.
 */
test_case(Effect, Constructor) {
  Effect effect1;

  assert_eq(0, effect1.command);
  assert_eq(0, effect1.value);

  Effect effect2(1, 2);

  assert_eq(1, effect2.command);
  assert_eq(2, effect2.value);
}


/**
 * @test Effect setters and getters.
 *
 * Make sure that the command and value are possible to be set or fetched
 * correctly.
 */
test_case(Effect, SettersAndGetters) {
  Effect effect;

  assert_setter_and_getter_twice_eq(1, 3, effect, command);
  assert_setter_and_getter_twice_eq(2, 4, effect, value);
}


/**
 * @test EffectFactory creates valid effects and destroy them.
 *
 * Make sure that the EffectFactory is able to create a valid effect both with
 * no arguments (setting default values) and with arguments, setting them to
 * the effect.
 */
test_case(Effect, FactoryNew) {
  Effect* effect1 = EffectFactory::New();
  assert_eq(0, effect1->get_command());
  assert_eq(0, effect1->get_value());
  Effect* effect2 = EffectFactory::New(1, 2);
  assert_eq(1, effect2->get_command());
  assert_eq(2, effect2->get_value());

  EffectFactory::Delete(&effect1);
  assert_eq(NULL, effect1);
  EffectFactory::Delete(&effect2);
  assert_eq(NULL, effect2);
}
