// ======================================================================
// \title  DPS310Tester.cpp
// \author aos
// \brief  cpp file for DPS310 component test harness implementation class
// ======================================================================

#include "DPS310Tester.hpp"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  DPS310Tester ::
      DPS310Tester() : DPS310GTestBase("DPS310Tester", DPS310Tester::MAX_HISTORY_SIZE),
                       component("DPS310")
  {
    this->initComponents();
    this->connectPorts();
  }

  DPS310Tester ::
      ~DPS310Tester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void DPS310Tester ::
      testDPS310Temperature()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    constexpr size_t sizeOfDataTemperature = 3;
    etl::vector<U8, sizeOfDataTemperature> dataTemperature = {0xB0, 0x22, 0x02};
    this->setMockReadBuffers(dataTemperature);
    etl::vector<Drv::I2C::Status, sizeOfDataTemperature> dataWriteTempStatuses;
    for (size_t i = 0; i < sizeOfDataTemperature; i++)
    {
      dataWriteTempStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteTempStatuses);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    constexpr F32 expectedTemperature = 31;
    constexpr F32 absError = 0.1;
    ASSERT_EVENTS_MeasurementTimeout_SIZE(0);
    ASSERT_EVENTS_NotInited_SIZE(0);
    ASSERT_EVENTS_SIZE(0);

    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }

  void DPS310Tester ::
      testDPS310ColdTemperature()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    constexpr size_t sizeOfDataTemperature = 3;
    etl::vector<U8, sizeOfDataTemperature> dataTemperature = {0xF0, 0x46, 0x03};
    this->setMockReadBuffers(dataTemperature);
    etl::vector<Drv::I2C::Status, sizeOfDataTemperature> dataWriteTempStatuses;
    for (size_t i = 0; i < sizeOfDataTemperature; i++)
    {
      dataWriteTempStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteTempStatuses);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    constexpr F32 expectedTemperature = -11.25;
    constexpr F32 absError = 0.1;
    ASSERT_EVENTS_MeasurementTimeout_SIZE(0);
    ASSERT_EVENTS_NotInited_SIZE(0);
    ASSERT_EVENTS_SIZE(0);

    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }

  void DPS310Tester ::
      testDPS310Pressure()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    constexpr size_t sizeOfDataPressure = 6;
    etl::vector<U8, sizeOfDataPressure> dataPressure = {0xB0, 0x22, 0x02, 0x7D, 0x85, 0xFE};
    this->setMockReadBuffers(dataPressure);
    etl::vector<Drv::I2C::Status, sizeOfDataPressure> dataWritePressStatuses;
    for (size_t i = 0; i < sizeOfDataPressure; i++)
    {
      dataWritePressStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWritePressStatuses);
    F32 actualPressure = 0;
    this->invoke_to_getPressure(0, actualPressure);

    constexpr F32 expectedPressure = 979.5;
    constexpr F32 absError = 0.1;
    ASSERT_EVENTS_MeasurementTimeout_SIZE(0);
    ASSERT_EVENTS_NotInited_SIZE(0);
    ASSERT_EVENTS_SIZE(0);

    ASSERT_NEAR(actualPressure / 100, expectedPressure, absError);
  }

  void DPS310Tester ::
      testDPS310FailureNotInited()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    etl::vector<Drv::I2C::Status, sizeOfDataConfig> dataReadConfStatuses;
    for (size_t i = 0; i < sizeOfDataConfig; i++)
    {
      dataReadConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    dataWriteConfStatuses[0] = Drv::I2C::Status::READ_ERR;
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    ASSERT_EVENTS_NotInited_SIZE(1);
    ASSERT_EVENTS_MeasurementTimeout_SIZE(1);
    F32 temperature = 0.0;
    F32 pressure = 0.0;
    auto recievedStatusTemp = this->invoke_to_getTemperature(0, temperature);
    auto recievedStatusPress = this->invoke_to_getPressure(0, pressure);
    ASSERT_EQ(Drv::Sensor::Status::FAILED, recievedStatusTemp);
    ASSERT_EQ(Drv::Sensor::Status::FAILED, recievedStatusPress);
  }

  void DPS310Tester ::
      testDPS310FailureWrite()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    dataWriteConfStatuses[26] = Drv::I2C::Status::WRITE_ERR;
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    ASSERT_EVENTS_FailedWriteToRegsiter_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }
  void DPS310Tester ::
      testDPS310FailureRead()
  {
    constexpr size_t sizeOfDataConfig = 23;
    etl::vector<U8, sizeOfDataConfig> dataConfig = {0x40, 0xC0, 0x0D, 0xCE, 0xD8, 0x13, 0xBA, 0xDF, 0x37, 0x0D,
                                                    0xF1, 0xD5, 0x04, 0x99, 0xD8, 0x52, 0x00, 0x20, 0xFA, 0x51, 0x10, 0xBD, 0xF7};
    this->setMockReadBuffers(dataConfig);
    constexpr size_t numOfWriteInConfig = 28;
    etl::vector<Drv::I2C::Status, numOfWriteInConfig> dataWriteConfStatuses;
    for (size_t i = 0; i < numOfWriteInConfig; i++)
    {
      dataWriteConfStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteConfStatuses);
    this->component.configure();

    constexpr size_t sizeOfDataTemperature = 3;
    etl::vector<U8, sizeOfDataTemperature> dataTemperature = {0xF0, 0x46, 0x03};
    this->setMockReadBuffers(dataTemperature);
    etl::vector<Drv::I2C::Status, sizeOfDataTemperature> dataReadTempStatuses;
    for (size_t i = 0; i < sizeOfDataTemperature; i++)
    {
      dataReadTempStatuses.push_back(Drv::I2C::Status::READ_ERR);
    }
    this->setMockReadBufferStatuses(dataReadTempStatuses);
    etl::vector<Drv::I2C::Status, sizeOfDataTemperature> dataWriteTempStatuses;
    for (size_t i = 0; i < sizeOfDataTemperature; i++)
    {
      dataWriteTempStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataWriteTempStatuses);
    F32 actualTemperature = 0;
    this->invoke_to_getTemperature(0, actualTemperature);

    ASSERT_EVENTS_FailedReadFromRegsiter_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::I2C::Status DPS310Tester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    U8 *serBufferData = serBuffer.getData();
    Fw::Buffer mockBuffer(&mockReadBuffers[readBufferIndex++], 1);
    U8 *mockData = mockBuffer.getData();
    auto minBufferSize = etl::min(serBuffer.getSize(), mockBuffer.getSize());

    for (U32 i = 0; i < minBufferSize; i++)
    {
      serBufferData[i] = mockData[i];
    }

    return mockedReadBufferStatuses[readBufferStatusesIndex++];
  }

  Drv::I2C::Status DPS310Tester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    return mockedWriteBufferStatuses[writeBufferStatusesIndex++];
  }

  void DPS310Tester::setMockReadBuffers(etl::span<U8> value)
  {
    readBufferIndex = 0;
    readBufferStatusesIndex = 0;
    mockReadBuffers.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockReadBuffers.push_back(value[i]);
      mockedReadBufferStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
  }

  void DPS310Tester::setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {
    readBufferStatusesIndex = 0;
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadBufferStatuses.push_back(statuses[i]);
    }
  }
  void DPS310Tester::setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {
    writeBufferStatusesIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }
}
