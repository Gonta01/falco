// ======================================================================
// \title  STS21TestMain.cpp
// \author aos
// \brief  cpp file for STS21 component test main function
// ======================================================================

#include "STS21Tester.hpp"

TEST(TestSTS21Temperature, STS21UT1) {
  Components::STS21Tester tester;
  tester.testSTS21Temperature();
}

TEST(TestSTS21ColdTemperature, STS21UT2) {
  Components::STS21Tester tester;
  tester.testSTS21ColdTemperature();
}

TEST(TestSTS22FailureCommunication, STS21UT3) {
  Components::STS21Tester tester;
  tester.testSTS21FailureCommunication();
}

TEST(TestSTS21FailureNotInited, STS21UT4) {
  Components::STS21Tester tester;
  tester.testSTS21FailureNotInited();
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
