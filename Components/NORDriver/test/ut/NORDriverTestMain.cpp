// ======================================================================
// \title  NORDriverTestMain.cpp
// \author aos
// \brief  cpp file for NORDriver component test main function
// ======================================================================

#include "NORDriverTester.hpp"

TEST(TestConfigureNORDriver, configure)
{
  Components::NORDriverTester tester;
  tester.testConfigureNORDriver();
}

TEST(TestReadNORDriver, read)
{
  Components::NORDriverTester tester;
  tester.testReadNORDriver();
}

TEST(TestWriteNORDriver, write)
{
  Components::NORDriverTester tester;
  tester.testWriteNORDriver();
}

TEST(TestFailureWrongMemoryIdNORDriver, erase)
{
  Components::NORDriverTester tester;
  tester.testFailureWrongMemoryIdNORDriver();
}
TEST(TestFailureAccessOutOfRangeNORDriver, failure)
{
  Components::NORDriverTester tester;
  tester.testFailureAccessOutOfRangeNORDriver();
}
TEST(TestFailureFlashMemoryBusyNORDriver, failure)
{
  Components::NORDriverTester tester;
  tester.testFailureFlashMemoryBusyNORDriver();
}
TEST(TestFailureInputBufferEmptyNORDriver, failure)
{
  Components::NORDriverTester tester;
  tester.testFailureInputBufferEmptyNORDriver();
}
TEST(TestFailureIOInterfaceErrorNORDriver, failure)
{
  Components::NORDriverTester tester;
  tester.testFailureIOInterfaceErrorNORDriver();
}
TEST(TestFailureUnalignedAccessNORDriver, failure)
{
  Components::NORDriverTester tester;
  tester.testFailureUnalignedAccessNORDriver();
}
TEST(TestGetMemoryAndSectorSizeNORDriver, getConstants)
{
  Components::NORDriverTester tester;
  tester.testGetMemoryAndSectorSizeNORDriver();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
