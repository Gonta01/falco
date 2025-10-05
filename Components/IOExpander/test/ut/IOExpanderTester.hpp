// ======================================================================
// \title  IOExpanderTester.hpp
// \author aos
// \brief  hpp file for IOExpander component test harness implementation class
// ======================================================================

#ifndef Components_IOExpanderTester_HPP
#define Components_IOExpanderTester_HPP

#include "Components/IOExpander/IOExpanderGTestBase.hpp"
#include "Components/IOExpander/IOExpander.hpp"
#include "etl/vector.h"

namespace Components
{

  class IOExpanderTester : public IOExpanderGTestBase
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

    //! Construct object IOExpanderTester
    IOExpanderTester();

    //! Destroy object IOExpanderTester
    ~IOExpanderTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testConfigureIOExpander();
    void testReadAnalogInputIOExpander();
    void testReadDigitalMultipleInputsIOExpander();
    void testWriteDigitalMultipleOutputsIOExpander();
    void testReadDigitalSingleInputIOExpander();
    void testWriteDigitalSingleOutputIOExpander();
    void testFailureNotInitedIOExpander();
    void testFailureWrongChannelConfigurationIOExpander();
    void testFailureCommunicationErrorIOExpander();
    void testFailureCRCErrorIOExpander();
    void testFailureCalibrationUnfinishedIOExpander();
  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for alert
    void from_alert_handler(
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

    //! Handler implementation for writeRead
    Drv::I2C::Status from_writeRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr,                //!< I2C slave device address
        Fw::Buffer &writeBuffer, //!< Buffer to write data to the i2c device
        Fw::Buffer &readBuffer,  //!< Buffer to read back data from the i2c device, must set size when passing in read buffer
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

    void setMockReadBuffers(etl::vector<etl::vector<U8, 32>, 1024> value);

    void setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses);
    void setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses);

    void setMockWriteReadBuffers(etl::vector<etl::vector<U8, 32>, 1024> value);
    void setMockWriteReadBufferStatuses(etl::span<Drv::I2C::Status> statuses);
    const etl::vector<U8, 32> &getCapturedWriteBuffer();
    void configHelperFunction(Components::IOExpander::ConfigTable *config);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    IOExpander component;

    static constexpr U32 maxSizeOfOperations = 1024;
    static constexpr U32 maxSizeOfDataBuffer = 32;

    U32 writeBufferStatusesIndex = 0;
    etl::vector<Drv::I2C::Status, maxSizeOfOperations> mockedWriteBufferStatuses;

    U32 readBufferStatusesIndex = 0;
    U32 readBufferIndex = 0;
    etl::vector<etl::vector<U8, maxSizeOfDataBuffer>, maxSizeOfOperations> mockedReadBuffers;
    etl::vector<Drv::I2C::Status, maxSizeOfOperations> mockedReadBufferStatuses;

    U32 writeReadBufferStatusesIndex = 0;
    U32 writeReadBufferIndex = 0;
    etl::vector<etl::vector<U8, maxSizeOfDataBuffer>, maxSizeOfOperations> mockedWriteReadBuffers;
    etl::vector<Drv::I2C::Status, maxSizeOfOperations> mockedWriteReadBufferStatuses;

    etl::vector<U8, maxSizeOfDataBuffer> capturedWriteData;
  };

}

#endif
