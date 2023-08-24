#include <gtest/gtest.h>

TEST(TestAddIntExample, test_add_int) {
  int res = 10 + 24;
  EXPECT_EQ(res, 34);
}