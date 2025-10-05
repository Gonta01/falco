// ======================================================================
// \title  BNO055Tester.cpp
// \author aos
// \brief  cpp file for BNO055 component test harness implementation class
// ======================================================================

#include "BNO055Tester.hpp"
#include "STest/Pick/Pick.hpp"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  BNO055Tester ::
      BNO055Tester() : BNO055GTestBase("BNO055Tester", BNO055Tester::MAX_HISTORY_SIZE),
                       component("BNO055")
  {
    this->initComponents();
    this->connectPorts();
  }

  BNO055Tester ::
      ~BNO055Tester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void BNO055Tester ::
      testReadBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);

    ASSERT_EQ(stResult, 0x0F);
  }

  void BNO055Tester ::
      testWriteBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x01}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.writeReg(0x07, chosenPage);

    ASSERT_EQ(writeStatus, Components::BNO055::ErrorCode::OPERATION_SUCCESS);
  }

  void BNO055Tester ::
      testGetAccelBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x06, 0x07, 0x00, 0x30, 0x00, 0xB4, 0x03}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Drv::IMU::AccelData actualAccelData;
    this->invoke_to_getAccelData(0, actualAccelData);

    Drv::IMU::AccelData expectedAccelData(0.07, 0.45, 9.5);
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualAccelData.getX(), expectedAccelData.getX(), absError);
    ASSERT_NEAR(actualAccelData.getY(), expectedAccelData.getY(), absError);
    ASSERT_NEAR(actualAccelData.getZ(), expectedAccelData.getZ(), absError);
  }

  void BNO055Tester ::
      testGetGyroBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x06, 0xFD, 0xFF, 0x00, 0x00, 0xFE, 0xFF}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Drv::IMU::GyroData actualGyroData;
    this->invoke_to_getGyroData(0, actualGyroData);

    Drv::IMU::GyroData expectedGyroData(-0.2, 0.0, -0.1);
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualGyroData.getX(), expectedGyroData.getX(), absError);
    ASSERT_NEAR(actualGyroData.getY(), expectedGyroData.getY(), absError);
    ASSERT_NEAR(actualGyroData.getZ(), expectedGyroData.getZ(), absError);
  }

  void BNO055Tester ::
      testGetMagnBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x06, 0x30, 0xFE, 0xE9, 0xFA, 0x2F, 0xFB}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Drv::IMU::MagnData actualMagnData;
    this->invoke_to_getMagnData(0, actualMagnData);

    Drv::IMU::MagnData expectedMagnData(-29, -81.5, -77);
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualMagnData.getX(), expectedMagnData.getX(), absError);
    ASSERT_NEAR(actualMagnData.getY(), expectedMagnData.getY(), absError);
    ASSERT_NEAR(actualMagnData.getZ(), expectedMagnData.getZ(), absError);
  }

  void BNO055Tester::
      testGetRawBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x12,
                                     0x06, 0x00, 0x33, 0x00, 0xBB, 0x03,
                                     0x2B, 0xFE, 0xE5, 0xFA, 0x3E, 0xFB,
                                     0x01, 0x00, 0x01, 0x00, 0xFE, 0xFF}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Drv::IMU::RawData actualRawData;
    this->invoke_to_getRawData(0, actualRawData);

    Drv::IMU::AccelData expectedAccelData(0.07, 0.45, 9.5);
    Drv::IMU::GyroData expectedGyroData(0.06, 0.0, -0.1);
    Drv::IMU::MagnData expectedMagnData(-29.3, -81.6, -76.1);
    Drv::IMU::RawData expectedRawData(expectedAccelData, expectedGyroData, expectedMagnData);
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualRawData.getaccelData().getX(), expectedRawData.getaccelData().getX(), absError);
    ASSERT_NEAR(actualRawData.getaccelData().getY(), expectedRawData.getaccelData().getY(), absError);
    ASSERT_NEAR(actualRawData.getaccelData().getZ(), expectedRawData.getaccelData().getZ(), absError);

    ASSERT_NEAR(actualRawData.getgyroData().getX(), expectedRawData.getgyroData().getX(), absError);
    ASSERT_NEAR(actualRawData.getgyroData().getY(), expectedRawData.getgyroData().getY(), absError);
    ASSERT_NEAR(actualRawData.getgyroData().getZ(), expectedRawData.getgyroData().getZ(), absError);

    ASSERT_NEAR(actualRawData.getmagnData().getX(), expectedRawData.getmagnData().getX(), absError);
    ASSERT_NEAR(actualRawData.getmagnData().getY(), expectedRawData.getmagnData().getY(), absError);
    ASSERT_NEAR(actualRawData.getmagnData().getZ(), expectedRawData.getmagnData().getZ(), absError);
  }

  void BNO055Tester ::
      testGetQuatBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Drv::IMU::QuatData actualQuatData;
    this->invoke_to_getQuatData(0, actualQuatData);

    Drv::IMU::QuatData expectedQuatData(0, 0, 0, 0);
    constexpr F32 absError = 0.1;
    ASSERT_EQ(actualQuatData, expectedQuatData);
  }

  void BNO055Tester ::
      testConfigureBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F},
        etl::vector<U8, maxSizeRead>{0xBB, 0x08, 0xA0, 0xFB, 0x32, 0x0F, 0x11, 0x03, 0x15, 0x00},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x07},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x00},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x07},
    };

    etl::vector<etl::vector<U8, maxSizeRead>, 1024> calibrationData;
    for (uint32_t i = 0; i < 1000; i++)
    {
      etl::vector<U8, maxSizeRead> mockVec{0xBB, 0x06};
      for (size_t j = 0; j < 6; j++)
      {
        mockVec.push_back(STest::Pick::lowerUpper(0, 255));
      }
      calibrationData.push_back(mockVec);
    }

    dataPacketRead.insert(dataPacketRead.begin() + dataPacketRead.size() - 1, calibrationData.begin(), calibrationData.end());

    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    this->component.configure(Components::BNO055::Mode::NO_FUSION);

    ASSERT_EVENTS_ReceiveFailError_SIZE(0);
  }

  void BNO055Tester::
      testFailureWriteFailBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x03}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.writeReg(0x07, chosenPage);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_WriteFailError_SIZE(1);
  }
  void BNO055Tester::
      testFailureReadFailBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x02}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ReadFailError_SIZE(1);
  }
  void BNO055Tester::
      testFailureRegmapInvalidAddressBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x02}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0xFF, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ReadFailError_SIZE(1);
  }
  void BNO055Tester::
      testFailureRegmapWriteDisabledBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x05}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.writeReg(0x07, chosenPage);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_RegmapWriteDisabledError_SIZE(1);
  }
  void BNO055Tester::
      testFailureWrongStartByteBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x06}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.writeReg(0x07, chosenPage);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_WrongStartByteError_SIZE(1);
  }

  void BNO055Tester::
      testFailureMaxLengthErrorBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x08}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0xFF, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_MaxLengthError_SIZE(1);
  }
  void BNO055Tester::
      testFailureMinLengthErrorBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x09}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0xFF, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_MinLengthError_SIZE(1);
  }
  void BNO055Tester::
      testFailureRecieveCharacterTimeoutBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x0A}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0xFF, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ReceiveCharacterTimeoutError_SIZE(1);
  }

  void BNO055Tester::
      testFailureResponseHeaderMismatchBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBA, 0x01, 0x0F}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ResponseHeaderMismatchError_SIZE(1);
  }
  void BNO055Tester::
      testFailureResponseSizeMismatchBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F, 0xAA}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ResponseSizeMismatchError_SIZE(1);
  }
  void BNO055Tester::
      testFailureReceiveBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F}};
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ReceiveFailError_SIZE(1);
  }
  void BNO055Tester::
      testFailureSendBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F}};
    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::FAILED);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    constexpr U8 chosenPage = 0;

    U8 stResult = 0;
    Components::BNO055::ErrorCode writeStatus = this->component.readReg(0x36, stResult);
    this->component.errorReport(writeStatus);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_SendFailError_SIZE(1);
  }

  void BNO055Tester::
      testFailureInitBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F},
        etl::vector<U8, maxSizeRead>{0xBB, 0x08, 0xA0, 0xFB, 0x32, 0x0F, 0x11, 0x03, 0x15, 0x00},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x07},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x00},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x00},
    };

    etl::vector<etl::vector<U8, maxSizeRead>, 1024> calibrationData;
    for (uint32_t i = 0; i < 1000; i++)
    {
      etl::vector<U8, maxSizeRead> mockVec{0xBB, 0x06};
      for (size_t j = 0; j < 6; j++)
      {
        mockVec.push_back(STest::Pick::lowerUpper(0, 255));
      }
      calibrationData.push_back(mockVec);
    }

    dataPacketRead.insert(dataPacketRead.begin() + dataPacketRead.size() - 1, calibrationData.begin(), calibrationData.end());

    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    ASSERT_EQ(Components::BNO055::ErrorCode::INIT_FAIL, this->component.initialize(Components::BNO055::Mode::NO_FUSION));
  }
  void BNO055Tester::
      testFailureSelfTestBNO055()
  {
    constexpr size_t maxSizeRead = 128;
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> dataPacketRead = {
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x0F},
        etl::vector<U8, maxSizeRead>{0xBB, 0x08, 0x00, 0xFB, 0x32, 0x0F, 0x11, 0x03, 0x15, 0x00},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x07},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x00},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xEE, 0x01},
        etl::vector<U8, maxSizeRead>{0xBB, 0x01, 0x07},
    };
    etl::vector<etl::vector<U8, maxSizeRead>, 1024> calibrationData;
    for (uint32_t i = 0; i < 1000; i++)
    {
      etl::vector<U8, maxSizeRead> mockVec{0xBB, 0x06};
      for (size_t j = 0; j < 6; j++)
      {
        mockVec.push_back(STest::Pick::lowerUpper(0, 255));
      }
      calibrationData.push_back(mockVec);
    }

    dataPacketRead.insert(dataPacketRead.begin() + dataPacketRead.size() - 1, calibrationData.begin(), calibrationData.end());

    this->setMockReadBuffers(dataPacketRead);
    constexpr size_t sizeOfWriteStatuses = 1024;
    etl::vector<Drv::UART::WriteStatus, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::UART::WriteStatus::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    this->component.configure(Components::BNO055::Mode::NO_FUSION);

    ASSERT_EVENTS_SelfTestError_SIZE(1);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  U32 BNO055Tester ::
      from_getNumOfBytes_handler(NATIVE_INT_TYPE portNum)
  {
    if (readBufferIndex < mockedReadBuffers.size())
    {
      return static_cast<U32>(mockedReadBuffers[readBufferIndex].size());
    }
    else
    {
      return 0;
    }
  }

  U32 BNO055Tester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {
    if (readBufferIndex >= mockedReadBuffers.size())
    {
      return 0;
    }

    if (isFirstReadPerBuffer[readBufferIndex])
    {
      isFirstReadPerBuffer[readBufferIndex] = false;
      return 0;
    }

    const auto &mockDataVec = mockedReadBuffers[readBufferIndex++];
    auto minBufferSize = etl::min(buffer.getSize(), static_cast<U32>(mockDataVec.size()));

    U8 *bufferData = buffer.getData();
    for (U32 i = 0; i < minBufferSize; i++)
    {
      bufferData[i] = mockDataVec[i];
    }

    return minBufferSize;
  }

  Drv::UART::WriteStatus BNO055Tester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {
    return mockedWriteBufferStatuses[writeBufferStatusesIndex++];
  }

  void BNO055Tester ::
      setMockReadBuffers(etl::vector<etl::vector<U8, 128>, 1024> value)
  {

    readBufferIndex = 0;
    mockedReadBuffers.clear();
    isFirstReadPerBuffer.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadBuffers.push_back(value[i]);
      isFirstReadPerBuffer.push_back(true);
    }
  }

  void BNO055Tester ::
      setMockWriteBufferStatuses(etl::span<Drv::UART::WriteStatus> statuses)
  {
    writeBufferStatusesIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }
}
