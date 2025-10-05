// ======================================================================
// \title  ExtBatManagerTestMain.cpp
// \author user
// \brief  cpp file for ExtBatManager component test main function
// ======================================================================

#include "ExtBatManagerTester.hpp"

TEST(Nominal, ExtBatManagerUT1) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT1();
}

TEST(Nominal, ExtBatManagerUT2) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT2();
}

TEST(Nominal, ExtBatManagerUT3) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT3();
}

TEST(Nominal, ExtBatManagerUT4_1) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT4_1();
}

TEST(Nominal, ExtBatManagerUT4_2) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT4_2();
}

TEST(Nominal, ExtBatManagerUT5_1) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT5_1();
}

TEST(Nominal, ExtBatManagerUT5_2) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT5_2();
}

TEST(Nominal, ExtBatManagerUT5_3) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT5_3();
}

TEST(Nominal, ExtBatManagerUT6_1) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT6_1();
}

TEST(Nominal, ExtBatManagerUT6_2) {
  Components::ExtBatManagerTester tester;
  tester.ExtBatManagerUT6_2();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
