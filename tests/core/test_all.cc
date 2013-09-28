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
#include "test_fixture.h"
#include "test_include_all.h"


TEST (SimpleDemoTest, SimpleDemo) {
  /*std::thread simple_test([&] () {
    EXPECT_EQ(1.0, 1.0);
  });*/
  EXPECT_EQ(1.0, 1.0);
}

/* CylceTable testing */
// ----------------------------------------------------------------------------
TEST (CycleTableTest, CreateCycleTable) {
  EXPECT_TRUE(true);
}

/* DailyTable testing */
// ----------------------------------------------------------------------------
TEST (DailyTableTest, CreateDailyTable) {
  mw::DailyTable("Test-DailyTable.db");
}

TEST (DailyTableTest, AddRecord) {
  //
}

TEST (DailyTableTest, ReadRecord) {
  //
}

/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
