// ======================================================================
// \title  NTCTestMain.cpp
// \author user
// \brief  cpp file for NTC component test main function
// ======================================================================

#include "NTCTester.hpp"

TEST(testNTCGetTemperature, getTemperature) {
  Components::NTCTester tester;
  tester.testNTCGetTemperature();
}

TEST(testNTCColdTemperature, getTemperature) {
  Components::NTCTester tester;
  tester.testNTCColdTemperature();
}

TEST(testNTCPolynomTemperature, getPolynomTemperature) {
  Components::NTCTester tester;
  tester.testNTCPolynomTemperature();
}

TEST(testNTCPolynomColdTemperature, getPolynomColdTemperature) {
  Components::NTCTester tester;
  tester.testNTCPolynomColdTemperature();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
