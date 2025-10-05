// ======================================================================
// \title  LedTestMain.cpp
// \author aos
// \brief  cpp file for Led component test main function
// ======================================================================

#include "LedTester.hpp"

TEST(testCmdBlinkingOnOffLed, testCmd) {
  Components::LedTester tester;
  tester.testCmdBlinkingOnOffLed();
}

TEST(testRunHanlderLed, testRun) {
  Components::LedTester tester;
  tester.testRunHandlerLed();
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
