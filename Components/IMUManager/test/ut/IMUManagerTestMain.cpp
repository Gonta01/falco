// ======================================================================
// \title  IMUManagerTestMain.cpp
// \author aos
// \brief  cpp file for IMUManager component test main function
// ======================================================================

#include "IMUManagerTester.hpp"

TEST(testGetDataSuccesICM20948, IMUManagerUT1) {
  Components::IMUManagerTester tester;
  tester.testGetDataSuccessICM20948();
}

TEST(testGetDataFailedICM20948, IMUManagerUT2) {
  Components::IMUManagerTester tester;
  tester.testGetDataFailedICM20948();
}

TEST(testGetRawDataSuccessBNO055, IMU2ManagerUT3) {
  Components::IMUManagerTester tester;
  tester.testGetRawDataSuccessBNO055();
}

TEST(testGetRawDataFailedBNO055, IMU2ManagerUT4) {
  Components::IMUManagerTester tester;
  tester.testGetRawDataFailedBNO055();
}

TEST(testGetQuatDataSuccessBNO055, IMU2ManagerUT5) {
  Components::IMUManagerTester tester;
  tester.testGetQuatDataSuccessBNO055();
}

TEST(testGetQuatDataFailedBNO055, IMU2ManagerUT6) {
  Components::IMUManagerTester tester;
  tester.testGetQuatDataFailedBNO055();
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
