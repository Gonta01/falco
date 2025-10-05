// ======================================================================
// \title  ICM20948Tester.cpp
// \author aos
// \brief  cpp file for ICM20948 component test harness implementation class
// ======================================================================

#include "ICM20948Tester.hpp"
#include "STest/Pick/Pick.hpp"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  ICM20948Tester ::
      ICM20948Tester() : ICM20948GTestBase("ICM20948Tester", ICM20948Tester::MAX_HISTORY_SIZE),
                         component("ICM20948")
  {
    this->initComponents();
    this->connectPorts();
  }

  ICM20948Tester ::
      ~ICM20948Tester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void ICM20948Tester ::
      testIsConnectedICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0xEA}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 2;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses = {Drv::I2C::Status::SUCCESS, Drv::I2C::Status::SUCCESS};
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.isConnected();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testDeviceResetICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 5;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.deviceReset();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigClockSourceICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x01}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configClockSource();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testEnableGyroAndAccelICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x40}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.enableGyroAndAccel();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testEnableFIFOFalseICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x40},
        etl::vector<U8, sizeOfDataPacketRead>{0x00},
    };
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 6;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.enableFIFO(false);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testDisableDMPICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x7F},
        etl::vector<U8, sizeOfDataPacketRead>{0x7F},
    };
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 4;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.disableDMP();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testSetConfigICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x07},
        etl::vector<U8, sizeOfDataPacketRead>{0x03},
    };
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 6;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.setConfig(Interfaces::ICM20948::AccelFullScale::Scale4g, Interfaces::ICM20948::GyroFullScale::Scale2000dps);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigureFIFOICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
        etl::vector<U8, sizeOfDataPacketRead>{0x1F},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
    };
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 9;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configureFIFO(true);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigureInterruptPinICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x20}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configureInterruptPin(false, false, true);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigureDataReadyInterruptICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configureDataReadyInterrupt(false);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigureGyroSampleRateICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configureGyroSampleRate();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testConfigureAccelSampleRateICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 3;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.configureAccelSampleRate();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testInitMagnetometerICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x20},
        etl::vector<U8, sizeOfDataPacketRead>{0x00},
        etl::vector<U8, sizeOfDataPacketRead>{0x17},
        etl::vector<U8, sizeOfDataPacketRead>{0x00},
        etl::vector<U8, sizeOfDataPacketRead>{0x20},
        etl::vector<U8, sizeOfDataPacketRead>{0x48, 0x09}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 32;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.initMagnetometer();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester::
      testCalibrateGyroOffsetsICM20948()
  {
    this->setMockReadCalibrateCoef();
    Interfaces::ICM20948::ErrorCode status = this->component.calibrateGyroOffsets();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testEnableFIFOTrueICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x20},
        etl::vector<U8, sizeOfDataPacketRead>{0x60}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 32;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.enableFIFO(true);

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester ::
      testResetFIFOICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x04}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 32;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.resetFIFO();

    ASSERT_EVENTS_SIZE(0);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::NoError);
  }

  void ICM20948Tester::
      testPollRawDataICM20948()
  {

    this->testIsConnectedICM20948();
    this->testDeviceResetICM20948();
    this->testConfigClockSourceICM20948();
    this->testEnableGyroAndAccelICM20948();
    this->testEnableFIFOFalseICM20948();
    this->testDisableDMPICM20948();
    this->testSetConfigICM20948();
    this->testConfigureFIFOICM20948();
    this->testConfigureInterruptPinICM20948();
    this->testConfigureDataReadyInterruptICM20948();
    this->testConfigureGyroSampleRateICM20948();
    this->testConfigureAccelSampleRateICM20948();
    this->testInitMagnetometerICM20948();
    this->testCalibrateGyroOffsetsICM20948();
    this->testEnableFIFOTrueICM20948();
    this->testResetFIFOICM20948();
    this->component.initializationComplete = true;

    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00},
        etl::vector<U8, sizeOfDataPacketRead>{0x00, 0x9A},
        etl::vector<U8, sizeOfDataPacketRead>{0x84, 0x90, 0x70, 0x98, 0x20, 0x00,
                                              0x02, 0x15, 0x09, 0x05, 0x0A, 0xBC,
                                              0x84, 0x90,
                                              0x03, 0x08, 0x02, 0x07, 0x01, 0x10,
                                              0x70, 0x98},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},
        etl::vector<U8, sizeOfDataPacketRead>{0x01},

    };
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 32;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    ASSERT_EQ(Drv::IMU::Status::SUCCESS, this->invoke_to_pollRawData(0));

    constexpr F32 absError = 0.1;

    Drv::IMU::AccelData actualAccelData;
    this->invoke_to_getAccelData(0, actualAccelData);

    Drv::IMU::MagnData actualMagnData;
    this->invoke_to_getMagnData(0, actualMagnData);

    Drv::IMU::GyroData actualGyroData;
    this->invoke_to_getGyroData(0, actualGyroData);

    Drv::IMU::AccelData expectedAccelData(-37.8, 34.5, 9.8);
    ASSERT_NEAR(actualAccelData.getX(), expectedAccelData.getX(), absError);
    ASSERT_NEAR(actualAccelData.getY(), expectedAccelData.getY(), absError);
    ASSERT_NEAR(actualAccelData.getZ(), expectedAccelData.getZ(), absError);

    Drv::IMU::GyroData expectedGyroData(32.5, 140.8, 167.5);
    ASSERT_NEAR(actualGyroData.getX(), expectedGyroData.getX(), absError);
    ASSERT_NEAR(actualGyroData.getY(), expectedGyroData.getY(), absError);
    ASSERT_NEAR(actualGyroData.getZ(), expectedGyroData.getZ(), absError);

    Drv::IMU::MagnData expectedMagnData(307.6, 269.1, 614.5);
    ASSERT_NEAR(actualMagnData.getX(), expectedMagnData.getX(), absError);
    ASSERT_NEAR(actualMagnData.getY(), expectedMagnData.getY(), absError);
    ASSERT_NEAR(actualMagnData.getZ(), expectedMagnData.getZ(), absError);
  }

  void ICM20948Tester ::
      testPollQuatDataICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00, 0x10},
        etl::vector<U8, sizeOfDataPacketRead>{0x04, 0x00},
        etl::vector<U8, sizeOfDataPacketRead>{
            0x06, 0x66, 0x66, 0x66,
            0x0C, 0xCC, 0xCC, 0xCD,
            0x13, 0x33, 0x33, 0x33, 
            0x19, 0x99},
        etl::vector<U8, sizeOfDataPacketRead>{0x04}};
    constexpr size_t sizeOfWriteStatuses = 32;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);
    this->setMockReadBuffers(dataPacketRead);
    this->component.initializationComplete = true;
    this->component.retrieveQuaternionData = true;
    this->invoke_to_pollQuatData(0);

    Drv::IMU::QuatData actualQuatData;
    auto status = this->invoke_to_getQuatData(0, actualQuatData);
    ASSERT_EQ(status, Drv::I2C::Status::SUCCESS);

    Drv::IMU::QuatData expectedQuatData(0.92, 0.1, 0.2, 0.3);
    constexpr F32 absError = 0.01;
    ASSERT_NEAR(actualQuatData.getQ0(), expectedQuatData.getQ0(), absError);
    ASSERT_NEAR(actualQuatData.getQ1(), expectedQuatData.getQ1(), absError);
    ASSERT_NEAR(actualQuatData.getQ2(), expectedQuatData.getQ2(), absError);
    ASSERT_NEAR(actualQuatData.getQ3(), expectedQuatData.getQ3(), absError);
  }

  void ICM20948Tester::testFailureNotInitedICM20948()
  {

    Drv::IMU::AccelData actualAccelData;
    this->invoke_to_getAccelData(0, actualAccelData);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_NotInited_SIZE(1);
  }
  void ICM20948Tester::testFailureReadICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0xEA}};
    this->setMockReadBuffers(dataPacketRead);
    etl::vector<Drv::I2C::Status, 1> dataPacketReadStatuses = {Drv::I2C::Status::READ_ERR};
    this->setMockReadBufferStatuses(dataPacketReadStatuses);

    constexpr size_t sizeOfWriteStatuses = 2;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses = {Drv::I2C::Status::SUCCESS, Drv::I2C::Status::SUCCESS};
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.isConnected();
    this->component.errorReport(status);
    ASSERT_EQ(status, Interfaces::ICM20948::ErrorCode::FailedReadIMURegister);
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_FailedReadIMURegister_SIZE(1);
  }
  void ICM20948Tester::testFailureWriteICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0x00}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 5;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    dataPacketWriteStatuses[1] = Drv::I2C::Status::WRITE_ERR;
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.deviceReset();
    this->component.errorReport(status);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_FailedWriteToIMURegister_SIZE(1);
  }
  void ICM20948Tester::testFailureReadNotEqualToWrittenICM20948()
  {
    constexpr size_t sizeOfDataPacketRead = 32;
    etl::vector<etl::vector<U8, sizeOfDataPacketRead>, sizeOfDataPacketRead> dataPacketRead = {
        etl::vector<U8, sizeOfDataPacketRead>{0xFF}};
    this->setMockReadBuffers(dataPacketRead);

    constexpr size_t sizeOfWriteStatuses = 5;
    etl::vector<Drv::I2C::Status, sizeOfWriteStatuses> dataPacketWriteStatuses;
    for (size_t i = 0; i < sizeOfWriteStatuses; i++)
    {
      dataPacketWriteStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
    this->setMockWriteBufferStatuses(dataPacketWriteStatuses);

    Interfaces::ICM20948::ErrorCode status = this->component.deviceReset();
    this->component.errorReport(status);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_ReadNotEqualTOWrittenRegister_SIZE(1);
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void ICM20948Tester ::
      from_intPin_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Logic &state)
  {
    state = Fw::Logic::LOW;
  }

  Drv::I2C::Status ICM20948Tester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {

    U8 *serBufferData = serBuffer.getData();
    const auto &mockDataVec = mockedReadBuffers[readBufferIndex++];
    auto minBufferSize = etl::min(serBuffer.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      serBuffer.getData()[i] = mockDataVec[i];
    }

    return mockedReadBufferStatuses[readBufferStatusesIndex++];
  }

  Drv::I2C::Status ICM20948Tester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    return mockedWriteBufferStatuses[writeBufferStatusesIndex++];
  }

  void ICM20948Tester ::
      setMockReadBuffers(etl::vector<etl::vector<U8, 32>, 32> value)
  {

    readBufferIndex = 0;
    readBufferStatusesIndex = 0;
    mockedReadBuffers.clear();
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedReadBuffers.push_back(value[i]);
      mockedReadBufferStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
  }

  void ICM20948Tester ::
      setMockReadCalibrateCoef()
  {
    readBufferIndex = 0;
    readBufferStatusesIndex = 0;
    writeBufferStatusesIndex = 0;
    mockedReadBuffers.clear();
    mockedReadBufferStatuses.clear();
    mockedWriteBufferStatuses.clear();

    for (uint32_t i = 0; i < 1024; i++)
    {
      etl::vector<U8, 32> mockVec;
      for (size_t j = 0; j < 6; j++)
      {
        mockVec[j] = STest::Pick::lowerUpper(0, 255);
      }
      mockedReadBuffers.push_back(mockVec);
      mockedReadBufferStatuses.push_back(Drv::I2C::Status::SUCCESS);
      mockedWriteBufferStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
  }

  void ICM20948Tester ::
      setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {

    readBufferStatusesIndex = 0;
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadBufferStatuses.push_back(statuses[i]);
    }
  }

  void ICM20948Tester ::
      setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {
    writeBufferStatusesIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }
}
