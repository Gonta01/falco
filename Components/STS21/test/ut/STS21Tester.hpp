// ======================================================================
// \title  STS21Tester.hpp
// \author aos
// \brief  hpp file for STS21 component test harness implementation class
// ======================================================================

#ifndef Components_STS21Tester_HPP
#define Components_STS21Tester_HPP

#include "Components/STS21/STS21GTestBase.hpp"
#include "Components/STS21/STS21.hpp"

namespace Components
{

  class STS21Tester : public STS21GTestBase
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

    //! Construct object STS21Tester
    STS21Tester();

    //! Destroy object STS21Tester
    ~STS21Tester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testSTS21Temperature();
    void testSTS21ColdTemperature();
    void testSTS21FailureCommunication();
    void testSTS21FailureNotInited();

    void setMockReadBuffer(Fw::Buffer &value);
    void setMockReadStatus(Drv::I2C::Status status);
    void setMockWriteStatus(Drv::I2C::Status status);

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

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

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    STS21 component;
    Fw::Buffer mockedReadBuffer;
    Drv::I2C::Status mockedReadStatus;

    Drv::I2C::Status mockedWriteStatus;
  };

}

#endif
