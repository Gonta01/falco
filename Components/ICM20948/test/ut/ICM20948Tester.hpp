// ======================================================================
// \title  ICM20948Tester.hpp
// \author aos
// \brief  hpp file for ICM20948 component test harness implementation class
// ======================================================================

#ifndef Components_ICM20948Tester_HPP
#define Components_ICM20948Tester_HPP

#include "Components/ICM20948/ICM20948GTestBase.hpp"
#include "Components/ICM20948/ICM20948.hpp"
#include "etl/vector.h"
namespace Components
{

  class ICM20948Tester : public ICM20948GTestBase
  {

  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object ICM20948Tester
    ICM20948Tester();

    //! Destroy object ICM20948Tester
    ~ICM20948Tester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testIsConnectedICM20948();
    void testDeviceResetICM20948();
    void testConfigClockSourceICM20948();
    void testEnableGyroAndAccelICM20948();
    void testEnableFIFOFalseICM20948();
    void testDisableDMPICM20948();
    void testSetConfigICM20948();
    void testConfigureFIFOICM20948();
    void testConfigureInterruptPinICM20948();
    void testConfigureDataReadyInterruptICM20948();
    void testConfigureGyroSampleRateICM20948();
    void testConfigureAccelSampleRateICM20948();
    void testInitMagnetometerICM20948();
    void testCalibrateGyroOffsetsICM20948();
    void testEnableFIFOTrueICM20948();
    void testResetFIFOICM20948();

    void testPollRawDataICM20948();
    void testPollQuatDataICM20948();

    void testFailureNotInitedICM20948();
    void testFailureReadICM20948();
    void testFailureWriteICM20948();
    void testFailureReadNotEqualToWrittenICM20948();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for intPin
    void from_intPin_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Logic &state);

    //! Handler implementation for read
    Drv::I2C::Status from_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr,                //!< I2C slave device address
        Fw::Buffer &serBuffer,   //!< Buffer with data to write from
        U32 timeout              //!< Max time of waititing for operation ending
    );

    //! Handler implementation for write
    Drv::I2C::Status from_write_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr,                //!< I2C slave device address
        Fw::Buffer &serBuffer,   //!< Buffer with data to read to
        U32 timeout              //!< Max time of waititing for operation ending
    );

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockReadBuffers(etl::vector<etl::vector<U8, 32>, 32> value);
    void setMockReadCalibrateCoef();

    void setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses);
    void setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    ICM20948 component;

    size_t writeBufferStatusesIndex = 0;
    etl::vector<Drv::I2C::Status, 1024> mockedWriteBufferStatuses;

    size_t readBufferStatusesIndex = 0;
    size_t readBufferIndex = 0;
    etl::vector<etl::vector<U8, 32>, 1024> mockedReadBuffers;
    etl::vector<Drv::I2C::Status, 1024> mockedReadBufferStatuses;
  };

}

#endif
