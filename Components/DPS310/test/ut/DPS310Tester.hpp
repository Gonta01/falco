// ======================================================================
// \title  DPS310Tester.hpp
// \author aos
// \brief  hpp file for DPS310 component test harness implementation class
// ======================================================================

#ifndef Components_DPS310Tester_HPP
#define Components_DPS310Tester_HPP

#include "Components/DPS310/DPS310GTestBase.hpp"
#include "Components/DPS310/DPS310.hpp"
#include "etl/vector.h"

namespace Components
{

  class DPS310Tester : public DPS310GTestBase
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

    //! Construct object DPS310Tester
    DPS310Tester();

    //! Destroy object DPS310Tester
    ~DPS310Tester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! To do
    void testDPS310Temperature();
    void testDPS310ColdTemperature();
    void testDPS310Pressure();
    void testDPS310FailureNotInited();
    void testDPS310FailureWrite();
    void testDPS310FailureRead();

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

    void setMockReadBuffers(etl::span<U8> value);

    void setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses);
    void setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses);

    

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    DPS310 component;

    Fw::Buffer mockedWriteBuffer;
    size_t writeBufferStatusesIndex = 0;
    etl::vector<Drv::I2C::Status, 32> mockedWriteBufferStatuses;


    size_t readBufferStatusesIndex = 0;
    size_t readBufferIndex = 0;
    etl::vector<U8, 32> mockReadBuffers;
    etl::vector<Drv::I2C::Status, 32> mockedReadBufferStatuses;
    

  };

}

#endif
