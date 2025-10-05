// ======================================================================
// \title  IMU1ManagerTestMain.cpp
// \author aos
// \brief  cpp file for IMU1Manager component test main function
// ======================================================================

#include "IMU1ManagerTester.hpp"

TEST(testGetDataSuccesIMU1Manager, IMU1ManagerUT1) {
  Components::IMU1ManagerTester tester;
  tester.testGetDataSuccessIMU1Manager();
}

TEST(testGetDataFailedIMU1Manager, IMU1ManagerUT2) {
  Components::IMU1ManagerTester tester;
  tester.testGetDataFailedIMU1Manager();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
