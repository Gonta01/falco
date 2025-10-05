// ======================================================================
// \title  IMU2ManagerTester.hpp
// \author aos
// \brief  hpp file for IMU2Manager component test harness implementation class
// ======================================================================

#ifndef Components_IMU2ManagerTester_HPP
#define Components_IMU2ManagerTester_HPP

#include "Components/IMU2Manager/IMU2ManagerGTestBase.hpp"
#include "Components/IMU2Manager/IMU2Manager.hpp"

namespace Components
{

  class IMU2ManagerTester : public IMU2ManagerGTestBase
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

    //! Construct object IMU2ManagerTester
    IMU2ManagerTester();

    //! Destroy object IMU2ManagerTester
    ~IMU2ManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testGetRawDataSuccessIMU2Manager();
    void testGetRawDataFailedIMU2Manager();

    void testGetQuatDataSuccessIMU2Manager();
    void testGetQuatDataFailedIMU2Manager();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getAccelData
    Drv::IMU::Status from_getAccelData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::AccelData &accelData);

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

    //! Handler implementation for getRawData
    Drv::IMU::Status from_getRawData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::RawData &rawData);

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

    void setMockGetRawData(Drv::IMU::RawData &data);
    void setMockGetRawDataStatus(Drv::IMU::Status status);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    IMU2Manager component;

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

    Drv::IMU::RawData mockedRawData;
    Drv::IMU::Status mockedRawDataStatus;
  };

}

#endif
