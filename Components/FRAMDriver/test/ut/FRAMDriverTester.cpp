// ======================================================================
// \title  FRAMDriverTester.cpp
// \author aos
// \brief  cpp file for FRAMDriver component test harness implementation class
// ======================================================================

#include "FRAMDriverTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  FRAMDriverTester ::
      FRAMDriverTester() : FRAMDriverGTestBase("FRAMDriverTester", FRAMDriverTester::MAX_HISTORY_SIZE),
                           component("FRAMDriver")
  {
    this->initComponents();
    this->connectPorts();
  }

  FRAMDriverTester ::
      ~FRAMDriverTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void FRAMDriverTester ::
      testConfigureFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(1024, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    constexpr size_t sizeReadBuffer = 32;
    etl::vector<etl::vector<U8, sizeReadBuffer>, sizeReadBuffer> readBuffers = {
        etl::vector<U8, sizeReadBuffer>{0x04, 0x7F, 0x49, 0x03}};

    this->setMockRead(readBuffers);

    this->component.configure();
    ASSERT_EQ(this->component.isInited, true);
    ASSERT_EVENTS_SIZE(0);
  }

  void FRAMDriverTester ::
      testReadFRAMDriver()
  {

    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    constexpr size_t sizeReadBuffer = 32;
    etl::vector<etl::vector<U8, sizeReadBuffer>, sizeReadBuffer> readBuffers = {
        etl::vector<U8, sizeReadBuffer>{0x04, 0x7F, 0x49, 0x03, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x53, 0x0B, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x47, 0x03, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x49, 0x03, 0x04, 0x7F, 0x49, 0x03}};

    this->setMockRead(readBuffers);
    this->component.isInited = true;

    etl::array<U8, sizeReadBuffer> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());

    constexpr U32 randomAddress = 0x00005000;
    auto status = this->invoke_to_read(0, randomAddress, actualDataBuff);
    ASSERT_EQ(status, Drv::FRAM::InStatus::SUCCESS);

    U8 *data = actualDataBuff.getData();
    for (size_t i = 0; i < actualDataBuff.getSize(); i++)
    {
      ASSERT_EQ(data[i], readBuffers[0][i]);
    }
  }
  void FRAMDriverTester ::
      testWriteFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    this->component.isInited = true;

    etl::array<U8, 3> writeData = {0x43, 0x04, 0x07};
    Fw::Buffer actualDataBuff(writeData.data(), writeData.size());

    constexpr U32 randomAddress = 0x00000F00;
    auto status = this->invoke_to_write(0, randomAddress, actualDataBuff);
    ASSERT_EQ(status, Drv::FRAM::InStatus::SUCCESS);

    const auto &captured = this->getCapturedWriteBuffer(2);

    for (size_t i = 0; i < captured.size(); i++)
    {
      ASSERT_EQ(captured[i], writeData[i]);
    }
  }

  void FRAMDriverTester::
      testReInitFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    constexpr size_t sizeReadBuffer = 32;
    etl::vector<etl::vector<U8, sizeReadBuffer>, sizeReadBuffer> readBuffers = {
        etl::vector<U8, sizeReadBuffer>{0x04, 0x7F, 0x49, 0x03}};

    this->setMockRead(readBuffers);

    this->invoke_to_reinit(0);
    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(this->component.isInited, true);
  }

  void FRAMDriverTester::
      testFailureReInitFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    constexpr size_t sizeReadBuffer = 32;
    etl::vector<etl::vector<U8, sizeReadBuffer>, sizeReadBuffer> readBuffers = {
        etl::vector<U8, sizeReadBuffer>{0x04, 0x70, 0x49, 0x03}};

    this->setMockRead(readBuffers);

    this->invoke_to_reinit(0);
    ASSERT_EVENTS_Uninited_SIZE(1);
    ASSERT_EQ(this->component.isInited, false);
  }

  void FRAMDriverTester ::testFailureAddressOutOfRangeFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    constexpr size_t sizeReadBuffer = 32;
    etl::vector<etl::vector<U8, sizeReadBuffer>, sizeReadBuffer> readBuffers = {
        etl::vector<U8, sizeReadBuffer>{0x04, 0x7F, 0x49, 0x03, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x53, 0x0B, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x47, 0x03, 0x04, 0x7F, 0x49, 0x03,
                                        0x04, 0x7F, 0x49, 0x03, 0x04, 0x7F, 0x49, 0x03}};

    this->setMockRead(readBuffers);
    this->component.isInited = true;

    etl::array<U8, sizeReadBuffer> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());

    constexpr U32 randomAddress = 0x08005000;
    auto status = this->component.read(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::AddressOutOfRange);

    status = this->component.write(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::AddressOutOfRange);
  }
  void FRAMDriverTester ::testFailureInputBufferEmptyFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::NoError);
    this->setMockWriteStatuses(writeBufferStatuses);

    this->component.isInited = true;
    Fw::Buffer actualDataBuff;

    constexpr U32 randomAddress = 0x00000F00;
    auto status = this->component.write(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::InputBufferEmpty);

    status = this->component.read(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::InputBufferEmpty);
  }
  void FRAMDriverTester ::testFailureIOInterfaceErrorFRAMDriver()
  {
    constexpr size_t maxNumOfOperations = 1024;
    etl::vector<Drv::SPI::Status, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::SPI::Status::IOError);
    this->setMockWriteStatuses(writeBufferStatuses);

    this->component.isInited = true;

    etl::array<U8, 3> writeData = {0x43, 0x04, 0x07};
    Fw::Buffer actualDataBuff(writeData.data(), writeData.size());

    constexpr U32 randomAddress = 0x00000F00;
    auto status = this->component.write(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::IOInterfaceError);

    status = this->component.read(randomAddress, actualDataBuff);
    ASSERT_EQ(status.error(), Components::FRAMDriver::ErrorCode::IOInterfaceError);
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void FRAMDriverTester ::
      from_chipSelect_handler(
          NATIVE_INT_TYPE portNum,
          const Fw::Logic &state)
  {
    // Not used in tests
  }

  Drv::SPI::Status FRAMDriverTester ::
      from_driver_read_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {
    const auto &mockDataVec = mockedReadBuffers[readBufferIndex++];
    auto minBufferSize = etl::min(buffer.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      buffer.getData()[i] = mockDataVec[i];
    }

    return mockedReadBufferStatuses[readBufferStatusesIndex++];
  }

  void FRAMDriverTester ::
      from_driver_reinit_handler(NATIVE_INT_TYPE portNum)
  {
    // Not used in tests
  }

  Drv::SPI::Status FRAMDriverTester ::
      from_driver_write_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {

    const U8 *src = buffer.getData();
    const U32 size = buffer.getSize();

    etl::vector<U8, 32> copied;
    for (U32 i = 0; i < size && i < copied.max_size(); ++i)
    {
      copied.push_back(src[i]);
    }

    this->capturedWriteBuffers.push_back(copied);

    return mockedWriteBufferStatuses[writeBufferIndex++];
  }

  void FRAMDriverTester ::
      setMockRead(etl::vector<etl::vector<U8, 32>, 32> value)
  {
    readBufferIndex = 0;
    readBufferStatusesIndex = 0;
    mockedReadBuffers.clear();
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadBuffers.push_back(value[i]);
      mockedReadBufferStatuses.push_back(Drv::SPI::Status::NoError);
    }
  }
  void FRAMDriverTester ::
      setMockReadStatuses(etl::span<Drv::SPI::Status> statuses)
  {
    readBufferIndex = 0;
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadBufferStatuses.push_back(statuses[i]);
    }
  }

  void FRAMDriverTester ::
      setMockWriteStatuses(etl::span<Drv::SPI::Status> statuses)
  {
    writeBufferIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }
  const etl::vector<U8, 32> &FRAMDriverTester ::
      getCapturedWriteBuffer(size_t index)
  {
    return this->capturedWriteBuffers.at(index);
  }

}
