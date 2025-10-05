// ======================================================================
// \title  IMUManagerTester.cpp
// \author aos
// \brief  cpp file for IMUManager component test harness implementation class
// ======================================================================

#include "IMUManagerTester.hpp"
#include "STest/Pick/Pick.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  IMUManagerTester ::
      IMUManagerTester() : IMUManagerGTestBase("IMUManagerTester", IMUManagerTester::MAX_HISTORY_SIZE),
                           component("IMUManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  IMUManagerTester ::
      ~IMUManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------
  void IMUManagerTester ::
      testGetDataSuccessICM20948()
  {
    Drv::IMU::AccelData accelData(0.01, -0.98, 0.05);
    this->setMockGetAccelDataICM20948(accelData);
    Drv::IMU::GyroData gyroData(0.5, -0.3, 0.0);
    this->setMockGetGyroDataICM20948(gyroData);
    Drv::IMU::MagnData magnData(30.2, -47.6, 12.0);
    this->setMockGetMagnDataICM20948(magnData);
    Drv::IMU::QuatData quatData(0.707, 0.0, 0.707, 0.0);
    this->setMockGetQuatDataICM20948(quatData);

    this->setMockPollRawDataStatusICM20948(Drv::IMU::Status::SUCCESS);
    this->setMockPollQuatDataStatusICM20948(Drv::IMU::Status::SUCCESS);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getICM20948Data(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    actualData.setCalibration(calibrData);
    actualData.setCalibrationValid(true);
    
    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_EQ(actualData, expectedData);

    Drv::IMU::Quaternion actualQuat;
    this->invoke_to_getICM20948Quaternion(0, actualQuat);

    Drv::IMU::Quaternion expectedQuat(quatData, true);
    ASSERT_EQ(actualQuat, expectedQuat);
  }

  void IMUManagerTester ::
      testGetDataFailedICM20948()
  {
    Drv::IMU::AccelData accelData(0.01, -0.98, 0.05);
    this->setMockGetAccelDataICM20948(accelData);
    Drv::IMU::GyroData gyroData(0.5, -0.3, 0.0);
    this->setMockGetGyroDataICM20948(gyroData);
    Drv::IMU::MagnData magnData(30.2, -47.6, 12.0);
    this->setMockGetMagnDataICM20948(magnData);
    Drv::IMU::QuatData quatData(0.707, 0.0, 0.707, 0.0);
    this->setMockGetQuatDataICM20948(quatData);

    this->setMockPollRawDataStatusICM20948(Drv::IMU::Status::FAILED);
    this->setMockPollQuatDataStatusICM20948(Drv::IMU::Status::FAILED);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getICM20948Data(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_NE(actualData, expectedData);

    Drv::IMU::Quaternion actualQuat;
    this->invoke_to_getICM20948Quaternion(0, actualQuat);

    Drv::IMU::Quaternion expectedQuat(quatData, true);
    ASSERT_NE(actualQuat, expectedQuat);
  }

  void IMUManagerTester ::
      testGetRawDataSuccessBNO055()
  {
    Drv::IMU::AccelData accelData(1.0, 2.0, 3.0);
    Drv::IMU::GyroData gyroData(1.0, 2.0, 3.0);
    Drv::IMU::MagnData magnData(1.0, 2.0, 3.0);
    this->setMockGetAccelDataBNO055(accelData);
    this->setMockGetGyroDataBNO055(gyroData);
    this->setMockGetMagnDataBNO055(magnData);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getBNO055Data(0, actualData);
    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    actualData.setCalibration(calibrData);
    actualData.setCalibrationValid(true);

    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_EQ(actualData, expectedData);
  }

  void IMUManagerTester ::
      testGetRawDataFailedBNO055()
  {
    Drv::IMU::AccelData accelData(1.0, 2.0, 3.0);
    Drv::IMU::GyroData gyroData(1.0, 2.0, 3.0);
    Drv::IMU::MagnData magnData(1.0, 2.0, 3.0);
    this->setMockGetAccelDataBNO055(accelData);
    this->setMockGetGyroDataBNO055(gyroData);
    this->setMockGetMagnDataBNO055(magnData);
    this->setMockGetAccelDataStatusBNO055(Drv::IMU::Status::FAILED);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getBNO055Data(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_NE(actualData, expectedData);
  }

  void IMUManagerTester ::
      testGetQuatDataSuccessBNO055()
  {
    Drv::IMU::QuatData quatData(0.92, 0.1, 0.2, 0.3);
    this->setMockGetQuatDataBNO055(quatData);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Quaternion actualQuaternion;
    this->invoke_to_getBNO055Quaternion(0, actualQuaternion);

    Drv::IMU::Quaternion expectedQuaternion(quatData, true);
    ASSERT_EQ(actualQuaternion, expectedQuaternion);
  }
  void IMUManagerTester ::
      testGetQuatDataFailedBNO055()
  {
    Drv::IMU::QuatData quatData(0.92, 0.1, 0.2, 0.3);
    this->setMockGetQuatDataBNO055(quatData);
    this->setMockGetQuatDataStatusBNO055(Drv::IMU::Status::FAILED);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Quaternion actualQuaternion;
    this->invoke_to_getBNO055Quaternion(0, actualQuaternion);

    Drv::IMU::Quaternion expectedQuaternion(quatData, true);
    ASSERT_NE(actualQuaternion, expectedQuaternion);
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::IMU::Status IMUManagerTester ::
      from_getAccelDataBNO055_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::AccelData &accelData)
  {
    accelData = mockedAccelDataBNO055;
    return mockedAccelDataStatusBNO055;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getAccelDataICM20948_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::AccelData &accelData)
  {
    accelData = mockedAccelDataICM20948;
    return mockedAccelDataStatusICM20948;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getGyroDataBNO055_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::GyroData &gyroData)
  {
    gyroData = mockedGyroDataBNO055;
    return mockedGyroDataStatusBNO055;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getGyroDataICM20948_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::GyroData &gyroData)
  {
    gyroData = mockedGyroDataICM20948;
    return mockedGyroDataStatusICM20948;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getMagnDataBNO055_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::MagnData &magnData)
  {
    magnData = mockedMagnDataBNO055;
    return mockedMagnDataStatusBNO055;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getMagnDataICM20948_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::MagnData &magnData)
  {
    magnData = mockedMagnDataICM20948;
    return mockedMagnDataStatusICM20948;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getQuatDataBNO055_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::QuatData &quatData)
  {
    quatData = mockedQuatDataBNO055;
    return mockedQuatDataStatusBNO055;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_getQuatDataICM20948_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::QuatData &quatData)
  {
    quatData = mockedQuatDataICM20948;
    return mockedQuatDataStatusICM20948;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_pollQuatData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollQuatDataStatusICM20948;
  }

  Drv::IMU::Status IMUManagerTester ::
      from_pollRawData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollRawDataStatusICM20948;
  }

  // BNO055 setters
  void IMUManagerTester::setMockGetAccelDataBNO055(Drv::IMU::AccelData &data)
  {
    this->mockedAccelDataBNO055 = data;
    this->mockedAccelDataStatusBNO055 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetAccelDataStatusBNO055(Drv::IMU::Status status)
  {
    this->mockedAccelDataStatusBNO055 = status;
  }

  void IMUManagerTester::setMockGetGyroDataBNO055(Drv::IMU::GyroData &data)
  {
    this->mockedGyroDataBNO055 = data;
    this->mockedGyroDataStatusBNO055 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetGyroDataStatusBNO055(Drv::IMU::Status status)
  {
    this->mockedGyroDataStatusBNO055 = status;
  }

  void IMUManagerTester::setMockGetMagnDataBNO055(Drv::IMU::MagnData &data)
  {
    this->mockedMagnDataBNO055 = data;
    this->mockedMagnDataStatusBNO055 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetMagnDataStatusBNO055(Drv::IMU::Status status)
  {
    this->mockedMagnDataStatusBNO055 = status;
  }

  void IMUManagerTester::setMockGetQuatDataBNO055(Drv::IMU::QuatData &data)
  {
    this->mockedQuatDataBNO055 = data;
    this->mockedQuatDataStatusBNO055 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetQuatDataStatusBNO055(Drv::IMU::Status status)
  {
    this->mockedQuatDataStatusBNO055 = status;
  }

  // ICM20948 setters
  void IMUManagerTester::setMockPollQuatDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedPollQuatDataStatusICM20948 = status;
  }

  void IMUManagerTester::setMockPollRawDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedPollRawDataStatusICM20948 = status;
  }

  void IMUManagerTester::setMockGetAccelDataICM20948(Drv::IMU::AccelData &data)
  {
    this->mockedAccelDataICM20948 = data;
    this->mockedAccelDataStatusICM20948 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetAccelDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedAccelDataStatusICM20948 = status;
  }

  void IMUManagerTester::setMockGetGyroDataICM20948(Drv::IMU::GyroData &data)
  {
    this->mockedGyroDataICM20948 = data;
    this->mockedGyroDataStatusICM20948 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetGyroDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedGyroDataStatusICM20948 = status;
  }

  void IMUManagerTester::setMockGetMagnDataICM20948(Drv::IMU::MagnData &data)
  {
    this->mockedMagnDataICM20948 = data;
    this->mockedMagnDataStatusICM20948 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetMagnDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedMagnDataStatusICM20948 = status;
  }

  void IMUManagerTester::setMockGetQuatDataICM20948(Drv::IMU::QuatData &data)
  {
    this->mockedQuatDataICM20948 = data;
    this->mockedQuatDataStatusICM20948 = Drv::IMU::Status::SUCCESS;
  }

  void IMUManagerTester::setMockGetQuatDataStatusICM20948(Drv::IMU::Status status)
  {
    this->mockedQuatDataStatusICM20948 = status;
  }
}
