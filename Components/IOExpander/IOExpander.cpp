// ======================================================================
// \title  IOExpander.cpp
// \author user
// \brief  cpp file for IOExpander component implementation class
// ======================================================================

#include "Components/IOExpander/IOExpander.hpp"
#include "FpConfig.hpp"
#include <etl/vector.h>
#include <etl/algorithm.h>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IOExpander::IOExpander(const char* const compName) : 
  IOExpanderComponentBase(compName) {

  }

  IOExpander::~IOExpander() {

  }

  void IOExpander::configure(uint8_t devAddr, const ConfigTable *config, const AnalogConfig *analogConfig, AnalogMeasurementsMode mode, Components::IOExpander::OversamplingRatio oversamplingRatio) {
    initializationComplete = false;
    this->devAddr = devAddr;
    if (config == nullptr) {
        errorReport(ErrorCode::WrongConfiguration);
        return;
    }

    if (analogConfig != NULL) {
        for (U32 i = 0; i < Drv::IOExpander::Channel::ChannelAmount; i++) {
            analogOffsetsMiliVolts.at(i) = analogConfig->analogOffsetsMiliVolts->at(i);
            FW_ASSERT((analogConfig->minThresholdsMiliVolts->at(i) < analogConfig->maxThresholdsMiliVolts->at(i)), analogConfig->minThresholdsMiliVolts->at(i));
            FW_ASSERT((analogConfig->maxThresholdsMiliVolts->at(i) <= thresholdMiliVoltMaxValue), analogConfig->maxThresholdsMiliVolts->at(i));
            minThresholdsMiliVolts.at(i) = analogConfig->minThresholdsMiliVolts->at(i);
            maxThresholdsMiliVolts.at(i) = analogConfig->maxThresholdsMiliVolts->at(i);
            thresholdsAreSet = true;
        }
    }
    
    analogMeasurementsMode = mode;
    
    if (!checkCRCEnabled()) {
        return;
    }

    if(softwareReset()) {
        isCRCEnabled = false;
        Os::Task::delay(waitTimeAfterReset);
    }
      else {
        return;
    }

    if (!checkPowerUpConfig()) {
        if(softwareReset()) {
          Os::Task::delay(waitTimeAfterReset);
        }
        else {
          return;
        }
    }

    if (!crcEnable()) {
        return;
    }
    
    if (!enableCRCAlert()) {
        return;
    }

    if (!configAlertPin()) {
        return;
    }

    uint8_t status = 0;
    bool isReadSuccess = readReg(ADS7138_REG::SYSTEM_STATUS, &status);
    if (isReadSuccess) {
        if ((status & ADS7138_SYSTEM_STATUS_REG::DEFAULT) != ADS7138_SYSTEM_STATUS_REG::DEFAULT) {
            return;
        }
    } else {
        return;
    }

    ErrorCode configStatus = setConfig(config);
    if (configStatus != ErrorCode::NoError) {
        return;
    }

    configStatus = setOversamplingRatio(oversamplingRatio);
    if (configStatus != ErrorCode::NoError) {
        return;
    }

    if (analogMeasurementsMode == AnalogMeasurementsMode::AUTO) {
        configStatus = configAutoSeqMode();
        if (configStatus != ErrorCode::NoError) {
            return;
        }

        configStatus = seqStart();
        if (configStatus != ErrorCode::NoError) {
            return;
        }
    }
    else if (analogMeasurementsMode == AnalogMeasurementsMode::ONE_SHOT) {
        configStatus = configManualSeqMode();
        if (configStatus != ErrorCode::NoError) {
            return;
        }
    }
    
    
    configStatus = appendChannelID();
    if (configStatus != ErrorCode::NoError) {
        return;
    }

    initializationComplete = true;
  }

  bool IOExpander::initDigitalOutputs() {
    uint8_t regGpoVal = 0;
    bool readResult = readReg(ADS7138_REG::GPO_VALUE, &regGpoVal);
    if (readResult == false) {
        errorReport(ErrorCode::CommError);
    }
    digitalOutputsValue = regGpoVal;

    return true;
  }

  void IOExpander::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    bool calibrateStart = false;
    mutex.lock();
    calibrateStart = (isOffsetCalibrationInitiated == true);
    isOffsetCalibrationInitiated = false;
    mutex.unLock();

    if (calibrateStart) {
        auto calibrateStatus = calibrateADCOffset();
        if (calibrateStatus != ErrorCode::NoError) {
            errorReport(calibrateStatus);
        }
    }

    if (analogMeasurementsMode == AnalogMeasurementsMode::AUTO) {
        auto pollStatus = pollAutoAnalogIn();
        if (pollStatus != ErrorCode::NoError) {
            errorReport(pollStatus);
        }
    }
  }

  void IOExpander::getNumOfThresholdHit_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::Channel& channel, Drv::IOExpander::NumOfThresholdHitData& data) {
    Drv::IOExpander::ThresholdsHitCounter thresholdsHitCounter;

    mutex.lock();
    thresholdsHitCounter.setMin(minThresholdsHitCounter.at(channel));
    thresholdsHitCounter.setMax(maxThresholdsHitCounter.at(channel));
    data.setThresholdsHitCounter(thresholdsHitCounter);
    data.setIsValid(thresholdsAreSet);
    mutex.unLock();
  }

  void IOExpander::readAnalogInput_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::Channel& channel, Drv::IOExpander::ReadAnaloglInputData& data){
    data.setIsValid(false);
    if (initializationComplete == false) {
        errorReport(ErrorCode::NotInitialized);
        return;
    }
    
    mutex.lock();
    auto isCalibrationUnfinished = (calibrationUnfinished == true);
    mutex.unLock();

    if (isCalibrationUnfinished) {
        errorReport(ErrorCode::CalibrationUnfinished);
        return;
    }

    if (analogMeasurementsMode == AnalogMeasurementsMode::ONE_SHOT) {
        auto analogValueResult = getAnalogValue(channel);
        if (analogValueResult.has_value()) {
            auto adcCode = analogValueResult.value();
            data.setAdcCode(adcCode);

            F32 voltageOffset = static_cast<F32>(analogOffsetsMiliVolts.at(channel)) / voltInMiliVoltCoeff;
            auto voltageValue = (static_cast<F32>(adcCode) * ADC_VREF / maxADCCode) + voltageOffset;
            data.setVoltageValue(voltageValue);
        } 
        else {
            errorReport(analogValueResult.error());
            return;
        }
    }
    else if (analogMeasurementsMode == AnalogMeasurementsMode::AUTO) {
        mutex.lock();
        data.setAdcCode(autoAdcCode.at(channel));
        data.setVoltageValue(autoVoltageValues.at(channel));
        mutex.unLock();
    }
    else {
        errorReport(ErrorCode::WrongConfiguration);
        return;
    }
    data.setIsValid(true);
  }

  void IOExpander::readDigitalSingleInput_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::Channel& channel, Drv::IOExpander::DigitalSingleInputData& data) {
    data.setIsValid(false);
    if (initializationComplete == false) {
        errorReport(ErrorCode::NotInitialized);
        return;
    }

    auto channelStateResult = getDigitalInput(channel);
    if (channelStateResult.has_value()) {
      auto state = (channelStateResult.value() == GPIOState::GpioSet) ? Fw::Logic::HIGH : Fw::Logic::LOW;
      data.setState(state);
    } else {
        data.setIsValid(false);
        errorReport(channelStateResult.error());
        return;
    }
    data.setIsValid(true);
  }

  void IOExpander::readDigitalMultipleInputs_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::Ports& ports, Drv::IOExpander::DigitalMultipleInputsData& data) {
    data.setIsValid(false);
    if (initializationComplete == false) {
        errorReport(ErrorCode::NotInitialized);
        return;
    }

    bool isCh0ConfigCorrect = true;
    bool isCh1ConfigCorrect = true;
    bool isCh2ConfigCorrect = true;
    bool isCh3ConfigCorrect = true;

    bool isCh4ConfigCorrect = true;
    bool isCh5ConfigCorrect = true;
    bool isCh6ConfigCorrect = true;
    bool isCh7ConfigCorrect = true;
    
    uint8_t regGpiVal = 0;
    bool readResult = readReg(ADS7138_REG::GPI_VALUE, &regGpiVal);
    if (readResult == false) {
        errorReport(ErrorCode::CommError);
        return;
    }
    
    Drv::IOExpander::PortsStates states;

    if (ports.getChannel1() == true) {
        isCh1ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_1).ioType == Drv::IOExpander::IOType::DigitalIn);
        
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_1)) != 0) {
            states.setChannel1(Fw::Logic::HIGH);
        }
        else {
            states.setChannel1(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel2() == true) {
        isCh2ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_2).ioType == Drv::IOExpander::IOType::DigitalIn);
        
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_2)) != 0) {
            states.setChannel2(Fw::Logic::HIGH);
        }
        else {
            states.setChannel2(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel3() == true) {
        isCh3ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_3).ioType == Drv::IOExpander::IOType::DigitalIn);
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_3)) != 0) {
            states.setChannel3(Fw::Logic::HIGH);
        }
        else {
            states.setChannel3(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel4() == true) {
        isCh4ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_4).ioType == Drv::IOExpander::IOType::DigitalIn);
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_4)) != 0) {
            states.setChannel4(Fw::Logic::HIGH);
        }
        else {
            states.setChannel4(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel5() == true) {
        isCh5ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_5).ioType == Drv::IOExpander::IOType::DigitalIn);
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_5)) != 0) {
            states.setChannel5(Fw::Logic::HIGH);
        }
        else {
            states.setChannel5(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel6() == true) {
        isCh6ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_6).ioType == Drv::IOExpander::IOType::DigitalIn);
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_6)) != 0) {
            states.setChannel6(Fw::Logic::HIGH);
        }
        else {
            states.setChannel6(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel7() == true) {
        isCh7ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_7).ioType == Drv::IOExpander::IOType::DigitalIn);
        
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_7)) != 0) {
            states.setChannel7(Fw::Logic::HIGH);
        }
        else {
            states.setChannel7(Fw::Logic::LOW);
        }
    }

    if (ports.getChannel8() == true) {
        isCh0ConfigCorrect = (configTable->at(Drv::IOExpander::Channel::Channel_8).ioType == Drv::IOExpander::IOType::DigitalIn);
        
        if ((regGpiVal & (1U << Drv::IOExpander::Channel::Channel_8)) != 0) {
            states.setChannel8(Fw::Logic::HIGH);
        }
        else {
            states.setChannel8(Fw::Logic::LOW);
        }
    }

    data.setStates(states);

    if ((configTable == nullptr) || 
        (isCh0ConfigCorrect == false) ||
        (isCh1ConfigCorrect == false) ||
        (isCh2ConfigCorrect == false) ||
        (isCh3ConfigCorrect == false) ||
        (isCh4ConfigCorrect == false) ||
        (isCh5ConfigCorrect == false) ||
        (isCh6ConfigCorrect == false) ||
        (isCh7ConfigCorrect == false)
    ) {
        errorReport(ErrorCode::WrongConfiguration);
        return;
    }

    data.setIsValid(true);
  }

  void IOExpander::writeDigitalSingleOutput_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::DigitalSingleOutputData& data, bool& IsValid) {
    IsValid = false;
    if (initializationComplete == false) {
        errorReport(ErrorCode::NotInitialized);
        return;
    }

    auto state = data.getState();
    auto channel = data.getChannel();
    ErrorCode error = setDigitalOutputValue(channel, state);
    if (error != ErrorCode::NoError) {
        errorReport(error);
        return;
    }
    IsValid = true;
  }
  
  void IOExpander::writeDigitalMultipleOutputs_handler(NATIVE_INT_TYPE portNum, const Drv::IOExpander::DigitalMultipleOutputsData& data, bool& IsValid) {
    IsValid = false;
    if (initializationComplete == false) {
        errorReport(ErrorCode::NotInitialized);
        return;
    }

    bool isCh1ConfigCorrect = true;
    bool isCh2ConfigCorrect = true;
    bool isCh3ConfigCorrect = true;
    bool isCh4ConfigCorrect = true;

    bool isCh5ConfigCorrect = true;
    bool isCh6ConfigCorrect = true;
    bool isCh7ConfigCorrect = true;
    bool isCh8ConfigCorrect = true;
    
  
    auto ports = data.getPorts();
    auto states = data.getStates();

    if (ports.getChannel1() == true) {
        isCh1ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_1).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_1).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_1);
        if (states.getChannel1() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_1);
        }
    }

    if (ports.getChannel2() == true) {
        isCh2ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_2).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_2).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_2);
        if (states.getChannel2() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_2);
        }
    }

    if (ports.getChannel3() == true) {
        isCh3ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_3).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_3).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_3);
        if (states.getChannel3() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_3);
        }
    }

    if (ports.getChannel4() == true) {
        isCh4ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_4).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_4).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_4);
        if (states.getChannel4() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_4);
        }
    }

    if (ports.getChannel5() == true) {
        isCh5ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_5).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_5).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_5);
        if (states.getChannel5() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_5);
        }
    }

    if (ports.getChannel6() == true) {
        isCh6ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_6).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_6).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_6);
        if (states.getChannel6() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_6);
        }
    }

    if (ports.getChannel7() == true) {
        isCh7ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_7).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_7).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_7);
        if (states.getChannel7() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_7);
        }
    }

    if (ports.getChannel8() == true) {
        isCh8ConfigCorrect = ((configTable->at(Drv::IOExpander::Channel::Channel_8).ioType == Drv::IOExpander::IOType::DigitalOutOD) || (configTable->at(Drv::IOExpander::Channel::Channel_8).ioType == Drv::IOExpander::IOType::DigitalOutPP));
        digitalOutputsValue = digitalOutputsValue & ~(1U << Drv::IOExpander::Channel::Channel_8);
        if (states.getChannel8() == Fw::Logic::HIGH) {
            digitalOutputsValue |= (1U << Drv::IOExpander::Channel::Channel_8);
        }
    }

    if ((configTable == nullptr) || 
        (isCh1ConfigCorrect == false) ||
        (isCh2ConfigCorrect == false) ||
        (isCh3ConfigCorrect == false) ||
        (isCh4ConfigCorrect == false) ||
        (isCh5ConfigCorrect == false) ||
        (isCh6ConfigCorrect == false) ||
        (isCh7ConfigCorrect == false) ||
        (isCh8ConfigCorrect == false) 
    ) {
        errorReport(ErrorCode::WrongConfiguration);
        return;
    }

    bool writeResult = writeReg(ADS7138_REG::GPO_VALUE, digitalOutputsValue);
    if (writeResult == false) {
        errorReport(ErrorCode::CommError);
        return;
    }
    IsValid = true;
  }

  void IOExpander::calibrateADCOffset_handler(NATIVE_INT_TYPE portNum)
  {
    mutex.lock();
    isOffsetCalibrationInitiated = true;
    mutex.unLock();
  }

  IOExpander::ErrorCode IOExpander::setDigitalOutputValue(const Drv::IOExpander::Channel& channel, Fw::Logic state) {
    GPIOState channelState = (Fw::Logic::HIGH == state) ? GPIOState::GpioSet : GPIOState::GpioReset;
    ErrorCode status = setDigitalOutput(channel, channelState);
    return status;
  }

  IOExpander::ErrorCode IOExpander::setConfig(const ConfigTable *config) {
    configTable = config;

    uint8_t pinCfgVal = 0;
    uint8_t gpioCfgVal = 0;
    uint8_t gpoDriveCfgVal = 0;
    uint8_t autoSeqChVal = 0;

    for (const auto &cfg: *configTable) {
        switch (cfg.ioType) {
            case Drv::IOExpander::IOType::AnalogIn:
                pinCfgVal = pinCfgVal & ~(1U << cfg.channel);                   // PIN_CFG - Reset bit
                gpioCfgVal = gpioCfgVal & ~(1U << cfg.channel);                 // GPIO_CFG - Reset bit
                gpoDriveCfgVal = gpoDriveCfgVal & ~(1U << cfg.channel);         // GPO_DRIVE_CFG - Reset bit

                if (analogMeasurementsMode == AnalogMeasurementsMode::AUTO) {
                    numOfConfiguredAutoAnalogChannels++;
                    autoSeqChVal = autoSeqChVal | (1U << cfg.channel);          // AUTO_SEQ_CH_SEL - Set bit
                }

                break;
            case Drv::IOExpander::IOType::DigitalIn:
                pinCfgVal = pinCfgVal | (1U << cfg.channel);            // PIN_CFG - Set bit
                gpioCfgVal = gpioCfgVal & ~(1U << cfg.channel);         // GPIO_CFG - Reset bit
                gpoDriveCfgVal = gpoDriveCfgVal & ~(1U << cfg.channel); // GPO_DRIVE_CFG - Reset bit
                break;
            case Drv::IOExpander::IOType::DigitalOutOD:
                pinCfgVal = pinCfgVal | (1U << cfg.channel);            // PIN_CFG - Set bit
                gpioCfgVal = gpioCfgVal | (1U << cfg.channel);          // GPIO_CFG - Set bit
                gpoDriveCfgVal = gpoDriveCfgVal & ~(1U << cfg.channel); // GPO_DRIVE_CFG - Reset bit
                break;
            case Drv::IOExpander::IOType::DigitalOutPP:
                pinCfgVal = pinCfgVal | (1U << cfg.channel);            // PIN_CFG - Set bit
                gpioCfgVal = gpioCfgVal | (1U << cfg.channel);          // GPIO_CFG - Set bit
                gpoDriveCfgVal = gpoDriveCfgVal | (1U << cfg.channel);  // GPO_DRIVE_CFG - Set bit
                break;

            default:
                return ErrorCode::WrongConfiguration;
        }
    }

    // set registers:
    if (!writeReg(ADS7138_REG::PIN_CFG, pinCfgVal)) {
        return ErrorCode::CommError;
    }

    if (!writeReg(ADS7138_REG::GPIO_CFG, gpioCfgVal)) {
        return ErrorCode::CommError;
    }

    if (!writeReg(ADS7138_REG::GPO_DRIVE_CFG, gpoDriveCfgVal)) {
        return ErrorCode::CommError;
    }
    
    if (analogMeasurementsMode == AnalogMeasurementsMode::AUTO) {
        if (!writeReg(ADS7138_REG::AUTO_SEQ_CH_SEL, autoSeqChVal)) {
            return ErrorCode::CommError;
        }
    }

    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::setOversamplingRatio(Components::IOExpander::OversamplingRatio oversamplingRatio) {
    U8 osrCfgVal = 0;
    isOversamplingEnabled = true;
    maxADCCode = maxADCCodeWithOversampling;

    switch (oversamplingRatio) {
        case Components::IOExpander::OversamplingRatio::NO_AVERAGING: {
            isOversamplingEnabled = false;
            osrCfgVal = 0b0;
            maxADCCode = maxADCCodeDefault;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES2: {
            osrCfgVal = 0b1;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES4: {
            osrCfgVal = 0b10;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES8: {
            osrCfgVal = 0b11;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES16: {
            osrCfgVal = 0b100;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES32: {
            osrCfgVal = 0b101;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES64: {
            osrCfgVal = 0b110;
            break;
        }
        
        case Components::IOExpander::OversamplingRatio::SAMPLES128: {
            osrCfgVal = 0b111;
            break;
        }

        default: 
            isOversamplingEnabled = false;
        break;
    }
    
    // selects the oversampling ratio for ADC conversion result
    if (!writeReg(ADS7138_REG::OSR_CFG, osrCfgVal)) {
        return ErrorCode::CommError;
    }

    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::configAutoSeqMode() {
    static constexpr U8 autoSeqMode = 1;
    if (!writeReg(ADS7138_REG::SEQUENCE_CFG, (autoSeqMode & ADS7138_SEQ_CFG_REG::SEQ_MODE))) {
        return ErrorCode::CommError;
    }
    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::configManualSeqMode() {
    static constexpr U8 manualSeqMode = 0;
    if (!writeReg(ADS7138_REG::SEQUENCE_CFG, (manualSeqMode & ADS7138_SEQ_CFG_REG::SEQ_MODE))) {
        return ErrorCode::CommError;
    }
    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::appendChannelID() {
    if (!writeReg(ADS7138_REG::DATA_CFG, ADS7138_DATA_CFG_REG::APPEND_STATUS)) {
        return ErrorCode::CommError;
    }
    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::seqStart() {
    U8 seqCfg;
    if (!readReg(ADS7138_REG::SEQUENCE_CFG, &seqCfg)) {
        return ErrorCode::CommError;
    }
    
    seqCfg &= (~ADS7138_SEQ_CFG_REG::SEQ_START);
    seqCfg |= ADS7138_SEQ_CFG_REG::SEQ_START;

    if (!writeReg(ADS7138_REG::SEQUENCE_CFG, seqCfg)) {
        return ErrorCode::CommError;
    }
    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::enableStats() {
    U8 generalCfg;
    if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
        return ErrorCode::CommError;
    }

    generalCfg &= (~ADS7138_GENERAL_CFG_REG::STATS_EN);
    generalCfg |= ADS7138_GENERAL_CFG_REG::STATS_EN;
    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return ErrorCode::CommError;
    }

    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::disableStats() {
    U8 generalCfg;
    if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
        return ErrorCode::CommError;
    }

    generalCfg &= (~ADS7138_GENERAL_CFG_REG::STATS_EN);
    generalCfg |= ADS7138_GENERAL_CFG_REG::STATS_EN;
    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return ErrorCode::CommError;
    }

    return ErrorCode::NoError;
  }

  IOExpander::ErrorCode IOExpander::startConversion() {
    U8 generalCfg;
    if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
        return ErrorCode::CommError;
    }

    generalCfg &= (~ADS7138_GENERAL_CFG_REG::CNVST);
    generalCfg |= ADS7138_GENERAL_CFG_REG::CNVST;
    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return ErrorCode::CommError;
    }

    return ErrorCode::NoError;
  }

  etl::expected<bool, IOExpander::ErrorCode> IOExpander::isAveragingResultReady() {
    U8 systemStatus;
    if (!readReg(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
        return etl::unexpected(ErrorCode::CommError);
    }

    bool isResultReady = ((systemStatus & static_cast<U8>(ADS7138_SYSTEM_STATUS_REG::OSR_DONE)) == static_cast<U8>(ADS7138_SYSTEM_STATUS_REG::OSR_DONE));
    return isResultReady;
  }

  etl::expected<uint16_t, IOExpander::ErrorCode> IOExpander::getAnalogValue(Drv::IOExpander::Channel channel) {
      if (!initializationComplete) {
          return etl::unexpected(ErrorCode::NotInitialized);
      }
      if ((configTable == nullptr) || (channel >= Drv::IOExpander::Channel::ChannelAmount) || (configTable->at(channel).ioType != Drv::IOExpander::IOType::AnalogIn)) {
          return etl::unexpected(ErrorCode::WrongConfiguration);
      }

      if (!writeReg(ADS7138_REG::CHANNEL_SEL, channel)) {
        return etl::unexpected(ErrorCode::CommError);
      }
    
      uint16_t ainVal = 0;
      Drv::IOExpander::Channel checkChannel;
      auto status = readADCData(isCRCEnabled, isOversamplingEnabled, &ainVal, checkChannel);
      if (status != ErrorCode::NoError) {
        return etl::unexpected(status);
      }
      if (channel != checkChannel) {
         return etl::unexpected(ErrorCode::CommError);
      }

      return ainVal;
  }

  IOExpander::ErrorCode IOExpander::pollAutoAnalogIn() {
    if (!initializationComplete) {
        return ErrorCode::NotInitialized;
    }
    if ((configTable == nullptr) || (numOfConfiguredAutoAnalogChannels == 0) || (numOfConfiguredAutoAnalogChannels > Drv::IOExpander::Channel::ChannelAmount)) {
        return ErrorCode::WrongConfiguration;
    }
    for (U8 i = 0 ; i < Drv::IOExpander::Channel::ChannelAmount; i++) {
        Drv::IOExpander::Channel channel;
        U16 ainVal = 0;
        auto status = readADCData(isCRCEnabled, isOversamplingEnabled, &ainVal, channel);
        if (status != ErrorCode::NoError) {
            return status;
        }

        F32 voltageOffset = static_cast<F32>(analogOffsetsMiliVolts.at(channel)) / voltInMiliVoltCoeff;
        auto autoVoltageValue = static_cast<F32>(ainVal) * ADC_VREF / maxADCCode + voltageOffset;

        mutex.lock();
        autoAdcCode.at(channel) = ainVal;
        autoVoltageValues.at(channel) = autoVoltageValue;
        if (thresholdsAreSet) {
            auto miliVoltageValue         = static_cast<U32>(autoVoltageValue*voltInMiliVoltCoeff);
            auto previousMiliVoltageValueForMinEstimation = static_cast<U32>(previousAutoVoltageValueForMinValueEstimation.at(channel)*voltInMiliVoltCoeff);
            auto previousMiliVoltageValueForMaxEstimation = static_cast<U32>(previousAutoVoltageValueForMaxValueEstimation.at(channel)*voltInMiliVoltCoeff);

            
            if ((miliVoltageValue >= maxThresholdsMiliVolts.at(channel)) && (previousMiliVoltageValueForMaxEstimation < maxThresholdsMiliVolts.at(channel))) {
                maxThresholdsHitCounter.at(channel) = maxThresholdsHitCounter.at(channel) + 1;
            }
            if ((miliVoltageValue <= minThresholdsMiliVolts.at(channel)) && (previousMiliVoltageValueForMinEstimation > minThresholdsMiliVolts.at(channel))) {
                minThresholdsHitCounter.at(channel) = minThresholdsHitCounter.at(channel) + 1;
            }
            
            previousAutoVoltageValueForMinValueEstimation.at(channel) = autoVoltageValue;
            previousAutoVoltageValueForMaxValueEstimation.at(channel) = autoVoltageValue;
        }
        mutex.unLock();
    }

    return ErrorCode::NoError;
  }

  etl::expected<IOExpander::GPIOState, IOExpander::ErrorCode>
  IOExpander::getDigitalInput(Drv::IOExpander::Channel channel) {
      if (!initializationComplete) {
          return etl::unexpected(ErrorCode::NotInitialized);
      }
      if ((configTable == nullptr) || (channel >= Drv::IOExpander::Channel::ChannelAmount) || (configTable->at(channel).ioType != Drv::IOExpander::IOType::DigitalIn)) {
        return etl::unexpected(ErrorCode::WrongConfiguration);
      }

      uint8_t regGpiVal = 0;
      bool readResult = readReg(ADS7138_REG::GPI_VALUE, &regGpiVal);

      if (readResult == true) {
        return (((1U << channel) & regGpiVal) == 0) ? GPIOState::GpioReset : GPIOState::GpioSet;
      }

      return etl::unexpected(ErrorCode::CommError);
  }

  IOExpander::ErrorCode IOExpander::setDigitalOutput(Drv::IOExpander::Channel channel, IOExpander::GPIOState state) {
      if (!initializationComplete) {
          return ErrorCode::NotInitialized;
      }
      if ((configTable == nullptr) || (channel >= Drv::IOExpander::Channel::ChannelAmount) ||
         ((configTable->at(channel).ioType != Drv::IOExpander::IOType::DigitalOutOD) && (configTable->at(channel).ioType != Drv::IOExpander::IOType::DigitalOutPP))) {
          return ErrorCode::WrongConfiguration;
      }

        digitalOutputsValue = digitalOutputsValue & ~(1U << channel);
        if (state == GPIOState::GpioSet) {
            digitalOutputsValue |= (1U << channel);
        }
        
        bool writeResult = writeReg(ADS7138_REG::GPO_VALUE, digitalOutputsValue);
        if (writeResult == false) {
          return ErrorCode::CommError;
        }
        return ErrorCode::NoError;
  }

  bool IOExpander:: writeReg(uint8_t reg, uint8_t regVal) {
    if (isCRCEnabled) {
        bool isCRCError = checkCRCError();
        if (isCRCError) {
          bool isCrcErrorCleared = clearCrcErrorBit();
          if (isCrcErrorCleared == false) {
              return ErrorCode::CommError;
          }
        }

        bool isWriteSuccess = writeRegWithCRC(reg, regVal);

        return isWriteSuccess;
    }
    else {
        bool isWriteSuccess = writeRegWithoutCRC(reg, regVal);
        return isWriteSuccess;
    }
    return false;
  }

  bool IOExpander::readReg(uint8_t reg, uint8_t *regVal) {
    if (isCRCEnabled) {
        bool isReadSuccess = readRegWithCRC(reg, regVal);

        return isReadSuccess;
    }
    else {
        bool isReadSuccess = readRegWithoutCRC(reg, regVal);
        return isReadSuccess;
    }
    return false;
  }

  bool IOExpander:: writeRegWithoutCRC(uint8_t reg, uint8_t regVal) {
    etl::array<uint8_t, 3> arr{ADS7138_OPCODE::WRITE, reg, regVal};
    Fw::Buffer writeBuffer(arr.data(), arr.size());
    return (write_out(0, devAddr, writeBuffer, writeTimeout) == Drv::I2C::Status::SUCCESS);
  }

  bool IOExpander::readRegWithoutCRC(uint8_t reg, uint8_t *regVal) {
    etl::array<uint8_t, 2> writeArr {ADS7138_OPCODE::READ, reg};
    Fw::Buffer writeBuffer(writeArr.data(), writeArr.size());

    etl::span<uint8_t> const readSpan(regVal, 1);
    Fw::Buffer readBuffer(readSpan.data(), readSpan.size());

    bool result = writeRead_out(0, devAddr, writeBuffer, readBuffer, readTimeout) == Drv::I2C::Status::SUCCESS;

    return result;
  }

   bool IOExpander::writeRegWithCRC(uint8_t reg, uint8_t regVal) {
    U8 opcodeCRC    = calculateCRC8(static_cast<U8>(ADS7138_OPCODE::WRITE));
    U8 regCRC       = calculateCRC8(static_cast<U8>(reg));
    U8 regValCRC    = calculateCRC8(static_cast<U8>(regVal));

    etl::array<uint8_t, 6> arr{ADS7138_OPCODE::WRITE, opcodeCRC, reg, regCRC, regVal, regValCRC};
    Fw::Buffer writeBuffer(arr.data(), arr.size());
    
    auto writeStatus = write_out(0, devAddr, writeBuffer, writeTimeout);

    return (writeStatus == Drv::I2C::Status::SUCCESS);
   }

   bool IOExpander::readRegWithCRC(uint8_t reg, uint8_t *regVal) {
    U8 opcodeCRC = calculateCRC8(static_cast<U8>(ADS7138_OPCODE::READ));
    U8 regCRC = calculateCRC8(reg);

    etl::array<uint8_t, 4> writeArr {ADS7138_OPCODE::READ, opcodeCRC, reg, regCRC};
    Fw::Buffer writeBuffer(writeArr.data(), writeArr.size());

    etl::array<U8, 2> readArr;
    etl::span<uint8_t> const readSpan(readArr.data(), readArr.size());
    Fw::Buffer readBuffer(readSpan.data(), readSpan.size());
    bool result = (writeRead_out(0, devAddr, writeBuffer, readBuffer, readTimeout) == Drv::I2C::Status::SUCCESS);
    
    auto data = readSpan.front();
    auto receivedCRC = *(readSpan.data() + 1);
    U8 calculateCRC = calculateCRC8(data);
    
    if (calculateCRC != receivedCRC) {
        result = false;
    }
    *regVal = data;

    return result;
   }

   IOExpander::ErrorCode IOExpander::readADCData(bool isCRCEnabled, bool isOversamplingEnabled, U16 *data, Drv::IOExpander::Channel& channel) {
    if (isCRCEnabled) {
        if (isOversamplingEnabled) {
            etl::array<uint8_t, 6> ainVal{};
            Fw::Buffer ainBuffer(ainVal.data(), ainVal.size());
            if (read_out(0, devAddr, ainBuffer, readTimeout) == Drv::I2C::Status::SUCCESS) {
              auto receivedCRC = ainVal[1];
              U8 calculateCRC = calculateCRC8(ainVal[0]);
              if (calculateCRC != receivedCRC) {
                  return ErrorCode::CrcError;
              }
          
              receivedCRC = ainVal[3];
              calculateCRC = calculateCRC8(ainVal[2]);
              if (calculateCRC != receivedCRC) {
                  return ErrorCode::CrcError;
              }

              receivedCRC = ainVal[5];
              calculateCRC = calculateCRC8(ainVal[4]);
              if (calculateCRC != receivedCRC) {
                  return ErrorCode::CrcError;
              }
          
              constexpr U8 channelIdMask = 0b1111'0000;
              constexpr U8 channelIdShift = 4U;
              U8 channelId = (ainVal[4] & channelIdMask) >> 4U;

              auto receivedChannel = getChannelBasedOn(channelId);
              if (receivedChannel.has_value() == false) {
                auto error = receivedChannel.error();
                errorReport(error);
                return error;
              }
              
              channel = receivedChannel.value();

              *data = static_cast<U16>((static_cast<U16>(ainVal[0]) << 8U) + (ainVal[2]));
            }
        }
        else {
            etl::array<uint8_t, 4> ainVal{};
            Fw::Buffer ainBuffer(ainVal.data(), ainVal.size());
            if (read_out(0, devAddr, ainBuffer, readTimeout) == Drv::I2C::Status::SUCCESS) {
              auto receivedCRC = ainVal[1];
              U8 calculateCRC = calculateCRC8(ainVal[0]);
              if (calculateCRC != receivedCRC) {
                  return ErrorCode::CrcError;;
              }
          
              receivedCRC = ainVal[3];
              calculateCRC = calculateCRC8(ainVal[2]);
              if (calculateCRC != receivedCRC) {
                  return ErrorCode::CrcError;;
              }
          
              constexpr U8 channelIdMask = 0b1111;
              U8 channelId = ainVal[2] & channelIdMask;

              auto receivedChannel = getChannelBasedOn(channelId);
              if (receivedChannel.has_value() == false) {
                auto error = receivedChannel.error();
                errorReport(error);
                return error;
              }
              channel = receivedChannel.value();
              
              *data = static_cast<U16>((static_cast<U16>(ainVal[0]) << 4U) + (ainVal[2] >> 4U));
            }
        }
        
      }
      else {
        if (isOversamplingEnabled) {
            etl::array<uint8_t, 3> ainVal{};
            Fw::Buffer ainBuffer(ainVal.data(), ainVal.size());
            if (read_out(0, devAddr, ainBuffer, readTimeout) == Drv::I2C::Status::SUCCESS) {

              constexpr U8 channelIdMask = 0b1111'0000;
              constexpr U8 channelIdShift = 4U;
              U8 channelId = (ainVal[2] & channelIdMask) >> 4U;

              auto receivedChannel = getChannelBasedOn(channelId);
              if (receivedChannel.has_value() == false) {
                auto error = receivedChannel.error(); 
                errorReport(error);
                return error;
              }
              channel = receivedChannel.value();
          
              *data = static_cast<U16>((static_cast<U16>(ainVal[0]) << 8U) + (ainVal[1]));
            }
        }
        else {
            etl::array<uint8_t, 2> ainVal{};
            Fw::Buffer ainBuffer(ainVal.data(), ainVal.size());
            if (read_out(0, devAddr, ainBuffer, readTimeout) != Drv::I2C::Status::SUCCESS) {
              return ErrorCode::CommError;
            }
          
            constexpr U8 channelIdMask = 0b1111;
            U8 channelId = ainVal[1] & channelIdMask;
            
            auto receivedChannel = getChannelBasedOn(channelId);
            if (receivedChannel.has_value() == false) {
              auto error = receivedChannel.error();
              errorReport(error);
              return error;
            }
            channel = receivedChannel.value();

            *data = static_cast<U16>((static_cast<U16>(ainVal[0]) << 4U) + (ainVal[1] >> 4U));
        }
        
      }
      return ErrorCode::NoError;
  }

  void IOExpander::errorReport(ErrorCode errorCode) {
    switch (errorCode) {
        case ErrorCode::NoError:
        break;
        case ErrorCode::NotInitialized:
            this->log_WARNING_HI_NotInited();
        break;
        case ErrorCode::WrongConfiguration:
            this->log_WARNING_HI_WrongChannelConfiguration();
        break;
        case ErrorCode::CommError:
            this->log_WARNING_HI_CommunicationError();
        break;
        case ErrorCode::CrcError:
            this->log_WARNING_HI_CRCError();
        break;
        case ErrorCode::CalibrationUnfinished:
            this->log_WARNING_HI_CalibrationUnfinishedError();
        break;
        default:
            this->log_WARNING_HI_UnknownError();
        break;
    }
  }

  etl::expected<Drv::IOExpander::Channel, IOExpander::ErrorCode> IOExpander::getChannelBasedOn(U8 id) {
    if (id >= Drv::IOExpander::Channel::ChannelAmount) {
        return etl::unexpected(ErrorCode::CommError);
    }

    Drv::IOExpander::Channel channel;

    switch(id) {
        case Drv::IOExpander::Channel::Channel_1:
            channel = Drv::IOExpander::Channel::Channel_1;
        break;
        case Drv::IOExpander::Channel::Channel_2:
            channel = Drv::IOExpander::Channel::Channel_2;
        break;
        case Drv::IOExpander::Channel::Channel_3:
            channel = Drv::IOExpander::Channel::Channel_3;
        break;
        case Drv::IOExpander::Channel::Channel_4:
            channel = Drv::IOExpander::Channel::Channel_4;
        break;
        case Drv::IOExpander::Channel::Channel_5:
            channel = Drv::IOExpander::Channel::Channel_5;
        break;
        case Drv::IOExpander::Channel::Channel_6:
            channel = Drv::IOExpander::Channel::Channel_6;
        break;
        case Drv::IOExpander::Channel::Channel_7:
            channel = Drv::IOExpander::Channel::Channel_7;
        break;
        case Drv::IOExpander::Channel::Channel_8:
            channel = Drv::IOExpander::Channel::Channel_8;
        break;
        default:
        break;
    }

    return channel;
  }

  uint8_t calculateCRC(const uint8_t dataBytes[], uint8_t numberBytes, uint8_t initialValue) {
      int bitIndex, byteIndex;
      bool dataMSb; 
      bool crcMSb;

      uint8_t crc = initialValue;
      const uint8_t poly = 0x07;

      for (byteIndex = 0; byteIndex < numberBytes; byteIndex++)
      {
          bitIndex = 0x80u;
          while (bitIndex > 0)
          {
              dataMSb = (bool) (dataBytes[byteIndex] & bitIndex);
              crcMSb = (bool) (crc & 0x80u);
              crc <<= 1;
              if (dataMSb ^ crcMSb)
              {
                  crc ^= poly;
              }
              bitIndex >>= 1;
          }
      }
      return crc;
  }

  U8 IOExpander::calculateCRC8(U8 byte) {
    constexpr U8 numOfBytes = 1;
    constexpr U8 initialSeed = 0;
    U8 crc = calculateCRC(&byte, numOfBytes, initialSeed);
    return crc;
  }

  bool IOExpander::enableCRCAlert() {
    if (clearCrcErrorBit() == false) {
        return false;
    }

    uint8_t alertMap = 0; 
    if (!readReg(ADS7138_REG::ALERT_MAP, &alertMap)) {
        return false;
    }
    alertMap &= (~ADS7138_ALERT_MAP_REG::ALERT_CRCIN);
    alertMap |= ADS7138_ALERT_MAP_REG::ALERT_CRCIN;

    if (!writeReg(ADS7138_REG::ALERT_MAP, alertMap)) {
        return false;
    }

    if (!readReg(ADS7138_REG::ALERT_MAP, &alertMap)) {
        return false;
    }

    bool isBitSet = ((alertMap & static_cast<U8>(ADS7138_ALERT_MAP_REG::ALERT_CRCIN)) == static_cast<U8>(ADS7138_ALERT_MAP_REG::ALERT_CRCIN));
    if (isBitSet == false) {
        return false;
    }

    return true;
  }

  bool IOExpander::configAlertPin() {
    uint8_t alertPinCfg = 0; 
    if (!readReg(ADS7138_REG::ALERT_PIN_CFG, &alertPinCfg)) {
        return false;
    }
    alertPinCfg &= (~ADS7138_ALERT_PIN_CFG_REG::ALERT_LOGIC);
    U8 activeHigh = 0b1;
    alertPinCfg |= activeHigh;

    if (!writeReg(ADS7138_REG::ALERT_PIN_CFG, alertPinCfg)) {
        return false;
    }

    if (!readReg(ADS7138_REG::ALERT_PIN_CFG, &alertPinCfg)) {
        return false;
    }

    bool isBitSet = ((alertPinCfg & static_cast<U8>(ADS7138_ALERT_PIN_CFG_REG::ALERT_LOGIC)) == activeHigh);
    if (isBitSet == false) {
        return false;
    }

    if (!readReg(ADS7138_REG::ALERT_PIN_CFG, &alertPinCfg)) {
        return false;
    }
    alertPinCfg &= (~ADS7138_ALERT_PIN_CFG_REG::ALERT_DRIVE);
    U8 pushPullOutput = 0b100;
    alertPinCfg |= pushPullOutput;

    if (!writeReg(ADS7138_REG::ALERT_PIN_CFG, alertPinCfg)) {
        return false;
    }

    if (!readReg(ADS7138_REG::ALERT_PIN_CFG, &alertPinCfg)) {
        return false;
    }

    isBitSet = ((alertPinCfg & static_cast<U8>(ADS7138_ALERT_PIN_CFG_REG::ALERT_DRIVE)) == pushPullOutput);
    if (isBitSet == false) {
        return false;
    }

    return true;
  }

  bool IOExpander::checkCRCEnabled() {
    uint8_t systemStatus = 0; 
    bool readWithCRCSuccess = true;
    if (readRegWithCRC(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
        if ((systemStatus & ADS7138_SYSTEM_STATUS_REG::DEFAULT) == ADS7138_SYSTEM_STATUS_REG::DEFAULT) {
            isCRCEnabled = true;    
        }
        else {
            if (readRegWithoutCRC(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
                if ((systemStatus & ADS7138_SYSTEM_STATUS_REG::DEFAULT) == ADS7138_SYSTEM_STATUS_REG::DEFAULT) {
                    isCRCEnabled = false;
                }
            }
            else {
                return false;
            }
        }
    }
    else {
        if (readRegWithoutCRC(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
            if ((systemStatus & ADS7138_SYSTEM_STATUS_REG::DEFAULT) == ADS7138_SYSTEM_STATUS_REG::DEFAULT) {
                isCRCEnabled = false;
            }
        }
        else {
            return false;
        }
    }
    
    return true;
  }
  
  bool IOExpander::checkPowerUpConfig() {
    U8 systemStatus;
    if (!readReg(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
        return false;
    }
    if ((systemStatus & ADS7138_SYSTEM_STATUS_REG::CRC_ERR_FUSE) == ADS7138_SYSTEM_STATUS_REG::CRC_ERR_FUSE) {
        return false;
    }
    
    return true;
  }

  bool IOExpander::softwareReset() {
    uint8_t generalCfg = ADS7138_GENERAL_CFG_REG::RST;

    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return false;
    }

    return true;
  }

  bool IOExpander::crcEnable() {
    uint8_t generalCfg = 0; 

    generalCfg &= (~ADS7138_GENERAL_CFG_REG::CRC_EN);
    generalCfg |= ADS7138_GENERAL_CFG_REG::CRC_EN;

    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return false;
    }

    isCRCEnabled = true;

    if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
        return false;
    }

    bool isBitSet = ((generalCfg & static_cast<U8>(ADS7138_GENERAL_CFG_REG::CRC_EN)) == static_cast<U8>(ADS7138_GENERAL_CFG_REG::CRC_EN));
    if (isBitSet == false) {
        return false;
    }

    return true;
  }

  bool IOExpander::checkCRCError() {
    // read alert pin
    Fw::Logic state;
    alert_out(0, state);
    
    // if pin is activated clear error bit in status register
    bool isWriteCRCError = (state == Fw::Logic::HIGH);
    
    return isWriteCRCError;
  }

  bool IOExpander::clearCrcErrorBit() {
    if (isCRCEnabled) {
        U8 clearCrcIn = ADS7138_SYSTEM_STATUS_REG::CRC_ERR_IN;
        if (!writeRegWithCRC(ADS7138_REG::SYSTEM_STATUS, clearCrcIn)) {
            return false;
        }
        
        U8 systemStatus;
        if (!readRegWithCRC(ADS7138_REG::SYSTEM_STATUS, &systemStatus)) {
            return false;
        }
    
        bool isBitCleared = ((systemStatus & ADS7138_SYSTEM_STATUS_REG::CRC_ERR_IN) == 0);
        if (isBitCleared == false) {
            return false;
        }
    
        return true;
    }
    return false;
  }

  IOExpander::ErrorCode IOExpander::calibrateADCOffset() {
    uint8_t generalCfg = 0; 

    if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
        return ErrorCode::CommError;
    }

    generalCfg &= (~ADS7138_GENERAL_CFG_REG::CAL);
    generalCfg |= ADS7138_GENERAL_CFG_REG::CAL;

    if (!writeReg(ADS7138_REG::GENERAL_CFG, generalCfg)) {
        return ErrorCode::CommError;
    }

    bool isCalibrationFinished = false;
    for (U8 i = 0; i < maxNumOfCalibResultReadAttempts; i++) {
        if (!readReg(ADS7138_REG::GENERAL_CFG, &generalCfg)) {
            return ErrorCode::CommError;
        }
        bool isBitReset = ((generalCfg & static_cast<U8>(ADS7138_GENERAL_CFG_REG::CAL)) == 0);
        if (isBitReset == true) {
            return ErrorCode::NoError;
        }
    }
    
    return ErrorCode::CalibrationUnfinished;
  }

}
