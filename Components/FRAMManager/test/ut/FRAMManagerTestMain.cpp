// ======================================================================
// \title  FRAMManagerTestMain.cpp
// \author aos
// \brief  cpp file for FRAMManager component test main function
// ======================================================================

#include "FRAMManagerTester.hpp"

TEST(TestRequestReadFRAMManager, testRequestRead) {
  Components::FRAMManagerTester tester;
  tester.testRequestReadFRAMManager();
}

TEST(TestRequestWriteFRAMManager, testRequestWrite) {
  Components::FRAMManagerTester tester;
  tester.testRequestWriteFRAMManager();
}

TEST(TestRequestSleepFRAMManager, testRequestSleep) {
  Components::FRAMManagerTester tester;
  tester.testRequestSleepFRAMManager();
}

TEST(TestRequestWakeupFRAMManager, testRequestWakeup) {
  Components::FRAMManagerTester tester;
  tester.testRequestWakeupFRAMManager();
}

TEST(TestFailureRequestReadFRAMManager, testFailure) {
  Components::FRAMManagerTester tester;
  tester.testFailureRequestReadFRAMManager();
}

TEST(TestFailureRequestWriteFRAMManager, testFailure) {
  Components::FRAMManagerTester tester;
  tester.testFailureRequestWriteFRAMManager();
}

TEST(TestFailureRequestSleepFRAMManager, testFailure) {
  Components::FRAMManagerTester tester;
  tester.testFailureRequestSleepFRAMManager();
}

TEST(TestFailureRequestWakeupFRAMManager, testFailure) {
  Components::FRAMManagerTester tester;
  tester.testFailureRequestWakeupFRAMManager();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
