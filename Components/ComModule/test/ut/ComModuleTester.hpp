// ======================================================================
// \title  ComModuleTester.hpp
// \author aos
// \brief  hpp file for ComModule component test harness implementation class
// ======================================================================

#ifndef Components_ComModuleTester_HPP
#define Components_ComModuleTester_HPP

#include "Components/ComModule/ComModuleGTestBase.hpp"
#include "Components/ComModule/ComModule.hpp"
#include "etl/vector.h"

namespace Components
{

  class ComModuleTester : public ComModuleGTestBase
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

    //! Construct object ComModuleTester
    ComModuleTester();

    //! Destroy object ComModuleTester
    ~ComModuleTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testComModuleRoutine();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for allocate
    Fw::Buffer from_allocate_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 size                 //!< The requested size
    );

    //! Handler implementation for deallocate
    void from_deallocate_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &fwBuffer     //!< The buffer
    );

    //! Handler implementation for read
    U32 from_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &buffer,      //!< Buffer with data to read to
        U32 timeoutMs);

    //! Handler implementation for ready
    void from_ready_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for recv
    void from_recv_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus);

    //! Handler implementation for write
    Drv::UART::WriteStatus from_write_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &buffer,      //!< Buffer with data to write from
        U32 timeoutMs);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockWriteStatus(Drv::UART::WriteStatus status);
    void setMockReadBuffer(etl::vector<U8, 64> value);
    Drv::RecvStatus &getCapturedRecvStatus();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    ComModule component;

    Drv::UART::WriteStatus mockedWriteStatus;
    etl::vector<U8, 64> mockedReadBuffers;

    Drv::RecvStatus capturedRecvStatus;
  };

}

#endif
