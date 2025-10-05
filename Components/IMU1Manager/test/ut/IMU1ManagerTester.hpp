// ======================================================================
// \title  IMU1ManagerTester.hpp
// \author aos
// \brief  hpp file for IMU1Manager component test harness implementation class
// ======================================================================

#ifndef Components_IMU1ManagerTester_HPP
#define Components_IMU1ManagerTester_HPP

#include "Components/IMU1Manager/IMU1ManagerGTestBase.hpp"
#include "Components/IMU1Manager/IMU1Manager.hpp"

namespace Components
{

  class IMU1ManagerTester : public IMU1ManagerGTestBase
  {

  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

    // Queue depth supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object IMU1ManagerTester
    IMU1ManagerTester();

    //! Destroy object IMU1ManagerTester
    ~IMU1ManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testGetDataSuccessIMU1Manager();
    void testGetDataFailedIMU1Manager();


  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getAccelData
    Drv::IMU::Status from_getAccelData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::AccelData &accelData);
    
    //! Handler implementation for getCalibrationData
    Drv::IMU::Status from_getCalibrationData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::CalibrationData& calibData
    );

    //! Handler implementation for getGyroData
    Drv::IMU::Status from_getGyroData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::GyroData &gyroData);

    //! Handler implementation for getMagnData
    Drv::IMU::Status from_getMagnData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::MagnData &magnData);

    //! Handler implementation for getQuatData
    Drv::IMU::Status from_getQuatData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::QuatData &quatData);

    //! Handler implementation for pollQuatData
    Drv::IMU::Status from_pollQuatData_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for pollRawData
    Drv::IMU::Status from_pollRawData_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();
    void setMockPollQuatDataStatus(Drv::IMU::Status status);
    void setMockPollRawDataStatus(Drv::IMU::Status status);

    void setMockGetAccelData(Drv::IMU::AccelData &data);
    void setMockGetAccelDataStatus(Drv::IMU::Status status);

    void setMockGetGyroData(Drv::IMU::GyroData &data);
    void setMockGetGyroDataStatus(Drv::IMU::Status status);

    void setMockGetMagnData(Drv::IMU::MagnData &data);
    void setMockGetMagnDataStatus(Drv::IMU::Status status);

    void setMockGetQuatData(Drv::IMU::QuatData &data);
    void setMockGetQuatDataStatus(Drv::IMU::Status status);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    IMU1Manager component;

    Drv::IMU::Status mockedPollRawDataStatus;
    Drv::IMU::Status mockedPollQuatDataStatus;

    Drv::IMU::AccelData mockedAccelData;
    Drv::IMU::Status mockedAccelDataStatus;

    Drv::IMU::GyroData mockedGyroData;
    Drv::IMU::Status mockedGyroDataStatus;

    Drv::IMU::MagnData mockedMagnData;
    Drv::IMU::Status mockedMagnDataStatus;

    Drv::IMU::QuatData mockedQuatData;
    Drv::IMU::Status mockedQuatDataStatus;
  };

}

#endif
