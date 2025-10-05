// ======================================================================
// \title  IOExpanderTestMain.cpp
// \author aos
// \brief  cpp file for IOExpander component test main function
// ======================================================================

#include "IOExpanderTester.hpp"

TEST(TestConfigureIOExpander, testConfigure)
{
  Components::IOExpanderTester tester;
  tester.testConfigureIOExpander();
}

TEST(TestReadAnalogInputIOExpander, testReadAnalog)
{
  Components::IOExpanderTester tester;
  tester.testReadAnalogInputIOExpander();
}

TEST(TestReadDigitalMultipleInputsIOExpander, testReadDigitalMultiple)
{
  Components::IOExpanderTester tester;
  tester.testReadDigitalMultipleInputsIOExpander();
}

TEST(TestWriteDigitalMultipleOutputsIOExpander, testWriteDigitalMiltiple)
{
  Components::IOExpanderTester tester;
  tester.testWriteDigitalMultipleOutputsIOExpander();
}

TEST(TestReadDigitalSingleInputIOExpander, testReadSingle)
{
  Components::IOExpanderTester tester;
  tester.testReadDigitalSingleInputIOExpander();
}

TEST(TestWriteDigitalSingleOutputIOExpander, testWriteDigitalSingle)
{
  Components::IOExpanderTester tester;
  tester.testWriteDigitalSingleOutputIOExpander();
}

TEST(TestFailureNotInitedIOExpander, testFailure)
{
  Components::IOExpanderTester tester;
  tester.testFailureNotInitedIOExpander();
}

TEST(TestFailureWrongChannelConfigurationIOExpander, testFailure)
{
  Components::IOExpanderTester tester;
  tester.testFailureWrongChannelConfigurationIOExpander();
}
TEST(TestFailureCommunicationErrorIOExpander, testFailure)
{
  Components::IOExpanderTester tester;
  tester.testFailureCommunicationErrorIOExpander();
}
TEST(TestFailureCRCErrorIOExpander, testFailure)
{
  Components::IOExpanderTester tester;
  tester.testFailureCRCErrorIOExpander();
}
TEST(TestFailureCalibrationUnfinishedIOExpander, testFailure)
{
  Components::IOExpanderTester tester;
  tester.testFailureCalibrationUnfinishedIOExpander();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
