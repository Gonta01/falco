// ======================================================================
// \title  ICM20948TestMain.cpp
// \author aos
// \brief  cpp file for ICM20948 component test main function
// ======================================================================

#include "ICM20948Tester.hpp"

TEST(rawInitICM20948, isConected) {
  Components::ICM20948Tester tester;
  tester.testIsConnectedICM20948();
}

TEST(rawInitICM20948, deviceReset) {
  Components::ICM20948Tester tester;
  tester.testDeviceResetICM20948();
}

TEST(rawInitICM20948, configClockSource) {
  Components::ICM20948Tester tester;
  tester.testConfigClockSourceICM20948();
}

TEST(rawInitICM20948, enableGyroAndAccel) {
  Components::ICM20948Tester tester;
  tester.testEnableGyroAndAccelICM20948();
}

TEST(rawInitICM20948, enableFIFOFalse) {
  Components::ICM20948Tester tester;
  tester.testEnableFIFOFalseICM20948();
}

TEST(rawInitICM20948, disableDMP) {
  Components::ICM20948Tester tester;
  tester.testDisableDMPICM20948();
}

TEST(rawInitICM20948, setConfig) {
  Components::ICM20948Tester tester;
  tester.testSetConfigICM20948();
}

TEST(rawInitICM20948, configureFIFO) {
  Components::ICM20948Tester tester;
  tester.testConfigureFIFOICM20948();
}

TEST(rawInitICM20948, configureInterruptPin) {
  Components::ICM20948Tester tester;
  tester.testConfigureInterruptPinICM20948();
}

TEST(rawInitICM20948, configureDataReadyInterrupt) {
  Components::ICM20948Tester tester;
  tester.testConfigureDataReadyInterruptICM20948();
}

TEST(rawInitICM20948, configureGyroSampleRate) {
  Components::ICM20948Tester tester;
  tester.testConfigureGyroSampleRateICM20948();
}

TEST(rawInitICM20948, configureAccelSampleRate) {
  Components::ICM20948Tester tester;
  tester.testConfigureAccelSampleRateICM20948();
}

TEST(rawInitICM20948, initMagnetometer) {
  Components::ICM20948Tester tester;
  tester.testInitMagnetometerICM20948();
}

TEST(rawInitICM20948, calibrateGyroOffsets) {
  Components::ICM20948Tester tester;
  tester.testCalibrateGyroOffsetsICM20948();
}

TEST(rawInitICM20948, enableFIFOTrue) {
  Components::ICM20948Tester tester;
  tester.testEnableFIFOTrueICM20948();
}

TEST(rawInitICM20948, resetFIFO) {
  Components::ICM20948Tester tester;
  tester.testResetFIFOICM20948();
}

TEST(testPollRawDataICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testPollRawDataICM20948();
}

TEST(testPollQuatDataICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testPollQuatDataICM20948();
}

TEST(testFailureNotInitedICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testFailureNotInitedICM20948();
}

TEST(testFailureReadICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testFailureReadICM20948();
}

TEST(testFailureWriteICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testFailureWriteICM20948();
}

TEST(testFailureReadNotEqualToWrittenICM20948, getData) {
  Components::ICM20948Tester tester;
  tester.testFailureReadNotEqualToWrittenICM20948();
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
