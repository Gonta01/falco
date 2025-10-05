// ======================================================================
// \title  IMU1ManagerTester.cpp
// \author aos
// \brief  cpp file for IMU1Manager component test harness implementation class
// ======================================================================

#include "IMU1ManagerTester.hpp"
#include "STest/Pick/Pick.hpp"
#include "STest/Random/Random.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  IMU1ManagerTester ::
      IMU1ManagerTester() : IMU1ManagerGTestBase("IMU1ManagerTester", IMU1ManagerTester::MAX_HISTORY_SIZE),
                            component("IMU1Manager")
  {
    this->initComponents();
    this->connectPorts();
  }

  IMU1ManagerTester ::
      ~IMU1ManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void IMU1ManagerTester ::
      testGetDataSuccessIMU1Manager()
  {
    Drv::IMU::AccelData accelData(0.01, -0.98, 0.05);
    this->setMockGetAccelData(accelData);
    Drv::IMU::GyroData gyroData(0.5, -0.3, 0.0);
    this->setMockGetGyroData(gyroData);
    Drv::IMU::MagnData magnData(30.2, -47.6, 12.0);
    this->setMockGetMagnData(magnData);
    Drv::IMU::QuatData quatData(0.707, 0.0, 0.707, 0.0);
    this->setMockGetQuatData(quatData);

    this->setMockPollRawDataStatus(Drv::IMU::Status::SUCCESS);
    this->setMockPollQuatDataStatus(Drv::IMU::Status::SUCCESS);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getData(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_EQ(actualData, expectedData);

    Drv::IMU::Quaternion actualQuat;
    this->invoke_to_getQuaternion(0, actualQuat);

    Drv::IMU::Quaternion expectedQuat(quatData, true);
    ASSERT_EQ(actualQuat, expectedQuat);
  }

  void IMU1ManagerTester ::
      testGetDataFailedIMU1Manager()
  {
    Drv::IMU::AccelData accelData(0.01, -0.98, 0.05);
    this->setMockGetAccelData(accelData);
    Drv::IMU::GyroData gyroData(0.5, -0.3, 0.0);
    this->setMockGetGyroData(gyroData);
    Drv::IMU::MagnData magnData(30.2, -47.6, 12.0);
    this->setMockGetMagnData(magnData);
    Drv::IMU::QuatData quatData(0.707, 0.0, 0.707, 0.0);
    this->setMockGetQuatData(quatData);

    this->setMockPollRawDataStatus(Drv::IMU::Status::FAILED);
    this->setMockPollQuatDataStatus(Drv::IMU::Status::FAILED);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::IMU::Data actualData;
    this->invoke_to_getData(0, actualData);

    Drv::IMU::CalibrationData calibrData(0, 0, 0, 0, 0, 0, 0 ,0, 0);
    Drv::IMU::Data expectedData(accelData, true, gyroData, true, magnData, true, calibrData, true);
    ASSERT_NE(actualData, expectedData);

    Drv::IMU::Quaternion actualQuat;
    this->invoke_to_getQuaternion(0, actualQuat);

    Drv::IMU::Quaternion expectedQuat(quatData, true);
    ASSERT_NE(actualQuat, expectedQuat);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::IMU::Status IMU1ManagerTester ::
      from_getAccelData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::AccelData &accelData)
  {
    accelData = mockedAccelData;
    return mockedAccelDataStatus;
  }

  Drv::IMU::Status IMU1ManagerTester ::
      from_getGyroData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::GyroData &gyroData)
  {
    gyroData = mockedGyroData;
    return mockedGyroDataStatus;
  }

    Drv::IMU::Status IMU1ManagerTester ::
    from_getCalibrationData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::CalibrationData& calibData
    )
  {
    Drv::IMU::CalibrationData mockCalibData(0, 0, 0, 0, 0, 0, 0, 0, 0);
    calibData = mockCalibData;
    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status IMU1ManagerTester ::
      from_getMagnData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::MagnData &magnData)
  {
    magnData = mockedMagnData;
    return mockedMagnDataStatus;
  }

  Drv::IMU::Status IMU1ManagerTester ::
      from_getQuatData_handler(
          NATIVE_INT_TYPE portNum,
          Drv::IMU::QuatData &quatData)
  {
    quatData = mockedQuatData;
    return mockedQuatDataStatus;
  }

  Drv::IMU::Status IMU1ManagerTester ::
      from_pollQuatData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollQuatDataStatus;
  }

  Drv::IMU::Status IMU1ManagerTester ::
      from_pollRawData_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedPollRawDataStatus;
  }

  void IMU1ManagerTester ::
      setMockPollQuatDataStatus(Drv::IMU::Status status)
  {
    mockedPollQuatDataStatus = status;
  }
  void IMU1ManagerTester ::
      setMockPollRawDataStatus(Drv::IMU::Status status)
  {
    mockedPollRawDataStatus = status;
  }

  void IMU1ManagerTester ::
      setMockGetAccelData(Drv::IMU::AccelData &data)
  {
    mockedAccelData = data;
    mockedAccelDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU1ManagerTester ::
      setMockGetAccelDataStatus(Drv::IMU::Status status)
  {
    mockedAccelDataStatus = status;
  }

  void IMU1ManagerTester ::
      setMockGetGyroData(Drv::IMU::GyroData &data)
  {
    mockedGyroData = data;
    mockedGyroDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU1ManagerTester ::
      setMockGetGyroDataStatus(Drv::IMU::Status status)
  {
    mockedGyroDataStatus = status;
  }

  void IMU1ManagerTester ::
      setMockGetMagnData(Drv::IMU::MagnData &data)
  {
    mockedMagnData = data;
    mockedMagnDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU1ManagerTester ::
      setMockGetMagnDataStatus(Drv::IMU::Status status)
  {
    mockedMagnDataStatus = status;
  }

  void IMU1ManagerTester ::
      setMockGetQuatData(Drv::IMU::QuatData &data)
  {
    mockedQuatData = data;
    mockedQuatDataStatus = Drv::IMU::Status::SUCCESS;
  }
  void IMU1ManagerTester ::
      setMockGetQuatDataStatus(Drv::IMU::Status status)
  {
    mockedQuatDataStatus = status;
  }
}
