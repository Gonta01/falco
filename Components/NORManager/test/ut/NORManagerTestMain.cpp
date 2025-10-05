// ======================================================================
// \title  NORManagerTestMain.cpp
// \author aos
// \brief  cpp file for NORManager component test main function
// ======================================================================

#include "NORManagerTester.hpp"

TEST(TestGetMemoryAndSectorSizeNORManager, getMemoryAndSize)
{
  Components::NORManagerTester tester;
  tester.testGetMemoryAndSectorSizeNORManager();
}

TEST(TestEraseNORManager, erase)
{
  Components::NORManagerTester tester;
  tester.testRequestEraseNORManager();
}
TEST(TestRequestReadNORManager, read)
{
  Components::NORManagerTester tester;
  tester.testRequestReadNORManager();
}
TEST(TestRequestWriteNORManager, write)
{
  Components::NORManagerTester tester;
  tester.testRequestWriteNORManager();
}

TEST(TestEraseNORManagerFailed, eraseFailed)
{
  Components::NORManagerTester tester;
  tester.testRequestEraseNORManagerFailed();
}
TEST(TestRequestReadNORManagerFailed, readFailed)
{
  Components::NORManagerTester tester;
  tester.testRequestReadNORManagerFailed();
}
TEST(TestRequestWriteNORManagerFailed, writeFailed)
{
  Components::NORManagerTester tester;
  tester.testRequestWriteNORManagerFailed();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
