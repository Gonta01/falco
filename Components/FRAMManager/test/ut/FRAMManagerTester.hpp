// ======================================================================
// \title  FRAMManagerTester.hpp
// \author aos
// \brief  hpp file for FRAMManager component test harness implementation class
// ======================================================================

#ifndef Components_FRAMManagerTester_HPP
#define Components_FRAMManagerTester_HPP

#include "Components/FRAMManager/FRAMManagerGTestBase.hpp"
#include "Components/FRAMManager/FRAMManager.hpp"
#include "etl/vector.h"
#include "etl/span.h"
namespace Components
{

  class FRAMManagerTester : public FRAMManagerGTestBase
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

    //! Construct object FRAMManagerTester
    FRAMManagerTester();

    //! Destroy object FRAMManagerTester
    ~FRAMManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testRequestReadFRAMManager();
    void testRequestWriteFRAMManager();
    void testRequestWakeupFRAMManager();
    void testRequestSleepFRAMManager();
    void testFailureRequestReadFRAMManager();
    void testFailureRequestWriteFRAMManager();
    void testFailureRequestWakeupFRAMManager();
    void testFailureRequestSleepFRAMManager();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for read
    Drv::FRAM::InStatus from_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr,
        Fw::Buffer &buffer);

    //! Handler implementation for reinit
    void from_reinit_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for sleep
    Drv::FRAM::InStatus from_sleep_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for status
    void from_status_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::FRAM::ExtStatus &status);

    //! Handler implementation for wakeup
    Drv::FRAM::InStatus from_wakeup_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for write
    Drv::FRAM::InStatus from_write_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr,
        Fw::Buffer &buffer);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockWakeupStatus(Drv::FRAM::InStatus status);
    void setMockSleepStatus(Drv::FRAM::InStatus status);

    void setMockRead(etl::vector<etl::vector<U8, 8192>, 32> value);
    void setMockReadStatuses(etl::span<Drv::FRAM::InStatus> statuses);

    void setMockWriteStatuses(etl::span<Drv::FRAM::InStatus> statuses);
    const etl::vector<U8, 8192> &getCapturedWriteBuffer(size_t index);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    FRAMManager component;

    Drv::FRAM::InStatus mockedWakeupStatus;
    Drv::FRAM::InStatus mockedSleepStatus;
    Drv::FRAM::ExtStatus recievedExtStatus;

    size_t readBufferStatusesIndex = 0;
    size_t readBufferIndex = 0;
    size_t writeBufferIndex = 0;

    static constexpr U32 maxSizeOfBytes = 8192;
    static constexpr U32 maxSizeOfOperations = 32;
    etl::vector<etl::vector<U8, maxSizeOfBytes>, maxSizeOfOperations> mockedReadBuffers;
    etl::vector<Drv::FRAM::InStatus, maxSizeOfOperations> mockedReadBufferStatuses;
    etl::vector<Drv::FRAM::InStatus, maxSizeOfOperations> mockedWriteBufferStatuses;

    etl::vector<etl::vector<U8, maxSizeOfBytes>, maxSizeOfOperations> capturedWriteBuffers;
  };

}

#endif
