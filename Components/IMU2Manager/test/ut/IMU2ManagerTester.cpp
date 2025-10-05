// ======================================================================
// \title  IMU2ManagerTester.cpp
// \author aos
// \brief  cpp file for IMU2Manager component test harness implementation class
// ======================================================================

#include "IMU2ManagerTester.hpp"
#include "STest/Pick/Pick.hpp"
#include "STest/Random/Random.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  IMU2ManagerTester ::
      IMU2ManagerTester() : IMU2ManagerGTestBase("IMU2ManagerTester", IMU2ManagerTester::MAX_HISTORY_SIZE),
                            component("IMU2Manager")
  {
    this->initComponents();
    this->connectPorts();
  }

  IMU2ManagerTester ::
      ~IMU2ManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void IMU2ManagerTester ::
      testGetRawDataSuccessIMU2Manager()
  {
    Drv::IMU::AccelData accelData(1.0, 2.0, 3.0);
    Drv::IMU::GyroData gyroData(1.0, 2.0, 3.0);
    Drv::IMU::MagnData magnData(1.0, 2.0, 3.0);
    Drv::IMU::RawData rawData(accelData, gyroData, magnData);
    this->setMockGetRawData(rawData);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getData(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0, 0, 0);
    actualData.setCalibration(calibrData);
    actualData.setCalibrationValid(true);

    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_EQ(actualData, expectedData);
  }

  void IMU2ManagerTester ::
      testGetRawDataFailedIMU2Manager()
  {
    Drv::IMU::AccelData accelData(1.0, 2.0, 3.0);
    Drv::IMU::GyroData gyroData(1.0, 2.0, 3.0);
    Drv::IMU::MagnData magnData(1.0, 2.0, 3.0);
    Drv::IMU::RawData rawData(accelData, gyroData, magnData);
    this->setMockGetRawData(rawData);
    this->setMockGetRawDataStatus(Drv::IMU::Status::FAILED);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getData(0, actualData);
    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0, 0, 0);

    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_NE(actualData, expectedData);
  }

  void IMU2ManagerTester ::
      testGetQuatDataSuccessIMU2Manager()
  {
    Drv::IMU::QuatData quatData(0.92, 0.1, 0.2, 0.3);
    this->setMockGetQuatData(quatData);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Quaternion actualQuaternion;
    this->invoke_to_getQuaternion(0, actualQuaternion);

    Drv::IMU::Quaternion expectedQuaternion(quatData, true);
    ASSERT_EQ(actualQuaternion, expectedQuaternion);
  }
  void IMU2ManagerTester ::
      testGetQuatDataFailedIMU2Manager()
  {
    Drv::IMU::QuatData quatData(0.92, 0.1, 0.2, 0.3);
    this->setMockGetQuatData(quatData);
    this->setMockGetQuatDataStatus(Drv::IMU::Status::FAILED);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Quaternion actualQuaternion;
    this->invoke_to_getQuaternion(0, actualQuaternion);

    Drv::IMU::Quaternion expectedQuaternion(quatData, true);
    ASSERT_NE(actualQuaternion, expectedQuaternion);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::IMU::Status IMU2ManagerTester ::
      from_getAccelData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::AccelData &accelData)
  {
    accelData = mockedAccelData;
    return mockedAccelDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_getGyroData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::GyroData &gyroData)
  {
    gyroData = mockedGyroData;
    return mockedGyroDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_getMagnData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::MagnData &magnData)
  {
    magnData = mockedMagnData;
    return mockedMagnDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_getQuatData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::QuatData &quatData)
  {
    quatData = mockedQuatData;
    return mockedQuatDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_getRawData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::RawData &rawData)
  {
    rawData = mockedRawData;
    return mockedRawDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_pollQuatData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollQuatDataStatus;
  }

  Drv::IMU::Status IMU2ManagerTester ::
      from_pollRawData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollRawDataStatus;
  }

  void IMU2ManagerTester ::
      setMockPollQuatDataStatus(Drv::IMU::Status status)
  {
    mockedPollQuatDataStatus = status;
  }
  void IMU2ManagerTester ::
      setMockPollRawDataStatus(Drv::IMU::Status status)
  {
    mockedPollRawDataStatus = status;
  }

  void IMU2ManagerTester ::
      setMockGetAccelData(Drv::IMU::AccelData &data)
  {
    mockedAccelData = data;
    mockedAccelDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU2ManagerTester ::
      setMockGetAccelDataStatus(Drv::IMU::Status status)
  {
    mockedAccelDataStatus = status;
  }

  void IMU2ManagerTester ::
      setMockGetGyroData(Drv::IMU::GyroData &data)
  {
    mockedGyroData = data;
    mockedGyroDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU2ManagerTester ::
      setMockGetGyroDataStatus(Drv::IMU::Status status)
  {
    mockedGyroDataStatus = status;
  }

  void IMU2ManagerTester ::
      setMockGetMagnData(Drv::IMU::MagnData &data)
  {
    mockedMagnData = data;
    mockedMagnDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU2ManagerTester ::
      setMockGetMagnDataStatus(Drv::IMU::Status status)
  {
    mockedMagnDataStatus = status;
  }

  void IMU2ManagerTester ::
      setMockGetQuatData(Drv::IMU::QuatData &data)
  {
    mockedQuatData = data;
    mockedQuatDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU2ManagerTester ::
      setMockGetQuatDataStatus(Drv::IMU::Status status)
  {
    mockedQuatDataStatus = status;
  }

  void IMU2ManagerTester ::
      setMockGetRawData(Drv::IMU::RawData &data)
  {
    mockedRawData = data;
    mockedRawDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU2ManagerTester ::setMockGetRawDataStatus(Drv::IMU::Status status)
  {
    mockedRawDataStatus = status;
  }
}
