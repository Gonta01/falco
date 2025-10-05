// ======================================================================
// \title  STS21Tester.cpp
// \author aos
// \brief  cpp file for STS21 component test harness implementation class
// ======================================================================

#include "STS21Tester.hpp"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  STS21Tester ::
      STS21Tester() : STS21GTestBase("STS21Tester", STS21Tester::MAX_HISTORY_SIZE),
                      component("STS21")
  {
    this->initComponents();
    this->connectPorts();
  }

  STS21Tester ::
      ~STS21Tester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void STS21Tester ::
      testSTS21Temperature()
  {
    this->setMockWriteStatus(Drv::I2C::Status::SUCCESS);
    this->component.configure();
    constexpr size_t sizeOfDataTemperature = 3;
    etl::array<U8, sizeOfDataTemperature> dataTemperature = {0x6D, 0x0B, 0xFF};

    Fw::Buffer dataTemperatureBuffer(dataTemperature.data(), dataTemperature.size());
    this->setMockReadBuffer(dataTemperatureBuffer);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    F32 expectedTemperature = 28.0;
    F32 absError = 0.1;
    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }

  void STS21Tester ::
      testSTS21ColdTemperature()
  {
    this->setMockWriteStatus(Drv::I2C::Status::SUCCESS);
    this->component.configure();
    constexpr size_t sizeOfDataTemperature = 3;
    etl::array<U8, sizeOfDataTemperature> dataTemperature = {0x35, 0xC8, 0x3B};

    Fw::Buffer dataTemperatureBuffer(dataTemperature.data(), dataTemperature.size());
    this->setMockReadBuffer(dataTemperatureBuffer);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    F32 expectedTemperature = -10.0;
    F32 absError = 0.1;
    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }

  void STS21Tester ::testSTS21FailureCommunication()
  {
    this->setMockWriteStatus(Drv::I2C::Status::SUCCESS);
    this->component.configure();
    constexpr size_t sizeOfDataTemperature = 3;
    etl::array<U8, sizeOfDataTemperature> dataTemperature = {0x6D, 0x0B, 0x00};

    Fw::Buffer dataTemperatureBuffer(dataTemperature.data(), dataTemperature.size());
    this->setMockReadBuffer(dataTemperatureBuffer);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_CommunicationError_SIZE(1);
  }

  void STS21Tester ::testSTS21FailureNotInited()
  {
    this->setMockWriteStatus(Drv::I2C::Status::WRITE_ERR);
    this->component.configure();
    constexpr size_t sizeOfDataTemperature = 3;
    etl::array<U8, sizeOfDataTemperature> dataTemperature = {0x6D, 0x0B, 0x00};

    Fw::Buffer dataTemperatureBuffer(dataTemperature.data(), dataTemperature.size());
    this->setMockReadBuffer(dataTemperatureBuffer);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    ASSERT_EVENTS_NotInited_SIZE(1);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::I2C::Status STS21Tester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    U8 *serBufferData = serBuffer.getData();
    U8 *mockedReadBufferData = mockedReadBuffer.getData();
    auto minBufferSize = etl::min(serBuffer.getSize(), mockedReadBuffer.getSize());

    for (U32 i = 0; i < minBufferSize; i++)
    {
      serBufferData[i] = mockedReadBufferData[i];
    }
    return mockedReadStatus;
  }

  Drv::I2C::Status STS21Tester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    return mockedWriteStatus;
  }

  void STS21Tester::setMockReadBuffer(Fw::Buffer &value)
  {
    mockedReadBuffer = value;
    mockedReadStatus = Drv::I2C::Status::SUCCESS;
  }

  void STS21Tester::setMockReadStatus(Drv::I2C::Status status)
  {
    mockedReadStatus = status;
  }

  void STS21Tester::setMockWriteStatus(Drv::I2C::Status status)
  {
    mockedWriteStatus = status;
  }

}
