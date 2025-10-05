// ======================================================================
// \title  IMUManagerTester.hpp
// \author aos
// \brief  hpp file for IMUManager component test harness implementation class
// ======================================================================

#ifndef Components_IMUManagerTester_HPP
#define Components_IMUManagerTester_HPP

#include "Components/IMUManager/IMUManagerGTestBase.hpp"
#include "Components/IMUManager/IMUManager.hpp"

namespace Components
{

  class IMUManagerTester : public IMUManagerGTestBase
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

    //! Construct object IMUManagerTester
    IMUManagerTester();

    //! Destroy object IMUManagerTester
    ~IMUManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testGetDataSuccessICM20948();
    void testGetDataFailedICM20948();

    void testGetRawDataSuccessBNO055();
    void testGetRawDataFailedBNO055();

    void testGetQuatDataSuccessBNO055();
    void testGetQuatDataFailedBNO055();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getAccelDataBNO055
    Drv::IMU::Status from_getAccelDataBNO055_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::AccelData &accelData);

    //! Handler implementation for getAccelDataICM20948
    Drv::IMU::Status from_getAccelDataICM20948_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::AccelData &accelData);

    //! Handler implementation for getGyroDataBNO055
    Drv::IMU::Status from_getGyroDataBNO055_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::GyroData &gyroData);

    //! Handler implementation for getGyroDataICM20948
    Drv::IMU::Status from_getGyroDataICM20948_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::GyroData &gyroData);

    //! Handler implementation for getMagnDataBNO055
    Drv::IMU::Status from_getMagnDataBNO055_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::MagnData &magnData);

    //! Handler implementation for getMagnDataICM20948
    Drv::IMU::Status from_getMagnDataICM20948_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::MagnData &magnData);

    //! Handler implementation for getQuatDataBNO055
    Drv::IMU::Status from_getQuatDataBNO055_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::QuatData &quatData);

    //! Handler implementation for getQuatDataICM20948
    Drv::IMU::Status from_getQuatDataICM20948_handler(
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

    void setMockGetAccelDataBNO055(Drv::IMU::AccelData &data);
    void setMockGetAccelDataStatusBNO055(Drv::IMU::Status status);

    void setMockGetGyroDataBNO055(Drv::IMU::GyroData &data);
    void setMockGetGyroDataStatusBNO055(Drv::IMU::Status status);

    void setMockGetMagnDataBNO055(Drv::IMU::MagnData &data);
    void setMockGetMagnDataStatusBNO055(Drv::IMU::Status status);

    void setMockGetQuatDataBNO055(Drv::IMU::QuatData &data);
    void setMockGetQuatDataStatusBNO055(Drv::IMU::Status status);

    void setMockPollQuatDataStatusICM20948(Drv::IMU::Status status);
    void setMockPollRawDataStatusICM20948(Drv::IMU::Status status);

    void setMockGetAccelDataICM20948(Drv::IMU::AccelData &data);
    void setMockGetAccelDataStatusICM20948(Drv::IMU::Status status);

    void setMockGetGyroDataICM20948(Drv::IMU::GyroData &data);
    void setMockGetGyroDataStatusICM20948(Drv::IMU::Status status);

    void setMockGetMagnDataICM20948(Drv::IMU::MagnData &data);
    void setMockGetMagnDataStatusICM20948(Drv::IMU::Status status);

    void setMockGetQuatDataICM20948(Drv::IMU::QuatData &data);
    void setMockGetQuatDataStatusICM20948(Drv::IMU::Status status);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    IMUManager component;

    Drv::IMU::AccelData mockedAccelDataBNO055;
    Drv::IMU::Status mockedAccelDataStatusBNO055;

    Drv::IMU::GyroData mockedGyroDataBNO055;
    Drv::IMU::Status mockedGyroDataStatusBNO055;

    Drv::IMU::MagnData mockedMagnDataBNO055;
    Drv::IMU::Status mockedMagnDataStatusBNO055;

    Drv::IMU::QuatData mockedQuatDataBNO055;
    Drv::IMU::Status mockedQuatDataStatusBNO055;

    Drv::IMU::Status mockedPollRawDataStatusICM20948;
    Drv::IMU::Status mockedPollQuatDataStatusICM20948;

    Drv::IMU::AccelData mockedAccelDataICM20948;
    Drv::IMU::Status mockedAccelDataStatusICM20948;

    Drv::IMU::GyroData mockedGyroDataICM20948;
    Drv::IMU::Status mockedGyroDataStatusICM20948;

    Drv::IMU::MagnData mockedMagnDataICM20948;
    Drv::IMU::Status mockedMagnDataStatusICM20948;

    Drv::IMU::QuatData mockedQuatDataICM20948;
    Drv::IMU::Status mockedQuatDataStatusICM20948;
  };

}

#endif
