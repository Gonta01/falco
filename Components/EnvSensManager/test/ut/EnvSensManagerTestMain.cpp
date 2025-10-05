// ======================================================================
// \title  EnvSensManagerTestMain.cpp
// \author user
// \brief  cpp file for EnvSensManager component test main function
// ======================================================================

#include "EnvSensManagerTester.hpp"

TEST(testEnvSensManagerAveraging, testAveraging) {
  Components::EnvSensManagerTester tester;
  tester.testEnvSensManagerAveraging();
}

TEST(testEnvSensManagerInvalidData, testInvalidData) {
  Components::EnvSensManagerTester tester;
  tester.testEnvSensManagerInvalidData();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
