/**
 * @file ProjectTest.cpp
 *
 * Test cases for the Project class.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Project.h"

/**
 * @test Constructor without arguments shall create a project.
 */
TEST(Project, Constructor_without_arguments_shall_create_a_project) {
  Project *project = new Project();
  ASSERT_FALSE(NULL == project);
}
