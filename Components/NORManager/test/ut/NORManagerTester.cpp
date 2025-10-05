// ======================================================================
// \title  NORManagerTester.cpp
// \author aos
// \brief  cpp file for NORManager component test harness implementation class
// ======================================================================

#include "NORManagerTester.hpp"
#include "etl/vector.h"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  NORManagerTester ::
      NORManagerTester() : NORManagerGTestBase("NORManagerTester", NORManagerTester::MAX_HISTORY_SIZE),
                           component("NORManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  NORManagerTester ::
      ~NORManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void NORManagerTester ::
      testRequestEraseNORManager()
  {
    this->setMockRequestEraseStatus(Drv::NOR::ErrorCode::NoError);
    constexpr U32 startAddress = 0x00000000;
    constexpr U32 amountOfBytesToErase = 4096;
    this->invoke_to_requestErase(0, startAddress, amountOfBytesToErase);
    this->component.doDispatch();

    ASSERT_EQ(receivedEraseStatus, Drv::NORManager::Status::SUCCESS);
  }
  void NORManagerTester ::
      testRequestReadNORManager()
  {
    constexpr U32 maxSizeBytes = 8192;
    etl::vector<U8, maxSizeBytes> mockData;
    mockData.assign(maxSizeBytes, 0x5C);
    this->setMockReadBuffer(mockData);

    etl::vector<U8, maxSizeBytes> actualData;
    Fw::Buffer readBuff(actualData.data(), actualData.size());
    this->setMockRequestReadStatus(Drv::NOR::ErrorCode::NoError);
    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestRead(0, startAddress, readBuff);
    this->component.doDispatch();

    ASSERT_EQ(receivedReadStatus, Drv::NORManager::Status::SUCCESS);

    U8 *data = readBuff.getData();
    for (size_t i = 0; i < maxSizeBytes; i++)
    {
      ASSERT_EQ(data[i], mockData[i]);
    }
  }
  void NORManagerTester ::
      testRequestWriteNORManager()
  {
    constexpr U32 maxSizeBytes = 8192;
    etl::vector<U8, maxSizeBytes> writeData;
    writeData.assign(maxSizeBytes, 0x5C);
    Fw::Buffer writeBuff(writeData.data(), writeData.size());
    this->setMockRequestWriteStatus(Drv::NOR::ErrorCode::NoError);
    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestWrite(0, startAddress, writeBuff);
    this->component.doDispatch();

    ASSERT_EQ(receivedWriteStatus, Drv::NORManager::Status::SUCCESS);

    etl::vector<U8, maxSizeBytes> capturedWriteBuffer = this->getCaptureWriteBuffer();

    for (size_t i = 0; i < maxSizeBytes; i++)
    {
      ASSERT_EQ(writeData[i], capturedWriteBuffer[i]);
    }
  }
  void NORManagerTester ::
      testRequestEraseNORManagerFailed()
  {
    this->setMockRequestEraseStatus(Drv::NOR::ErrorCode::UnalignedAccess);
    constexpr U32 startAddress = 0x00000000;
    constexpr U32 invalidAmountOfBytesToErase = 4100;
    this->invoke_to_requestErase(0, startAddress, invalidAmountOfBytesToErase);
    this->component.doDispatch();

    ASSERT_EQ(receivedEraseStatus, Drv::NORManager::Status::FAILED);
  }
  void NORManagerTester ::
      testRequestReadNORManagerFailed()
  {
    constexpr U32 maxSizeBytes = 8192;
    etl::vector<U8, maxSizeBytes> readData;
    readData.assign(maxSizeBytes, 0x5C);
    Fw::Buffer readBuff(readData.data(), readData.size());
    this->setMockRequestReadStatus(Drv::NOR::ErrorCode::IOInterfaceError);
    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestRead(0, startAddress, readBuff);
    this->component.doDispatch();

    ASSERT_EQ(receivedReadStatus, Drv::NORManager::Status::FAILED);
  }
  void NORManagerTester ::
      testRequestWriteNORManagerFailed()
  {
    constexpr U32 maxSizeBytes = 8192;
    etl::vector<U8, maxSizeBytes> writeData;
    writeData.assign(maxSizeBytes, 0x5C);
    Fw::Buffer writeBuff(writeData.data(), writeData.size());
    this->setMockRequestWriteStatus(Drv::NOR::ErrorCode::FlashMemoryBusy);
    constexpr U32 startAddress = 0x00000000;
    this->invoke_to_requestWrite(0, startAddress, writeBuff);
    this->component.doDispatch();

    ASSERT_EQ(receivedWriteStatus, Drv::NORManager::Status::FAILED);
  }
  void NORManagerTester ::
      testGetMemoryAndSectorSizeNORManager()
  {
    U32 actualMemorySize = this->invoke_to_getOutMemorySize(0);
    U32 actualSectorSize = this->invoke_to_getOutSectorSize(0);

    constexpr U32 expecetedMemorySize = 0x08000000;
    constexpr U32 expecetedSectorSize = 4096;
    ASSERT_EQ(actualMemorySize, expecetedMemorySize);
    ASSERT_EQ(actualSectorSize, expecetedSectorSize);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::NOR::ErrorCode NORManagerTester ::
      from_erase_handler(
          NATIVE_INT_TYPE portNum,
          U32 startAddress,
          U32 length)
  {
    return mockedEraseRequestStatus;
  }

  void NORManagerTester ::
      from_eraseStatus_handler(
          NATIVE_INT_TYPE portNum,
          const Drv::NORManager::Status &status)
  {
    receivedEraseStatus = status;
  }

  U32 NORManagerTester ::
      from_getInMemorySize_handler(NATIVE_INT_TYPE portNum)
  {
    return memorySize;
  }

  U32 NORManagerTester ::
      from_getInSectorSize_handler(NATIVE_INT_TYPE portNum)
  {
    return sectorSize;
  }

  Drv::NOR::ErrorCode NORManagerTester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          U32 startAddress,
          Fw::Buffer &dataInBuff)
  {
    U8 *data = dataInBuff.getData();
    U8 *mockData = mockedReadBuffer.data();

    for (size_t i = 0; i < mockedReadBuffer.size(); i++)
    {
      data[i] = mockData[i];
    }

    return mockedReadRequestStatus;
  }

  void NORManagerTester ::
      from_readStatus_handler(
          NATIVE_INT_TYPE portNum,
          const Drv::NORManager::Status &status)
  {
    receivedReadStatus = status;
  }

  Drv::NOR::ErrorCode NORManagerTester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 startAddress,
          Fw::Buffer &dataOutBuff)
  {
    U8 *srcPtr = static_cast<U8 *>(dataOutBuff.getData());
    U32 size = dataOutBuff.getSize();

    size = std::min<U32>(size, capturedWriteBuffer.max_size());

    capturedWriteBuffer.clear();
    capturedWriteBuffer.insert(capturedWriteBuffer.end(), srcPtr, srcPtr + size);

    return mockedWriteRequestStatus;
  }

  void NORManagerTester ::
      from_writeStatus_handler(
          NATIVE_INT_TYPE portNum,
          const Drv::NORManager::Status &status)
  {
    receivedWriteStatus = status;
  }

  void NORManagerTester ::setMockRequestReadStatus(Drv::NOR::ErrorCode status)
  {
    mockedReadRequestStatus = status;
  }
  void NORManagerTester ::setMockRequestWriteStatus(Drv::NOR::ErrorCode status)
  {
    mockedWriteRequestStatus = status;
  }
  void NORManagerTester ::setMockRequestEraseStatus(Drv::NOR::ErrorCode status)
  {
    mockedEraseRequestStatus = status;
  }

  void NORManagerTester ::setMockReadBuffer(etl::vector<U8, 8192> value)
  {
    mockedReadBuffer.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadBuffer.push_back(value[i]);
    }
  }
  etl::vector<U8, 8192> &NORManagerTester::getCaptureWriteBuffer()
  {
    return this->capturedWriteBuffer;
  }
}
