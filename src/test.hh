/**
 * @file test.hh
 */
#ifndef _TEST_HH_
#define _TEST_HH_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>

using ::testing::_;
using ::testing::Return;
using ::testing::Eq;

#define ASSERT_SETTER_TWICE_EQ(firstval, secondval, object, member)     \
  object.set_##member((firstval));                                      \
  ASSERT_EQ((firstval), object.member);                                 \
  object.set_##member((secondval));                                     \
  ASSERT_EQ((secondval), object.member)

#define ASSERT_GETTER_TWICE_EQ(firstval, secondval, object, member)     \
  object.member = (firstval);                                           \
  ASSERT_EQ((firstval), object.get_##member());                         \
  object.member = (secondval);                                          \
  ASSERT_EQ((secondval), object.member)

#define TestFixture protected ::testing::Test

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * Only default constructor with no arguments is used.
 */
#define expose_class_0(base)                              \
  class base##Exposed0 : public base {                    \
    friend class base##_Constructors_Test;                \
    friend class base##_SettersAndGetters_Test;           \
  public:                                                 \
    base##Exposed0() : base() {}                          \
    ~base##Exposed0() {}                                  \
  }

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * A default constructor with no arguments, and one with two arguments is used.
 */
#define expose_class_0_and_2(base, type1, name1, type2, name2)            \
  class base##Exposed0And2 : public base {                                \
    friend class base##_Constructors_Test;                                \
    friend class base##_SettersAndGetters_Test;                           \
  public:                                                                 \
    base##Exposed0And2() : base() {}                                      \
    base##Exposed0And2(type1 name1, type2 name2) : base(name1, name2) {}  \
    ~base##Exposed0And2() {}                                              \
  }

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * Only a constructor with two arguments is used.
 */
#define expose_class_1(base, type, name)           \
  class base##Exposed1 : public base {             \
    friend class base##_Constructors_Test;         \
    friend class base##_SettersAndGetters_Test;    \
  public:                                          \
    base##Exposed1(type name) : base(name) {}      \
    ~base##Exposed1() {}                           \
  }

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * Only a constructor with two arguments is used.
 */
#define expose_class_2(base, type1, name1, type2, name2)                \
  class base##Exposed2 : public base {                                  \
    friend class base##_Constructors_Test;                              \
    friend class base##_SettersAndGetters_Test;                         \
  public:                                                               \
    base##Exposed2(type1 name1, type2 name2) : base(name1, name2) {}    \
    ~base##Exposed2() {}                                                \
  }

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * Only a constructor with three arguments is used.
 */
#define expose_class_3(base, type1, name1,                              \
                       type2, name2,                                    \
                       type3, name3)                                    \
  class base##Exposed3 : public base {                                  \
    friend class base##_Constructors_Test;                              \
    friend class base##_SettersAndGetters_Test;                         \
  public:                                                               \
    base##Exposed3(type1 name1, type2 name2, type3 name3) :             \
      base(name1, name2, name3) {}                                      \
    ~base##Exposed3() {}                                                \
  }

/**
 * Declare a wrapper class with friends to tests: <base-class>, Constructors
 * and <base-class>, SettersAndGetters (exposing protected member variables).
 *
 * Only a constructor with four arguments is used.
 */
#define expose_class_4(base, type1, name1,                              \
                       type2, name2,                                    \
                       type3, name3,                                    \
                       type4, name4)                                    \
  class base##Exposed4 : public base {                                  \
    friend class base##_Constructors_Test;                              \
    friend class base##_SettersAndGetters_Test;                         \
  public:                                                               \
  base##Exposed4(type1 name1, type2 name2,                              \
                 type3 name3, type4 name4):                             \
      base(name1, name2, name3, name4) {}                               \
    ~base##Exposed4() {}                                                \
  }

/**
 * Asserts that setter- and getter-methods can be used more than once.
 */
#define assert_setter_and_getter_twice_eq(firstval, secondval, object, member) \
  ASSERT_SETTER_TWICE_EQ(firstval, secondval, object, member);          \
  ASSERT_GETTER_TWICE_EQ(firstval, secondval, object, member)

#define assert_eq(expected, actual) \
  ASSERT_EQ(expected, actual)

#define assert_ne(expected, actual) \
  ASSERT_NE(expected, actual)

#define assert_false(expression) \
  ASSERT_FALSE(expression)

#define assert_true(expression) \
  ASSERT_TRUE(expression)

#define on_call(mock, method) \
  ON_CALL(mock, method)

#define expect_call_times(mock, method, times)        \
  EXPECT_CALL(mock, method).Times(times)

#define expect_call_times_will_return(mock, method, times, ret)         \
  EXPECT_CALL(mock, method).Times(times).WillRepeatedly(Return(ret))

#define expect_call_times_will_call(mock, method, times, other_method) \
  EXPECT_CALL(mock, method).Times(times).WillByDefault(Invoke(&mock, &other_method)

#define test_case(base, name) TEST(base, name)
#define fixture_test_case(fixture, name) TEST_F(fixture, name)

#define friend_test(classname, name) \
  friend class classname##_##name##_Test;

#define assert_stderr_eq(expected, expression)                          \
  {                                                                     \
    streambuf* _cerr_buf(std::cerr.rdbuf());                            \
    stringstream _cerr_variable;                                        \
    std::cerr.rdbuf(_cerr_variable.rdbuf());                            \
    expression;                                                         \
    assert_eq(expected, _cerr_variable.str());                          \
    std::cerr.rdbuf(_cerr_buf);                                         \
  }

#define assert_stdout_eq(expected, expression)                          \
  {                                                                     \
    streambuf* _cout_buf(std::cout.rdbuf());                            \
    stringstream _cout_variable;                                        \
    std::cout.rdbuf(_cout_variable.rdbuf());                            \
    expression;                                                         \
    assert_eq(expected, _cout_variable.str());                          \
    std::cout.rdbuf(_cout_buf);                                         \
  }

#endif
