// ======================================================================
// \title  FRAMDriverTester.hpp
// \author aos
// \brief  hpp file for FRAMDriver component test harness implementation class
// ======================================================================

#ifndef Components_FRAMDriverTester_HPP
#define Components_FRAMDriverTester_HPP

#include "Components/FRAMDriver/FRAMDriverGTestBase.hpp"
#include "Components/FRAMDriver/FRAMDriver.hpp"
#include "etl/vector.h"

namespace Components
{

  class FRAMDriverTester : public FRAMDriverGTestBase
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

    //! Construct object FRAMDriverTester
    FRAMDriverTester();

    //! Destroy object FRAMDriverTester
    ~FRAMDriverTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testConfigureFRAMDriver();
    void testReadFRAMDriver();
    void testWriteFRAMDriver();
    void testReInitFRAMDriver();
    void testFailureReInitFRAMDriver();
    void testFailureAddressOutOfRangeFRAMDriver();
    void testFailureInputBufferEmptyFRAMDriver();
    void testFailureIOInterfaceErrorFRAMDriver();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for chipSelect
    void from_chipSelect_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Fw::Logic &state);

    //! Handler implementation for driver_read
    Drv::SPI::Status from_driver_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &buffer,
        U32 timeoutMs);

    //! Handler implementation for driver_reinit
    void from_driver_reinit_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for driver_write
    Drv::SPI::Status from_driver_write_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &buffer,
        U32 timeoutMs);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockRead(etl::vector<etl::vector<U8, 32>, 32> value);
    void setMockReadStatuses(etl::span<Drv::SPI::Status> statuses);

    void setMockWriteStatuses(etl::span<Drv::SPI::Status> statuses);
    const etl::vector<U8, 32> &getCapturedWriteBuffer(size_t index);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    FRAMDriver component;

    size_t readBufferStatusesIndex = 0;
    size_t readBufferIndex = 0;
    size_t writeBufferIndex = 0;

    static constexpr U32 maxSizeOfOperations = 1024;
    static constexpr U32 maxSizeofBytes = 32;
    etl::vector<etl::vector<U8, maxSizeofBytes>, maxSizeOfOperations> mockedReadBuffers;
    etl::vector<Drv::SPI::Status, maxSizeOfOperations> mockedReadBufferStatuses;
    etl::vector<Drv::SPI::Status, maxSizeOfOperations> mockedWriteBufferStatuses;

    etl::vector<etl::vector<U8, maxSizeofBytes>, maxSizeOfOperations> capturedWriteBuffers;
  };

}

#endif
