// ======================================================================
// \title  NORManagerTester.hpp
// \author aos
// \brief  hpp file for NORManager component test harness implementation class
// ======================================================================

#ifndef Components_NORManagerTester_HPP
#define Components_NORManagerTester_HPP

#include "Components/NORManager/NORManagerGTestBase.hpp"
#include "Components/NORManager/NORManager.hpp"
#include "etl/vector.h"

namespace Components
{

  class NORManagerTester : public NORManagerGTestBase
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

    //! Construct object NORManagerTester
    NORManagerTester();

    //! Destroy object NORManagerTester
    ~NORManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testRequestEraseNORManager();
    void testRequestReadNORManager();
    void testRequestWriteNORManager();
    void testRequestEraseNORManagerFailed();
    void testRequestReadNORManagerFailed();
    void testRequestWriteNORManagerFailed();
    void testGetMemoryAndSectorSizeNORManager();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for erase
    Drv::NOR::ErrorCode from_erase_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        U32 length);

    //! Handler implementation for eraseStatus
    void from_eraseStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status &status);

    //! Handler implementation for getInMemorySize
    U32 from_getInMemorySize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for getInSectorSize
    U32 from_getInSectorSize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for read
    Drv::NOR::ErrorCode from_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        Fw::Buffer &dataInBuff);

    //! Handler implementation for readStatus
    void from_readStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status &status);

    //! Handler implementation for write
    Drv::NOR::ErrorCode from_write_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        Fw::Buffer &dataOutBuff);

    //! Handler implementation for writeStatus
    void from_writeStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status &status);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockRequestReadStatus(Drv::NOR::ErrorCode status);
    void setMockReadBuffer(etl::vector<U8, 8192> value);
    etl::vector<U8, 8192> &getCaptureWriteBuffer();
    void setMockRequestWriteStatus(Drv::NOR::ErrorCode status);
    void setMockRequestEraseStatus(Drv::NOR::ErrorCode status);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    NORManager component;

    static constexpr U32 memorySize = 0x08000000;
    static constexpr U32 sectorSize = 4096;

    Drv::NOR::ErrorCode mockedReadRequestStatus;
    Drv::NOR::ErrorCode mockedWriteRequestStatus;
    Drv::NOR::ErrorCode mockedEraseRequestStatus;

    static constexpr U32 maxSizeOfBytes = 8192;
    etl::vector<U8, maxSizeOfBytes> mockedReadBuffer;
    etl::vector<U8, maxSizeOfBytes> capturedWriteBuffer;

    Drv::NORManager::Status receivedReadStatus;
    Drv::NORManager::Status receivedWriteStatus;
    Drv::NORManager::Status receivedEraseStatus;
  };

}

#endif
