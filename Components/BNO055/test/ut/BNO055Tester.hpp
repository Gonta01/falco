// ======================================================================
// \title  BNO055Tester.hpp
// \author aos
// \brief  hpp file for BNO055 component test harness implementation class
// ======================================================================

#ifndef Components_BNO055Tester_HPP
#define Components_BNO055Tester_HPP

#include "Components/BNO055/BNO055GTestBase.hpp"
#include "Components/BNO055/BNO055.hpp"
#include "etl/vector.h"
namespace Components
{

  class BNO055Tester : public BNO055GTestBase
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

    //! Construct object BNO055Tester
    BNO055Tester();

    //! Destroy object BNO055Tester
    ~BNO055Tester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testReadBNO055();
    void testWriteBNO055();

    void testGetAccelBNO055();
    void testGetGyroBNO055();
    void testGetMagnBNO055();
    void testGetRawBNO055();
    void testGetQuatBNO055();
    void testConfigureBNO055();

    void testFailureWriteFailBNO055();
    void testFailureReadFailBNO055();
    void testFailureRegmapInvalidAddressBNO055();
    void testFailureRegmapWriteDisabledBNO055();
    void testFailureWrongStartByteBNO055();
    void testFailureMaxLengthErrorBNO055();
    void testFailureMinLengthErrorBNO055();
    void testFailureRecieveCharacterTimeoutBNO055();

    void testFailureResponseHeaderMismatchBNO055();
    void testFailureResponseSizeMismatchBNO055();
    void testFailureReceiveBNO055();
    void testFailureSendBNO055();
    void testFailureInitBNO055();
    void testFailureSelfTestBNO055();



  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getNumOfBytes
    U32 from_getNumOfBytes_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    );

    //! Handler implementation for read
    U32 from_read_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer &buffer,      //!< Buffer with data to read to
        U32 timeoutMs);

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

    void setMockReadBuffers(etl::vector<etl::vector<U8, 128>, 1024> value);

    void setMockWriteBufferStatuses(etl::span<Drv::UART::WriteStatus> statuses);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    BNO055 component;

    size_t writeBufferStatusesIndex = 0;
    etl::vector<Drv::UART::WriteStatus, 1024> mockedWriteBufferStatuses;

    etl::vector<bool, 1024> isFirstReadPerBuffer;
    size_t readBufferIndex = 0;
    etl::vector<etl::vector<U8, 128>, 1024> mockedReadBuffers;
  };

}

#endif
