// ======================================================================
// \title  NORDriverTester.hpp
// \author aos
// \brief  hpp file for NORDriver component test harness implementation class
// ======================================================================

#ifndef Components_NORDriverTester_HPP
#define Components_NORDriverTester_HPP

#include "Components/NORDriver/NORDriverGTestBase.hpp"
#include "Components/NORDriver/NORDriver.hpp"
#include "etl/vector.h"
namespace Components
{

  class NORDriverTester : public NORDriverGTestBase
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

    //! Construct object NORDriverTester
    NORDriverTester();

    //! Destroy object NORDriverTester
    ~NORDriverTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testConfigureNORDriver();
    void testReadNORDriver();
    void testWriteNORDriver();
    void testEraseNORDriver();
    void testGetMemoryAndSectorSizeNORDriver();

    void testFailureWrongMemoryIdNORDriver();
    void testFailureInputBufferEmptyNORDriver();
    void testFailureAccessOutOfRangeNORDriver();
    void testFailureUnalignedAccessNORDriver();
    void testFailureIOInterfaceErrorNORDriver();
    void testFailureFlashMemoryBusyNORDriver();
 

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for commandWrite
    Drv::QSPI::QSPIError from_commandWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QPSICommandConfig &config,
        U32 address);

    //! Handler implementation for memoryRead
    Drv::QSPI::QSPIError from_memoryRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIMemoryConfig &config,
        U32 address,
        Fw::Buffer &buff);

    //! Handler implementation for memoryWrite
    Drv::QSPI::QSPIError from_memoryWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIMemoryConfig &config,
        U32 address,
        Fw::Buffer &buff);

    //! Handler implementation for registerRead
    Drv::QSPI::QSPIError from_registerRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIRegisterConfig &config,
        Fw::Buffer &buff);

    //! Handler implementation for registerWrite
    Drv::QSPI::QSPIError from_registerWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIRegisterConfig &config,
        Fw::Buffer &buff);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockReadRegister(etl::vector<etl::vector<U8, 32>, 32> value);
    void setMockReadMemory(etl::vector<etl::vector<U8, 32>, 32> value);

    void setMockReadRegisterStatuses(etl::span<Drv::QSPI::QSPIError> statuses);
    void setMockReadMemoryStatuses(etl::span<Drv::QSPI::QSPIError> statuses);

    const Fw::Buffer getCapturedWriteBuffer();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    NORDriver component;

    size_t readRegisterBufferStatusesIndex = 0;
    size_t readRegisterBufferIndex = 0;

    size_t readMemoryBufferStatusesIndex = 0;
    size_t readMemoryBufferIndex = 0;

    static constexpr U32 maxSizeOfBuffer = 32;
    static constexpr U32 maxSizeOfOperations = 1024;

    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> mockedReadRegisterBuffers;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> mockedReadMemoryBuffers;
    etl::vector<Drv::QSPI::QSPIError, maxSizeOfOperations> mockedReadRegisterBufferStatuses;
    etl::vector<Drv::QSPI::QSPIError, maxSizeOfOperations> mockedReadMemoryBufferStatuses;

    U8 bufferData[maxSizeOfBuffer];
    Fw::Buffer capturedWriteBuffer = Fw::Buffer(bufferData, sizeof(bufferData));
  };

}

#endif
