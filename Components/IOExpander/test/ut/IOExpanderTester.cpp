// ======================================================================
// \title  IOExpanderTester.cpp
// \author aos
// \brief  cpp file for IOExpander component test harness implementation class
// ======================================================================

#include "IOExpanderTester.hpp"
#include "STest/Pick/Pick.hpp"
namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  IOExpanderTester ::
      IOExpanderTester() : IOExpanderGTestBase("IOExpanderTester", IOExpanderTester::MAX_HISTORY_SIZE),
                           component("IOExpander")
  {
    this->initComponents();
    this->connectPorts();
  }

  IOExpanderTester ::
      ~IOExpanderTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void IOExpanderTester ::
      testConfigureIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);

    ASSERT_EVENTS_SIZE(0);
    bool actualInitStatus = this->component.initializationComplete;
    ASSERT_TRUE(actualInitStatus);
    bool actualCRCEnabledStatus = this->component.isCRCEnabled;
    ASSERT_TRUE(actualCRCEnabledStatus);
    bool actualOversamplingEnabledStatus = this->component.isOversamplingEnabled;
    ASSERT_TRUE(actualOversamplingEnabledStatus);
  }

  void IOExpanderTester ::
      testReadAnalogInputIOExpander()
  {
    this->testConfigureIOExpander();
    this->invoke_to_calibrateADCOffset(0);

    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawData = {
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x00, this->component.calculateCRC8(0x00)}};
    this->setMockWriteReadBuffers(writeReadRawData);
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> readADCData = {
        etl::vector<U8, maxSizeOfBuffer>{0xF4, this->component.calculateCRC8(0xF4), 0x6C, this->component.calculateCRC8(0x6C), 0x00, this->component.calculateCRC8(0x00)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x10, this->component.calculateCRC8(0x10)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x20, this->component.calculateCRC8(0x20)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x30, this->component.calculateCRC8(0x30)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x40, this->component.calculateCRC8(0x40)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x50, this->component.calculateCRC8(0x50)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x60, this->component.calculateCRC8(0x60)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x70, this->component.calculateCRC8(0x70)}};
    this->setMockReadBuffers(readADCData);

    etl::vector<Drv::I2C::Status, maxSizeOfOperations> writeStatuses;
    writeStatuses.assign(maxSizeOfOperations, Drv::I2C::Status::SUCCESS);
    this->setMockWriteBufferStatuses(writeStatuses);

    U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    auto analogInChannel = Drv::IOExpander::Channel::Channel_1;
    Drv::IOExpander::ReadAnaloglInputData actualReadAnalogInputData;
    this->invoke_to_readAnalogInput(0, analogInChannel, actualReadAnalogInputData);
    U16 expectedAdcCode = 0xF46C;
    ASSERT_EQ(actualReadAnalogInputData.getAdcCode(), expectedAdcCode);
    ASSERT_TRUE(actualReadAnalogInputData.getIsValid());
    constexpr F32 absError = 0.1;
    constexpr F32 expectedVoltage = 3.1;
    ASSERT_NEAR(actualReadAnalogInputData.getVoltageValue(), expectedVoltage, absError);

    Drv::IOExpander::NumOfThresholdHitData actualNumOfThresholdHitData;
    this->invoke_to_getNumOfThresholdHit(0, analogInChannel, actualNumOfThresholdHitData);

    constexpr U32 expectedThresholdHitMaxCounter = 0;
    constexpr U32 expectedThresholdHitMinCounter = 0;
    ASSERT_EQ(actualNumOfThresholdHitData.getThresholdsHitCounter().getMax(), expectedThresholdHitMaxCounter);
    ASSERT_EQ(actualNumOfThresholdHitData.getThresholdsHitCounter().getMin(), expectedThresholdHitMinCounter);
    ASSERT_EVENTS_SIZE(0);
  }

  void IOExpanderTester ::
      testReadDigitalMultipleInputsIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);
    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawDataDigitalInputs = {
        etl::vector<U8, maxSizeOfBuffer>{0b0000'0100, this->component.calculateCRC8(0b0000'0100)}};
    this->setMockWriteReadBuffers(writeReadRawDataDigitalInputs);

    Drv::IOExpander::Ports ports(false, false, true, true, false, false, false, false);
    Drv::IOExpander::DigitalMultipleInputsData actualData;
    this->invoke_to_readDigitalMultipleInputs(0, ports, actualData);
    ASSERT_EVENTS_SIZE(0);
    auto channel3State = actualData.getStates().getChannel3();
    auto channel4State = actualData.getStates().getChannel4();
    auto expectedStateLow = Fw::Logic::LOW;
    auto expectedStateHigh = Fw::Logic::HIGH;
    ASSERT_EQ(channel3State, expectedStateHigh);
    ASSERT_EQ(channel4State, expectedStateLow);
  }

  void IOExpanderTester ::
      testWriteDigitalMultipleOutputsIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);
    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;

    Drv::IOExpander::Ports ports(false, false, false, false, true, true, true, true);
    Drv::IOExpander::PortsStates portsStates(Fw::Logic::LOW, Fw::Logic::HIGH, Fw::Logic::LOW, Fw::Logic::HIGH,
                                             Fw::Logic::LOW, Fw::Logic::HIGH, Fw::Logic::LOW, Fw::Logic::HIGH);
    Drv::IOExpander::DigitalMultipleOutputsData data(ports, portsStates);
    bool IsValid;
    this->invoke_to_writeDigitalMultipleOutputs(0, data, IsValid);
    ASSERT_EVENTS_SIZE(0);
    etl::vector<U8, 32> capturedWrittenData = this->getCapturedWriteBuffer();
    constexpr U8 expectedWriteRegValue = 0b1010'0000;
    ASSERT_EQ(capturedWrittenData[4], expectedWriteRegValue);
    ASSERT_TRUE(IsValid);
  }

  void IOExpanderTester ::
      testReadDigitalSingleInputIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);
    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawDataDigitalInputs = {
        etl::vector<U8, maxSizeOfBuffer>{0b0000'0100, this->component.calculateCRC8(0b0000'0100)}};
    this->setMockWriteReadBuffers(writeReadRawDataDigitalInputs);

    auto channel3 = Drv::IOExpander::Channel::Channel_3;
    Drv::IOExpander::DigitalSingleInputData data;
    this->invoke_to_readDigitalSingleInput(0, channel3, data);
    auto actualChannel3State = data.getState();
    auto actualChannel3isValid = data.getIsValid();
    auto expectedStateHigh = Fw::Logic::HIGH;
    ASSERT_TRUE(actualChannel3isValid);
    ASSERT_EQ(actualChannel3State, expectedStateHigh);
  }
  void IOExpanderTester ::
      testWriteDigitalSingleOutputIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);

    Drv::IOExpander::DigitalSingleOutputData data(Drv::IOExpander::Channel::Channel_8, Fw::Logic::HIGH);
    bool IsValid;
    this->invoke_to_writeDigitalSingleOutput(0, data, IsValid);
    ASSERT_EVENTS_SIZE(0);
    ASSERT_TRUE(IsValid);

    etl::vector<U8, 32> capturedWrittenData = this->getCapturedWriteBuffer();
    constexpr U8 expectedWriteRegValue = 0b1000'0000;
    ASSERT_EQ(capturedWrittenData[4], expectedWriteRegValue);
  }

  void IOExpanderTester ::
      testFailureNotInitedIOExpander()
  {
    Drv::IOExpander::DigitalSingleOutputData data(Drv::IOExpander::Channel::Channel_8, Fw::Logic::HIGH);
    bool IsValid;
    this->invoke_to_writeDigitalSingleOutput(0, data, IsValid);
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_NotInited_SIZE(1);
  }
  void IOExpanderTester ::
      testFailureWrongChannelConfigurationIOExpander()
  {
    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);

    Drv::IOExpander::DigitalSingleOutputData data(Drv::IOExpander::Channel::Channel_8, Fw::Logic::HIGH);
    bool IsValid;
    this->invoke_to_writeDigitalSingleOutput(0, data, IsValid);
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_WrongChannelConfiguration_SIZE(1);
    ASSERT_FALSE(IsValid);

    etl::vector<U8, 32> capturedWrittenData = this->getCapturedWriteBuffer();
    constexpr U8 expectedWriteRegValue = 0b1000'0000;
    ASSERT_NE(capturedWrittenData[4], expectedWriteRegValue);
  }
  void IOExpanderTester ::
      testFailureCommunicationErrorIOExpander()
  {

    Components::IOExpander::ChannelConfig config1;
    config1.channel = Drv::IOExpander::Channel::Channel_1;
    config1.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config2;
    config2.channel = Drv::IOExpander::Channel::Channel_2;
    config2.ioType = Drv::IOExpander::IOType::AnalogIn;
    Components::IOExpander::ChannelConfig config3;
    config3.channel = Drv::IOExpander::Channel::Channel_3;
    config3.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config4;
    config4.channel = Drv::IOExpander::Channel::Channel_4;
    config4.ioType = Drv::IOExpander::IOType::DigitalIn;
    Components::IOExpander::ChannelConfig config5;
    config5.channel = Drv::IOExpander::Channel::Channel_5;
    config5.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config6;
    config6.channel = Drv::IOExpander::Channel::Channel_6;
    config6.ioType = Drv::IOExpander::IOType::DigitalOutOD;
    Components::IOExpander::ChannelConfig config7;
    config7.channel = Drv::IOExpander::Channel::Channel_7;
    config7.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ChannelConfig config8;
    config8.channel = Drv::IOExpander::Channel::Channel_8;
    config8.ioType = Drv::IOExpander::IOType::DigitalOutPP;
    Components::IOExpander::ConfigTable config = {config1, config2, config3, config4, config5, config6, config7, config8};

    this->configHelperFunction(&config);
    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawDataDigitalInputs = {
        etl::vector<U8, maxSizeOfBuffer>{0b0000'0100, this->component.calculateCRC8(0b0000'0100)}};
    this->setMockWriteReadBuffers(writeReadRawDataDigitalInputs);

    etl::vector<Drv::I2C::Status, maxSizeOfOperations> writeReadStatuses;
    writeReadStatuses.assign(maxSizeOfOperations, Drv::I2C::Status::WRITE_READ_ERR);
    this->setMockWriteReadBufferStatuses(writeReadStatuses);
    auto channel3 = Drv::IOExpander::Channel::Channel_3;
    Drv::IOExpander::DigitalSingleInputData data;
    this->invoke_to_readDigitalSingleInput(0, channel3, data);
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_CommunicationError_SIZE(1);
    auto actualChannel3State = data.getState();
    auto actualChannel3isValid = data.getIsValid();
    auto expectedStateHigh = Fw::Logic::HIGH;
    ASSERT_FALSE(actualChannel3isValid);
    ASSERT_NE(actualChannel3State, expectedStateHigh);
  }
  void IOExpanderTester ::
      testFailureCRCErrorIOExpander()
  {

    this->testConfigureIOExpander();
    this->invoke_to_calibrateADCOffset(0);

    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawData = {
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x00, this->component.calculateCRC8(0x00)}};
    this->setMockWriteReadBuffers(writeReadRawData);
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> readADCData = {
        etl::vector<U8, maxSizeOfBuffer>{0xF4, this->component.calculateCRC8(0xF0), 0x6C, this->component.calculateCRC8(0x60), 0x00, this->component.calculateCRC8(0x0F)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x10, this->component.calculateCRC8(0x10)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x20, this->component.calculateCRC8(0x20)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x30, this->component.calculateCRC8(0x30)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x40, this->component.calculateCRC8(0x40)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x50, this->component.calculateCRC8(0x50)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x60, this->component.calculateCRC8(0x60)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x70, this->component.calculateCRC8(0x70)}};
    this->setMockReadBuffers(readADCData);

    etl::vector<Drv::I2C::Status, maxSizeOfOperations> writeStatuses;
    writeStatuses.assign(maxSizeOfOperations, Drv::I2C::Status::SUCCESS);
    this->setMockWriteBufferStatuses(writeStatuses);

    U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_CRCError_SIZE(1);
  }
  void IOExpanderTester ::
      testFailureCalibrationUnfinishedIOExpander()
  {
    this->testConfigureIOExpander();
    this->invoke_to_calibrateADCOffset(0);

    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawData = {
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)},
        etl::vector<U8, maxSizeOfBuffer>{0x02, this->component.calculateCRC8(0x02)}};
    this->setMockWriteReadBuffers(writeReadRawData);
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> readADCData = {
        etl::vector<U8, maxSizeOfBuffer>{0xF4, this->component.calculateCRC8(0xF4), 0x6C, this->component.calculateCRC8(0x6C), 0x00, this->component.calculateCRC8(0x00)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x10, this->component.calculateCRC8(0x10)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x20, this->component.calculateCRC8(0x20)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x30, this->component.calculateCRC8(0x30)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x40, this->component.calculateCRC8(0x40)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x50, this->component.calculateCRC8(0x50)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x60, this->component.calculateCRC8(0x60)},
        etl::vector<U8, maxSizeOfBuffer>{0x0F, this->component.calculateCRC8(0x0F), 0x6C, this->component.calculateCRC8(0x6C), 0x70, this->component.calculateCRC8(0x70)}};
    this->setMockReadBuffers(readADCData);

    etl::vector<Drv::I2C::Status, maxSizeOfOperations> writeStatuses;
    writeStatuses.assign(maxSizeOfOperations, Drv::I2C::Status::SUCCESS);
    this->setMockWriteBufferStatuses(writeStatuses);

    U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_CalibrationUnfinishedError_SIZE(1);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void IOExpanderTester ::
      from_alert_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Logic &state)
  {
    state = Fw::Logic::LOW;
  }

  Drv::I2C::Status IOExpanderTester ::
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
      serBufferData[i] = mockDataVec[i];
    }

    return mockedReadBufferStatuses[readBufferStatusesIndex++];
  }

  Drv::I2C::Status IOExpanderTester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &serBuffer,
          U32 timeout)
  {
    U8 *dest = serBuffer.getData();
    U32 sizeOfBuffer = serBuffer.getSize();
    capturedWriteData.assign(dest, dest + sizeOfBuffer);
    return mockedWriteBufferStatuses[writeBufferStatusesIndex++];
  }

  Drv::I2C::Status IOExpanderTester ::
      from_writeRead_handler(
          NATIVE_INT_TYPE portNum,
          U32 addr,
          Fw::Buffer &writeBuffer,
          Fw::Buffer &readBuffer,
          U32 timeout)
  {
    U8 *readBufferData = readBuffer.getData();
    const auto &mockDataVec = mockedWriteReadBuffers[writeReadBufferIndex++];
    auto minBufferSize = etl::min(readBuffer.getSize(), static_cast<U32>(mockDataVec.size()));

    for (U32 i = 0; i < minBufferSize; i++)
    {
      readBufferData[i] = mockDataVec[i];
    }

    return mockedWriteReadBufferStatuses[writeReadBufferStatusesIndex++];
  }

  void IOExpanderTester ::
      setMockReadBuffers(etl::vector<etl::vector<U8, 32>, 1024> value)
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

  void IOExpanderTester ::
      setMockReadBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {

    readBufferStatusesIndex = 0;
    mockedReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedReadBufferStatuses.push_back(statuses[i]);
    }
  }

  void IOExpanderTester ::
      setMockWriteBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {
    writeBufferStatusesIndex = 0;
    mockedWriteBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteBufferStatuses.push_back(statuses[i]);
    }
  }

  void IOExpanderTester ::
      setMockWriteReadBuffers(etl::vector<etl::vector<U8, 32>, 1024> value)
  {
    writeReadBufferIndex = 0;
    writeReadBufferStatusesIndex = 0;
    mockedWriteReadBuffers.clear();
    mockedWriteReadBufferStatuses.clear();
    for (size_t i = 0; i < value.size(); i++)
    {
      mockedWriteReadBuffers.push_back(value[i]);
      mockedWriteReadBufferStatuses.push_back(Drv::I2C::Status::SUCCESS);
    }
  }
  void IOExpanderTester ::
      setMockWriteReadBufferStatuses(etl::span<Drv::I2C::Status> statuses)
  {
    writeReadBufferStatusesIndex = 0;
    mockedWriteReadBufferStatuses.clear();
    for (size_t i = 0; i < statuses.size(); i++)
    {
      mockedWriteReadBufferStatuses.push_back(statuses[i]);
    }
  }

  const etl::vector<U8, 32> &IOExpanderTester::getCapturedWriteBuffer()
  {
    return this->capturedWriteData;
  }

  void IOExpanderTester ::
      configHelperFunction(Components::IOExpander::ConfigTable *config)
  {
    uint8_t devAddr = 0x67;
    static const etl::array<I32, 8> analogOffsets = {0, 0, 0, 0, 0, 0, 0, 0};
    static const etl::array<I32, 8> maxThresholds = {3300, 3300, 3300, 3300, 3300, 3300, 3300, 3300};
    static const etl::array<I32, 8> minThresholds = {300, 300, 300, 300, 300, 300, 300, 300};
    Components::IOExpander::AnalogConfig analogConfig;
    analogConfig.analogOffsetsMiliVolts = &analogOffsets;
    analogConfig.maxThresholdsMiliVolts = &maxThresholds;
    analogConfig.minThresholdsMiliVolts = &minThresholds;

    auto mode = Components::IOExpander::AnalogMeasurementsMode::AUTO;
    auto oversamplingRatio = Components::IOExpander::OversamplingRatio::SAMPLES8;

    constexpr U32 maxSizeOfBuffer = 32;
    constexpr U32 maxSizeOfOperations = 1024;
    etl::vector<etl::vector<U8, maxSizeOfBuffer>, maxSizeOfOperations> writeReadRawData = {
        etl::vector<U8, maxSizeOfBuffer>{0x81, this->component.calculateCRC8(0x81)},
        etl::vector<U8, maxSizeOfBuffer>{0x04, this->component.calculateCRC8(0x04)},
        etl::vector<U8, maxSizeOfBuffer>{0x40, this->component.calculateCRC8(0x40)},
        etl::vector<U8, maxSizeOfBuffer>{0x00, this->component.calculateCRC8(0x00)},
        etl::vector<U8, maxSizeOfBuffer>{0x01, this->component.calculateCRC8(0x01)},
        etl::vector<U8, maxSizeOfBuffer>{0x01, this->component.calculateCRC8(0x01)},
        etl::vector<U8, maxSizeOfBuffer>{0x03, this->component.calculateCRC8(0x03)},
        etl::vector<U8, maxSizeOfBuffer>{0x01, this->component.calculateCRC8(0x01)},
        etl::vector<U8, maxSizeOfBuffer>{0x01, this->component.calculateCRC8(0x01)},
        etl::vector<U8, maxSizeOfBuffer>{0x04, this->component.calculateCRC8(0x04)},
        etl::vector<U8, maxSizeOfBuffer>{0x81, this->component.calculateCRC8(0x81)},
        etl::vector<U8, maxSizeOfBuffer>{0x10, this->component.calculateCRC8(0x10)}};
    this->setMockWriteReadBuffers(writeReadRawData);

    etl::vector<Drv::I2C::Status, maxSizeOfOperations> writeStatuses;
    writeStatuses.assign(maxSizeOfOperations, Drv::I2C::Status::SUCCESS);
    this->setMockWriteBufferStatuses(writeStatuses);

    this->component.configure(devAddr, config, &analogConfig, mode, oversamplingRatio);
  }
}