// ======================================================================
// \title  IMU2ManagerTestMain.cpp
// \author aos
// \brief  cpp file for IMU2Manager component test main function
// ======================================================================

#include "IMU2ManagerTester.hpp"

TEST(testGetRawDataSuccessIMU2Manager, IMU2ManagerUT1) {
  Components::IMU2ManagerTester tester;
  tester.testGetRawDataSuccessIMU2Manager();
}

TEST(testGetRawDataFailedIMU2Manager, IMU2ManagerUT2) {
  Components::IMU2ManagerTester tester;
  tester.testGetRawDataFailedIMU2Manager();
}

TEST(testGetQuatDataSuccessIMU2Manager, IMU2ManagerUT3) {
  Components::IMU2ManagerTester tester;
  tester.testGetQuatDataSuccessIMU2Manager();
}

TEST(testGetQuatDataFailedIMU2Manager, IMU2ManagerUT4) {
  Components::IMU2ManagerTester tester;
  tester.testGetQuatDataFailedIMU2Manager();
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
