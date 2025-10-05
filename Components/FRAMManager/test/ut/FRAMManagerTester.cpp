// ======================================================================
// \title  FRAMManagerTester.cpp
// \author aos
// \brief  cpp file for FRAMManager component test harness implementation class
// ======================================================================

#include "FRAMManagerTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  FRAMManagerTester ::
      FRAMManagerTester() : FRAMManagerGTestBase("FRAMManagerTester", FRAMManagerTester::MAX_HISTORY_SIZE),
                            component("FRAMManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  FRAMManagerTester ::
      ~FRAMManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void FRAMManagerTester ::
      testRequestReadFRAMManager()
  {
    constexpr size_t sizeReadBuffer = 8192;
    constexpr size_t chunkSize = 256;
    constexpr size_t maxNumOfOperations = 32;
    constexpr size_t numChunks = sizeReadBuffer / chunkSize;

    etl::vector<etl::vector<U8, sizeReadBuffer>, maxNumOfOperations> readBuffers;

    for (size_t i = 0; i < numChunks; ++i)
    {
      etl::vector<U8, sizeReadBuffer> chunk;
      chunk.assign(chunkSize, 0xFA);
      readBuffers.push_back(chunk);
    }

    this->setMockRead(readBuffers);

    etl::vector<Drv::FRAM::InStatus, numChunks> readStatuses;
    readStatuses.assign(numChunks, Drv::FRAM::InStatus::SUCCESS);
    this->setMockReadStatuses(readStatuses);

    etl::array<U8, sizeReadBuffer> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());

    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestRead(0, startAddress, actualDataBuff);
    this->component.doDispatch();
    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::READ_SUCCESS);

    U8 *data = actualDataBuff.getData();
    for (size_t i = 0; i < actualDataBuff.getSize(); i++)
    {
      ASSERT_EQ(data[i], 0xFA);
    }
  }

  void FRAMManagerTester ::
      testRequestWriteFRAMManager()
  {
    constexpr size_t sizeWriteBuffer = 8192;
    constexpr size_t chunkSize = 256;
    constexpr size_t maxNumOfOperations = 32;
    constexpr size_t numChunks = sizeWriteBuffer / chunkSize;

    etl::vector<etl::vector<U8, sizeWriteBuffer>, maxNumOfOperations> writeBuffers;

    for (size_t i = 0; i < numChunks; ++i)
    {
      etl::vector<U8, sizeWriteBuffer> chunk;
      chunk.assign(chunkSize, 0xFA);
      writeBuffers.push_back(chunk);
    }

    Fw::Buffer actualDataBuff(writeBuffers[0].data(), writeBuffers[0].size());

    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestWrite(0, startAddress, actualDataBuff);
    this->component.doDispatch();

    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::WRITE_SUCCESS);

    etl::vector<U8, sizeWriteBuffer> writtenData = this->getCapturedWriteBuffer(0);
    U8 *data = actualDataBuff.getData();
    for (size_t i = 0; i < actualDataBuff.getSize(); i++)
    {
      ASSERT_EQ(data[i], writtenData[i]);
    }
  }

  void FRAMManagerTester ::
      testRequestWakeupFRAMManager()
  {
    this->setMockWakeupStatus(Drv::FRAM::InStatus::SUCCESS);
    this->invoke_to_requestWakeup(0);
    this->component.doDispatch();

    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::WAKEUP_SUCCESS);
  }

  void FRAMManagerTester ::
      testRequestSleepFRAMManager()
  {
    this->setMockSleepStatus(Drv::FRAM::InStatus::SUCCESS);
    this->invoke_to_requestSleep(0);
    this->component.doDispatch();

    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::SLEEP_SUCCESS);
  }

  void FRAMManagerTester ::
      testFailureRequestReadFRAMManager()
  {
    constexpr size_t sizeReadBuffer = 8192;
    constexpr size_t maxNumOfOperations = 32;
    constexpr size_t chunkSize = 256;
    constexpr size_t numChunks = sizeReadBuffer / chunkSize;

    etl::vector<etl::vector<U8, sizeReadBuffer>, 32> readBuffers;

    for (size_t i = 0; i < numChunks; ++i)
    {
      etl::vector<U8, sizeReadBuffer> chunk;
      chunk.assign(chunkSize, 0xFA);
      readBuffers.push_back(chunk);
    }

    this->setMockRead(readBuffers);

    etl::vector<Drv::FRAM::InStatus, maxNumOfOperations>
        readBufferStatuses;
    readBufferStatuses.assign(maxNumOfOperations, Drv::FRAM::InStatus::FAILED);
    this->setMockReadStatuses(readBufferStatuses);

    etl::array<U8, sizeReadBuffer> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());

    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestRead(0, startAddress, actualDataBuff);
    this->component.doDispatch();

    ASSERT_EVENTS_ReadFailed_SIZE(1);
    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::READ_FAILED);
  }

  void FRAMManagerTester ::
      testFailureRequestWriteFRAMManager()
  {
    constexpr size_t sizeWriteBuffer = 8192;
    constexpr size_t chunkSize = 256;
    constexpr size_t maxNumOfOperations = 32;
    constexpr size_t numChunks = sizeWriteBuffer / chunkSize;

    etl::vector<etl::vector<U8, sizeWriteBuffer>, maxNumOfOperations> writeBuffers;

    for (size_t i = 0; i < numChunks; ++i)
    {
      etl::vector<U8, sizeWriteBuffer> chunk;
      chunk.assign(chunkSize, 0xFA);
      writeBuffers.push_back(chunk);
    }

    etl::vector<Drv::FRAM::InStatus, maxNumOfOperations> writeBufferStatuses;
    writeBufferStatuses.assign(maxNumOfOperations, Drv::FRAM::InStatus::FAILED);
    this->setMockWriteStatuses(writeBufferStatuses);

    Fw::Buffer actualDataBuff(writeBuffers.at(0).data(), writeBuffers.at(0).size());

    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestWrite(0, startAddress, actualDataBuff);
    this->component.doDispatch();

    ASSERT_EVENTS_WriteFailed_SIZE(1);
    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::WRITE_FAILED);
  }

  void FRAMManagerTester ::
      testFailureRequestWakeupFRAMManager()
  {
    this->setMockWakeupStatus(Drv::FRAM::InStatus::FAILED);
    this->invoke_to_requestWakeup(0);
    this->component.doDispatch();

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_WakeupFailed_SIZE(1);
    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::WAKEUP_FAILED);
  }

  void FRAMManagerTester ::
      testFailureRequestSleepFRAMManager()
  {
    this->setMockSleepStatus(Drv::FRAM::InStatus::FAILED);
    this->invoke_to_requestSleep(0);
    this->component.doDispatch();

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_SleepFailed_SIZE(1);
    ASSERT_EQ(recievedExtStatus, Drv::FRAM::ExtStatus::SLEEP_FAILED);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::FRAM::InStatus FRAMManagerTester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &buffer)
  {
    const auto &mockDataVec = mockedReadBuffers[readBufferIndex++];
    auto minBufferSize = etl::min(buffer.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      buffer.getData()[i] = mockDataVec[i];
    }

    return mockedReadBufferStatuses[readBufferStatusesIndex++];
  }

  void FRAMManagerTester ::
      from_reinit_handler(NATIVE_INT_TYPE portNum)
  {
    // Not used
  }

  Drv::FRAM::InStatus FRAMManagerTester ::
      from_sleep_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedSleepStatus;
  }

  void FRAMManagerTester ::
      from_status_handler(
          NATIVE_INT_TYPE portNum,
          const Drv::FRAM::ExtStatus &status)
  {
    recievedExtStatus = status;
  }

  Drv::FRAM::InStatus FRAMManagerTester ::
      from_wakeup_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedWakeupStatus;
  }

  Drv::FRAM::InStatus FRAMManagerTester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &buffer)
  {
    const U8 *src = buffer.getData();
    const U32 size = buffer.getSize();

    etl::vector<U8, 8192> copied;
    for (U32 i = 0; i < size && i < copied.max_size(); ++i)
    {
      copied.push_back(src[i]);
    }

    this->capturedWriteBuffers.push_back(copied);

    return mockedWriteBufferStatuses[writeBufferIndex++];
  }

  void FRAMManagerTester ::
      setMockWakeupStatus(Drv::FRAM::InStatus status)
  {
    mockedWakeupStatus = status;
  }
  void FRAMManagerTester ::
      setMockSleepStatus(Drv::FRAM::InStatus status)
  {
    mockedSleepStatus = status;
  }

  void FRAMManagerTester ::
      setMockRead(etl::vector<etl::vector<U8, 8192>, 32> value)
  {
    readBufferIndex = 0;
    readBufferStatusesIndex = 0;
    mockedReadBuffers.clear();
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadBuffers.push_back(value[i]);
      mockedReadBufferStatuses.push_back(Drv::FRAM::InStatus::SUCCESS);
    }
  }
  void FRAMManagerTester ::
      setMockReadStatuses(etl::span<Drv::FRAM::InStatus> statuses)
  {
    readBufferIndex = 0;
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadBufferStatuses.push_back(statuses[i]);
    }
  }

  void FRAMManagerTester ::
      setMockWriteStatuses(etl::span<Drv::FRAM::InStatus> statuses)
  {
    writeBufferIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }
  const etl::vector<U8, 8192> &FRAMManagerTester ::
      getCapturedWriteBuffer(size_t index)
  {
    return this->capturedWriteBuffers.at(index);
  }

}
