// ======================================================================
// \title  NORDriverTester.cpp
// \author aos
// \brief  cpp file for NORDriver component test harness implementation class
// ======================================================================

#include "NORDriverTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  NORDriverTester ::
      NORDriverTester() : NORDriverGTestBase("NORDriverTester", NORDriverTester::MAX_HISTORY_SIZE),
                          component("NORDriver")
  {
    this->initComponents();
    this->connectPorts();
  }

  NORDriverTester ::
      ~NORDriverTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void NORDriverTester ::
      testConfigureNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0xEF, 0x70, 0x21}};

    this->setMockReadRegister(readRegisters);
    this->component.configure(Components::NORDriver::PowerDown::Disable);

    ASSERT_EVENTS_SIZE(0);
  }

  void NORDriverTester ::
      testReadNORDriver()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{}};

    dataPacketRead[0].assign(sizeOfDataPacketRead, 0xC9);
    this->setMockReadMemory(dataPacketRead);

    etl::array<U8, sizeOfDataPacketRead> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());
    constexpr U32 randomMemoryAddress = 0x00F40B0;
    auto status = this->invoke_to_read(0, randomMemoryAddress, actualDataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::NoError);

    U8 *data = actualDataBuff.getData();
    for (size_t i = 0; i < actualDataBuff.getSize(); i++)
    {
      ASSERT_EQ(data[i], dataPacketRead[0][i]);
    }
  }
  void NORDriverTester ::
      testWriteNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x02},
        etl::vector<U8, sizeReadRegisters>{0x00}};

    this->setMockReadRegister(readRegisters);

    constexpr size_t sizeOfDataPacketWrite = 32;
    etl::array<U8, sizeOfDataPacketWrite> writeData = {0x43, 0x04, 0x07};
    Fw::Buffer dataBuff(writeData.data(), writeData.size());

    constexpr U32 startAdress = 0x00000000;
    auto status = this->invoke_to_write(0, startAdress, dataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::NoError);

    Fw::Buffer capturedData = this->getCapturedWriteBuffer();
    U8 *actualData = capturedData.getData();
    for (size_t i = 0; i < capturedData.getSize(); i++)
    {
      ASSERT_EQ(actualData[i], writeData[i]);
    }
  }
  void NORDriverTester ::
      testEraseNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x02},
        etl::vector<U8, sizeReadRegisters>{0x00}};
    this->setMockReadRegister(readRegisters);
    constexpr U32 startAddress = 0x00000000;
    constexpr U32 amountOfBytesToErase = 8192;
    auto status = this->invoke_to_erase(0, startAddress, amountOfBytesToErase);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::NoError);
  }
  void NORDriverTester ::
      testFailureWrongMemoryIdNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x0F, 0x00, 0x01}};

    this->setMockReadRegister(readRegisters);
    this->component.configure(Components::NORDriver::PowerDown::Disable);

    ASSERT_EVENTS_WrongMemoryId_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }
  void NORDriverTester ::
      testFailureInputBufferEmptyNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x02},
        etl::vector<U8, sizeReadRegisters>{0x00}};

    this->setMockReadRegister(readRegisters);

    Fw::Buffer dataBuff;

    constexpr U32 startAddress = 0x00000000;
    auto status = this->invoke_to_write(0, startAddress, dataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::InputBufferEmpty);

    ASSERT_EVENTS_InputBufferEmpty_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }
  void NORDriverTester ::
      testFailureAccessOutOfRangeNORDriver()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{}};

    dataPacketRead[0].assign(sizeOfDataPacketRead, 0xC9);
    this->setMockReadMemory(dataPacketRead);

    etl::array<U8, sizeOfDataPacketRead> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());

    constexpr U32 addressOutOfRange = 0xFFFFFFFF;
    auto status = this->invoke_to_read(0, addressOutOfRange, actualDataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::AccessOutOfRange);

    ASSERT_EVENTS_AccessOutOfRange_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }
  void NORDriverTester ::
      testFailureUnalignedAccessNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x00},
        etl::vector<U8, sizeReadRegisters>{0x02},
        etl::vector<U8, sizeReadRegisters>{0x00}};
    this->setMockReadRegister(readRegisters);
    constexpr U32 startAddress = 0x00000000;
    constexpr U32 wrongAmountOfBytesToErase = 4100;
    auto status = this->invoke_to_erase(0, startAddress, wrongAmountOfBytesToErase);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::UnalignedAccess);

    ASSERT_EVENTS_UnalignedAccess_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }

  void NORDriverTester ::
      testFailureIOInterfaceErrorNORDriver()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{}};

    dataPacketRead[0].assign(sizeOfDataPacketRead, 0xC9);
    this->setMockReadMemory(dataPacketRead);
    etl::vector<Drv::QSPI::QSPIError, 1> readMemoryStatuses = {Drv::QSPI::QSPIError::IOError};
    this->setMockReadMemoryStatuses(readMemoryStatuses);

    etl::array<U8, sizeOfDataPacketRead> readData;
    Fw::Buffer actualDataBuff(readData.data(), readData.size());
    constexpr U32 randomMemoryAddress = 0x00F40B0;
    auto status = this->invoke_to_read(0, randomMemoryAddress, actualDataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::IOInterfaceError);

    ASSERT_EVENTS_IOInterfaceError_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }
  void NORDriverTester ::
      testFailureFlashMemoryBusyNORDriver()
  {
    constexpr size_t sizeReadRegisters = 32;
    etl::vector<etl::vector<U8, sizeReadRegisters>, sizeReadRegisters> readRegisters = {
        etl::vector<U8, sizeReadRegisters>{0x01},
        etl::vector<U8, sizeReadRegisters>{0x01},
        etl::vector<U8, sizeReadRegisters>{0x01}};

    this->setMockReadRegister(readRegisters);

    constexpr U32 numOfOperations = 100;
    etl::vector<Drv::QSPI::QSPIError, numOfOperations> readRegistersStatuses;
    readRegistersStatuses.assign(numOfOperations, Drv::QSPI::QSPIError::NoError);
    this->setMockReadRegisterStatuses(readRegistersStatuses);

    constexpr size_t sizeOfDataPacketWrite = 32;
    etl::array<U8, sizeOfDataPacketWrite> writeData = {0x43, 0x04, 0x07};
    Fw::Buffer dataBuff(writeData.data(), writeData.size());

    constexpr U32 startAddress = 0x00000000;
    auto status = this->invoke_to_write(0, startAddress, dataBuff);
    ASSERT_EQ(status, Drv::NOR::ErrorCode::FlashMemoryBusy);

    ASSERT_EVENTS_FlashMemoryBusy_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
  }

  void NORDriverTester ::
      testGetMemoryAndSectorSizeNORDriver()
  {
    U32 memorySize = this->invoke_to_getMemorySize(0);
    U32 sectorSize = this->invoke_to_getSectorSize(0);

    constexpr U32 expectedMemorySize = 0x08000000;
    constexpr U32 expectedSectorSize = 4096;
    ASSERT_EQ(memorySize, expectedMemorySize);
    ASSERT_EQ(sectorSize, expectedSectorSize);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::QSPI::QSPIError NORDriverTester ::
      from_commandWrite_handler(
          NATIVE_INT_TYPE portNum,
          Drv::QSPI::QPSICommandConfig &config,
          U32 address)
  {
    return Drv::QSPI::QSPIError::NoError;
  }

  Drv::QSPI::QSPIError NORDriverTester ::
      from_memoryRead_handler(
          NATIVE_INT_TYPE portNum,
          Drv::QSPI::QSPIMemoryConfig &config,
          U32 address,
          Fw::Buffer &buff)
  {
    U8 *bufferData = buff.getData();
    const auto &mockDataVec = mockedReadMemoryBuffers[readMemoryBufferIndex++];
    auto minBufferSize = etl::min(buff.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      buff.getData()[i] = mockDataVec[i];
    }

    return mockedReadMemoryBufferStatuses[readMemoryBufferStatusesIndex++];
  }

  Drv::QSPI::QSPIError NORDriverTester ::
      from_memoryWrite_handler(
          NATIVE_INT_TYPE portNum,
          Drv::QSPI::QSPIMemoryConfig &config,
          U32 address,
          Fw::Buffer &buff)
  {
    const U8 *src = buff.getData();
    U8 *dest = this->capturedWriteBuffer.getData();
    const U32 size = buff.getSize();
    for (U32 i = 0; i < size; i++)
    {
      dest[i] = src[i];
    }
    this->capturedWriteBuffer.setSize(size);
    return Drv::QSPI::QSPIError::NoError;
  }

  Drv::QSPI::QSPIError NORDriverTester ::
      from_registerRead_handler(
          NATIVE_INT_TYPE portNum,
          Drv::QSPI::QSPIRegisterConfig &config,
          Fw::Buffer &buff)
  {
    U8 *bufferData = buff.getData();
    const auto &mockDataVec = mockedReadRegisterBuffers[readRegisterBufferIndex++];
    auto minBufferSize = etl::min(buff.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      buff.getData()[i] = mockDataVec[i];
    }

    return mockedReadRegisterBufferStatuses[readRegisterBufferStatusesIndex++];
  }

  Drv::QSPI::QSPIError NORDriverTester ::
      from_registerWrite_handler(
          NATIVE_INT_TYPE portNum,
          Drv::QSPI::QSPIRegisterConfig &config,
          Fw::Buffer &buff)
  {
    return Drv::QSPI::QSPIError::NoError;
  }

  void NORDriverTester ::
      setMockReadRegister(etl::vector<etl::vector<U8, 32>, 32> value)
  {
    readRegisterBufferIndex = 0;
    readRegisterBufferStatusesIndex = 0;
    mockedReadRegisterBuffers.clear();
    mockedReadRegisterBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadRegisterBuffers.push_back(value[i]);
      mockedReadRegisterBufferStatuses.push_back(Drv::QSPI::QSPIError::NoError);
    }
  }
  void NORDriverTester ::
      setMockReadMemory(etl::vector<etl::vector<U8, 32>, 32> value)
  {
    readMemoryBufferIndex = 0;
    readMemoryBufferStatusesIndex = 0;
    mockedReadMemoryBuffers.clear();
    mockedReadMemoryBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadMemoryBuffers.push_back(value[i]);
      mockedReadMemoryBufferStatuses.push_back(Drv::QSPI::QSPIError::NoError);
    }
  }

  void NORDriverTester ::
      setMockReadRegisterStatuses(etl::span<Drv::QSPI::QSPIError> statuses)
  {
    readRegisterBufferIndex = 0;
    mockedReadRegisterBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadRegisterBufferStatuses.push_back(statuses[i]);
    }
  }
  void NORDriverTester ::
      setMockReadMemoryStatuses(etl::span<Drv::QSPI::QSPIError> statuses)
  {
    readMemoryBufferStatusesIndex = 0;
    mockedReadMemoryBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadMemoryBufferStatuses.push_back(statuses[i]);
    }
  }

  const Fw::Buffer NORDriverTester::getCapturedWriteBuffer()
  {
    return this->capturedWriteBuffer;
  }
}
