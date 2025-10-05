// ======================================================================
// \title  IMU1Manager.cpp
// \author user
// \brief  cpp file for IMU1Manager component implementation class
// ======================================================================

#include "Components/IMU1Manager/IMU1Manager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IMU1Manager ::
    IMU1Manager(const char* const compName) :
      IMU1ManagerComponentBase(compName)
  {

  }

  IMU1Manager ::
    ~IMU1Manager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void IMU1Manager::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    handleData();
  }

  void IMU1Manager::getData_handler(NATIVE_INT_TYPE portNum, Drv::IMU::Data& data) {
    data = icm20948Data;
  }

  void IMU1Manager::getQuaternion_handler(NATIVE_INT_TYPE portNum, Drv::IMU::Quaternion& quat) {
    quat = icm20948Quaternion;
  }

  void IMU1Manager::handleData() {
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
    Drv::IMU::CalibrationData calibrationData;
    bool isCalibrationValid = false;
    Drv::IMU::Status status;

    if (isRawDataPolled) {
      status = this->getAccelData_out(0, accelData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isAccelValid = true;
      } 
      else {
        isAccelValid = false;
      }
  
      status = this->getGyroData_out(0,  gyroData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isGyroValid = true;
      } 
      else {
        isGyroValid = false;
      }
  
      status = this->getMagnData_out(0,  magnData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isMagnValid = true;
      } 
      else {
        isMagnValid = false;
      }

      status = this->getCalibrationData_out(0, calibrationData);
      if(status == Drv::IMU::Status::SUCCESS) {
        isCalibrationValid = true;
      } 
      else {
        isCalibrationValid = false;
      }
    }
    else {
      isAccelValid = false;
      isGyroValid  = false;
      isMagnValid  = false;
    }

    if (isQuatDataPolled) { 
      status = this->getQuatData_out(0, quatData);
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
    
    icm20948Data.setCalibration(calibrationData);
    icm20948Data.setCalibrationValid(isCalibrationValid);
    
    icm20948Quaternion.setQuat(quatData);
    icm20948Quaternion.setQuatValid(isQuatValid);
    
    this->unLock();
  }
}
