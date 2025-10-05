// ======================================================================
// \title  DPS310TestMain.cpp
// \author aos
// \brief  cpp file for DPS310 component test main function
// ======================================================================

#include "DPS310Tester.hpp"

TEST(TestDPS310GetTemperature, DPS310UT1) {
  Components::DPS310Tester tester;
  tester.testDPS310Temperature();
}

TEST(TestDPS310GetColdTemperature, DPS310UT2) {
  Components::DPS310Tester tester;
  tester.testDPS310ColdTemperature();
}

TEST(TestDPS310Pressure, DPS310UT3) {
  Components::DPS310Tester tester;
  tester.testDPS310Pressure();
}

TEST(TestDPS310FailureNotInited, DPS310UT4) {
  Components::DPS310Tester tester;
  tester.testDPS310FailureNotInited();
}

TEST(TestDPS310FailureWrite, DPS310UT5) {
  Components::DPS310Tester tester;
  tester.testDPS310FailureWrite();
}

TEST(TestDPS310FailureRead, DPS310UT6) {
  Components::DPS310Tester tester;
  tester.testDPS310FailureRead();
}



int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
