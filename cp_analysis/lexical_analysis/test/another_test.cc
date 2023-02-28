#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(Test2, BasicAssertions) {
  // Expect two strings not to be equal.
  ASSERT_EQ("hello", "world");
  EXPECT_STREQ("FUCK", "YOU");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}