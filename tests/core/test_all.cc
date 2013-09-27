/*
 * test_all.cc
 *
 *  Description: Unit-tests for core testing.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <thread>
#include "tests/google/gtest/gtest.h"


TEST (SimpleDemoTest, SimpleDemo) {
  /*std::thread simple_test([&] () {
    EXPECT_EQ(1.0, 1.0);
  });*/
  EXPECT_EQ(1.0, 1.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
