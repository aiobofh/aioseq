/**
 * @file part_test.cc
 *
 * Test-suite for the Part clas.
 *
 */
#include "test.hh"

/**
 * Make all relevant test cases friends with the Part class to be able to
 * inspect the protected or private variables.
 */
#define PART_FRIENDS                            \
  friend_test(Part, Constructor)                \
  friend_test(Part, SettersAndGetters)          \
  friend_test(Part, Factory)                    \

#include "part.hh"

#include "mock_track.hh"
#include "mock_track_entry.hh"
#include "mock_pattern_row.hh"
#include "mock_pattern.hh"

#include "mock_part.hh"


/**
 * @test Part constructor.
 *
 * Make sure that the constructor pass the name of the part and the reference
 * to a list of patterns to the internal storage untouched.
 */
test_case(Part, Constructor) {
  string name("Foobar");
  MockPatterns patterns;
  Part part(name, &patterns);
  assert_eq(name, part.name);
  assert_eq(&patterns, part.patterns);
}


/**
 * @test Part setters and getters.
 *
 * Make sure that the name is possible to be set or fetched, and the reference
 * to the list of patterns that make up the part is possible to fetch
 * correctly.
 */
test_case(Part, SettersAndGetters) {
  string name("Foobar");
  MockPatterns patterns;
  Part part(name, &patterns);

  string name_foo("Foo");
  string name_bar("Bar");

  assert_setter_and_getter_twice_eq(name_foo, name_bar, part, name);
  assert_eq(&patterns, part.get_patterns());
}


/**
 * @test PartFactory creates valid part.
 *
 * Make sure that the PartFactory is able to create a valid part, both with
 * pattern-list reference argument or name and without.
 */
test_case(Part, Factory) {
  MockPattern project_pattern;
  MockPatterns project_patterns;
  project_patterns.push_back(&project_pattern);

  /*
   * Normal operaions for creating a new part with a pattern list with one
   * pointing to the first entry in the project patterns list.
   */
  Part *part = PartFactoryMock::New(&project_patterns);
  assert_eq("New part", part->name);
  assert_eq(static_cast<unsigned int>(1),
            dynamic_cast<MockPatterns*>(part->patterns)->size());
  assert_eq(&project_pattern, dynamic_cast<MockPatterns*>(part->patterns)->front());

  PartFactoryMock::Delete(&part);
  assert_eq(NULL, part);

  project_patterns.pop_back();
}
