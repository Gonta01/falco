// ======================================================================
// \title  IMUTester.cpp
// \author user
// \brief  cpp file for IMUTester component implementation class
// ======================================================================

#include "Components/IMUTester/IMUTester.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IMUTester ::
    IMUTester(const char* const compName) :
      IMUTesterComponentBase(compName)
  {

  }

  IMUTester ::
    ~IMUTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void IMUTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    this->getICM20948Data_out(0, icm20948Data);
    if (icm20948Data.getAccelValid() && icm20948Data.getGyroValid() && icm20948Data.getMagnValid()) {
      this->tlmWrite_ICM20948_Accel_X(icm20948Data.getAccel().getX());
      this->tlmWrite_ICM20948_Accel_Y(icm20948Data.getAccel().getY());
      this->tlmWrite_ICM20948_Accel_Z(icm20948Data.getAccel().getZ());

      this->tlmWrite_ICM20948_Gyro_X(icm20948Data.getGyro().getX());
      this->tlmWrite_ICM20948_Gyro_Y(icm20948Data.getGyro().getY());
      this->tlmWrite_ICM20948_Gyro_Z(icm20948Data.getGyro().getZ());

      this->tlmWrite_ICM20948_Magnet_X(icm20948Data.getMagn().getX());
      this->tlmWrite_ICM20948_Magnet_Y(icm20948Data.getMagn().getY());
      this->tlmWrite_ICM20948_Magnet_Z(icm20948Data.getMagn().getZ());
    }

    this->getICM20948Quaternion_out(0, icm20948Quaternion);
    if (icm20948Quaternion.getQuatValid()) {
      this->tlmWrite_ICM20948_Q0(icm20948Quaternion.getQuat().getQ0());
      this->tlmWrite_ICM20948_Q1(icm20948Quaternion.getQuat().getQ1());
      this->tlmWrite_ICM20948_Q2(icm20948Quaternion.getQuat().getQ2());
      this->tlmWrite_ICM20948_Q3(icm20948Quaternion.getQuat().getQ3());
    }

    this->getBNO055Data_out(0, bno055Data);
    if (bno055Data.getAccelValid() && bno055Data.getGyroValid() && bno055Data.getMagnValid()) {
      this->tlmWrite_BNO055_Accel_X(bno055Data.getAccel().getX());
      this->tlmWrite_BNO055_Accel_Y(bno055Data.getAccel().getY());
      this->tlmWrite_BNO055_Accel_Z(bno055Data.getAccel().getZ());

      this->tlmWrite_BNO055_Gyro_X(bno055Data.getGyro().getX());
      this->tlmWrite_BNO055_Gyro_Y(bno055Data.getGyro().getY());
      this->tlmWrite_BNO055_Gyro_Z(bno055Data.getGyro().getZ());

      this->tlmWrite_BNO055_Magn_X(bno055Data.getMagn().getX());
      this->tlmWrite_BNO055_Magn_Y(bno055Data.getMagn().getY());
      this->tlmWrite_BNO055_Magn_Z(bno055Data.getMagn().getZ());
    }
    
    this->getBNO055Quaternion_out(0, bno055Quaternion);
    if (bno055Quaternion.getQuatValid()) {
      this->tlmWrite_BNO055_W(bno055Quaternion.getQuat().getQ0());
      this->tlmWrite_BNO055_X(bno055Quaternion.getQuat().getQ1());
      this->tlmWrite_BNO055_Y(bno055Quaternion.getQuat().getQ2());
      this->tlmWrite_BNO055_Z(bno055Quaternion.getQuat().getQ3());
    }
  }

}
