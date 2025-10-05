// ======================================================================
// \title  IMUManager.cpp
// \author user
// \brief  cpp file for IMUManager component implementation class
// ======================================================================

#include "Components/IMUManager/IMUManager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IMUManager ::
    IMUManager(const char* const compName) :
      IMUManagerComponentBase(compName)
  {

  }

  IMUManager ::
    ~IMUManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void IMUManager ::
    getBNO055Data_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::Data& data
    )
  {
    data = bno055Data;
  }

  void IMUManager ::
    getBNO055Quaternion_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::Quaternion& quat
    )
  {
    quat = bno055Quaternion;
  }

  void IMUManager ::
    getICM20948Data_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::Data& data
    )
  {
    data = icm20948Data;
  }

  void IMUManager ::
    getICM20948Quaternion_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::Quaternion& quat
    )
  {
    quat = icm20948Quaternion;
  }
  
  void IMUManager::handleICM20948() {
    bool isRawDataPolled  = (this->pollRawData_out(0) == Drv::IMU::Status::SUCCESS);
    bool isQuatDataPolled = (this->pollQuatData_out(0) == Drv::IMU::Status::SUCCESS);

    Drv::IMU::AccelData accelData;
    bool isAccelValid = false;
    Drv::IMU::GyroData gyroData;
    bool isGyroValid = false;
    Drv::IMU::MagnData magnData;
    bool isMagnValid = false;
    Drv::IMU::QuatData quatData;
    bool isQuatValid = false;
    Drv::IMU::Status status;

    if (isRawDataPolled) {
      status = this->getAccelDataICM20948_out(0, accelData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isAccelValid = true;
      } 
      else {
        isAccelValid = false;
      }
  
      status = this->getGyroDataICM20948_out(0,  gyroData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isGyroValid = true;
      } 
      else {
        isGyroValid = false;
      }
  
      status = this->getMagnDataICM20948_out(0,  magnData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isMagnValid = true;
      } 
      else {
        isMagnValid = false;
      }
    }
    else {
      isAccelValid = false;
      isGyroValid  = false;
      isMagnValid  = false;
    }

    if (isQuatDataPolled) { 
      status = this->getQuatDataICM20948_out(0, quatData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isQuatValid = true;
      }
      else {
        isQuatValid = false;
      }
    } else {
      isQuatValid  = false;
    }

    this->lock();
    icm20948Data.setAccel(accelData);
    icm20948Data.setAccelValid(isAccelValid);
    
    icm20948Data.setGyro(gyroData);
    icm20948Data.setGyroValid(isGyroValid);
  
    icm20948Data.setMagn(magnData);
    icm20948Data.setMagnValid(isMagnValid);
  
    icm20948Quaternion.setQuat(quatData);
    icm20948Quaternion.setQuatValid(isQuatValid);
    this->unLock();
  }
  
  void IMUManager::handleBNO055() {
    Drv::IMU::AccelData accelData;
    bool isAccelValid = false;
    Drv::IMU::GyroData gyroData;
    bool isGyroValid = false;
    Drv::IMU::MagnData magnData;
    bool isMagnValid = false;
    Drv::IMU::QuatData quatData;
    bool isQuatValid = false;
    Drv::IMU::Status status;
    
    status = this->getAccelDataBNO055_out(0, accelData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isAccelValid = true;
    } 
    else {
      isAccelValid = false;
    }

    status = this->getGyroDataBNO055_out(0,  gyroData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isGyroValid = true;
    } 
    else {
      isGyroValid = false;
    }
    
    status = this->getMagnDataBNO055_out(0,  magnData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isMagnValid = true;
    } 
    else {
      isMagnValid = false;
    }

    status = this->getQuatDataBNO055_out(0, quatData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isQuatValid = true;
    }
    else {
      isQuatValid = false;
    }

    this->lock();
    bno055Data.setAccel(accelData);
    bno055Data.setAccelValid(isAccelValid);
    
    bno055Data.setGyro(gyroData);
    bno055Data.setGyroValid(isGyroValid);

    bno055Data.setMagn(magnData);
    bno055Data.setMagnValid(isMagnValid);

    bno055Quaternion.setQuat(quatData);
    bno055Quaternion.setQuatValid(isQuatValid);
    this->unLock();
  }

  void IMUManager::run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    handleICM20948();
    handleBNO055();
  }
}
