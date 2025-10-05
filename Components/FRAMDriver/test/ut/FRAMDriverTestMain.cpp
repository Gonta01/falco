// ======================================================================
// \title  FRAMDriverTestMain.cpp
// \author aos
// \brief  cpp file for FRAMDriver component test main function
// ======================================================================

#include "FRAMDriverTester.hpp"

TEST(TestConfigureFRAMDriver, testConfigure) {
  Components::FRAMDriverTester tester;
  tester.testConfigureFRAMDriver();
}

TEST(TestReadFRAMDriver, testRead) {
  Components::FRAMDriverTester tester;
  tester.testReadFRAMDriver();
}

TEST(TestWriteFRAMDriver, testWrite) {
  Components::FRAMDriverTester tester;
  tester.testWriteFRAMDriver();
}

TEST(TestReInitFRAMDriver, testReinit) {
  Components::FRAMDriverTester tester;
  tester.testReInitFRAMDriver();
}

TEST(TestFailureReInitFRAMDriver, testFailure) {
  Components::FRAMDriverTester tester;
  tester.testFailureReInitFRAMDriver();
}

TEST(TestFailureAddressOutOfRangeFRAMDriver, testFailure) {
  Components::FRAMDriverTester tester;
  tester.testFailureAddressOutOfRangeFRAMDriver();
}

TEST(TestFailureInputBufferEmptyFRAMDriver, testFailure) {
  Components::FRAMDriverTester tester;
  tester.testFailureInputBufferEmptyFRAMDriver();
}

TEST(TestFailureIOInterfaceErrorFRAMDriver, testFailure) {
  Components::FRAMDriverTester tester;
  tester.testFailureIOInterfaceErrorFRAMDriver();
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
