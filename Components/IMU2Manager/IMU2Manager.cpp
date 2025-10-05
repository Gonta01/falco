// ======================================================================
// \title  IMU2Manager.cpp
// \author user
// \brief  cpp file for IMU2Manager component implementation class
// ======================================================================

#include "Components/IMU2Manager/IMU2Manager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IMU2Manager ::
    IMU2Manager(const char* const compName) :
      IMU2ManagerComponentBase(compName)
  {

  }

  IMU2Manager ::
    ~IMU2Manager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void IMU2Manager::run_handler( NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    handleData();
  }

  void IMU2Manager::getData_handler(NATIVE_INT_TYPE portNum, Drv::IMU::Data& data) {
    data = bno055Data;
  }

  void IMU2Manager::getQuaternion_handler(NATIVE_INT_TYPE portNum, Drv::IMU::Quaternion& quat) {
    quat = bno055Quaternion;
  }

  void IMU2Manager::handleData() {
    Drv::IMU::RawData rawData;
    Drv::IMU::AccelData accelData;
    Drv::IMU::GyroData gyroData;
    Drv::IMU::MagnData magnData;
    bool isRawDataValid = false;

    Drv::IMU::QuatData quatData;
    bool isQuatValid = false;

    Drv::IMU::Status status;

    status = this->getRawData_out(0, rawData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isRawDataValid = true;
    } 
    else {
      isRawDataValid = false;
    }

    status = this->getQuatData_out(0, quatData);
    if(status == Drv::IMU::Status::SUCCESS) {
      isQuatValid = true;
    }
    else {
      isQuatValid = false;
    }

    this->lock();
    bno055Data.setAccel(rawData.getaccelData());
    bno055Data.setAccelValid(isRawDataValid);
    
    bno055Data.setGyro(rawData.getgyroData());
    bno055Data.setGyroValid(isRawDataValid);

    bno055Data.setMagn(rawData.getmagnData());
    bno055Data.setMagnValid(isRawDataValid);

    bno055Quaternion.setQuat(quatData);
    bno055Quaternion.setQuatValid(isQuatValid);
    this->unLock();
  }

}
