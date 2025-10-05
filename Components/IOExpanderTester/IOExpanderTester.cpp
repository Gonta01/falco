// ======================================================================
// \title  IOExpanderTester.cpp
// \author user
// \brief  cpp file for IOExpanderTester component implementation class
// ======================================================================

#include "Components/IOExpanderTester/IOExpanderTester.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  IOExpanderTester ::
    IOExpanderTester(const char* const compName) :
      IOExpanderTesterComponentBase(compName)
  {

  }

  IOExpanderTester ::
    ~IOExpanderTester()
  {

  }

  void IOExpanderTester::updateDigitalInputTelemetry(IO expander, const Drv::IOExpander::Channel& channel, Fw::Logic& state) {
    if (expander == IO::Expander1) {
      switch (channel) {
        case Drv::IOExpander::Channel::Channel_1:
            this->tlmWrite_IOExpander1_DigitalInput1(state);
        break;
        case Drv::IOExpander::Channel::Channel_2:
            this->tlmWrite_IOExpander1_DigitalInput2(state);
        break;
        case Drv::IOExpander::Channel::Channel_3:
            this->tlmWrite_IOExpander1_DigitalInput3(state);
        break;
        case Drv::IOExpander::Channel::Channel_4:
            this->tlmWrite_IOExpander1_DigitalInput4(state);
        break;
        case Drv::IOExpander::Channel::Channel_5:
            this->tlmWrite_IOExpander1_DigitalInput5(state);
        break;
        case Drv::IOExpander::Channel::Channel_6:
            this->tlmWrite_IOExpander1_DigitalInput6(state);
        break;
        case Drv::IOExpander::Channel::Channel_7:
            this->tlmWrite_IOExpander1_DigitalInput7(state);
        break;
        case Drv::IOExpander::Channel::Channel_8:
            this->tlmWrite_IOExpander1_DigitalInput8(state);
        break;

        default:
        break;
      }
    }
    else if (expander == IO::Expander2) {
      switch (channel) {
        case Drv::IOExpander::Channel::Channel_1:
            this->tlmWrite_IOExpander2_DigitalInput1(state);
        break;
        case Drv::IOExpander::Channel::Channel_2:
            this->tlmWrite_IOExpander2_DigitalInput2(state);
        break;
        case Drv::IOExpander::Channel::Channel_3:
            this->tlmWrite_IOExpander2_DigitalInput3(state);
        break;
        case Drv::IOExpander::Channel::Channel_4:
            this->tlmWrite_IOExpander2_DigitalInput4(state);
        break;
        case Drv::IOExpander::Channel::Channel_5:
            this->tlmWrite_IOExpander2_DigitalInput5(state);
        break;
        case Drv::IOExpander::Channel::Channel_6:
            this->tlmWrite_IOExpander2_DigitalInput6(state);
        break;
        case Drv::IOExpander::Channel::Channel_7:
            this->tlmWrite_IOExpander2_DigitalInput7(state);
        break;
        case Drv::IOExpander::Channel::Channel_8:
            this->tlmWrite_IOExpander2_DigitalInput8(state);
        break;
        default:
        break;
      }
    }
  }

  void IOExpanderTester::updateAnalogInputTelemetry(IO expander, const Drv::IOExpander::Channel& channel, U16 value, F32 voltage) {
    if (expander == IO::Expander1) {
      switch (channel) {
        case Drv::IOExpander::Channel::Channel_1:
            this->tlmWrite_IOExpander1_AICode1(value);
            this->tlmWrite_IOExpander1_AIVoltage1(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_2:
            this->tlmWrite_IOExpander1_AICode2(value);
            this->tlmWrite_IOExpander1_AIVoltage2(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_3:
            this->tlmWrite_IOExpander1_AICode3(value);
            this->tlmWrite_IOExpander1_AIVoltage3(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_4:
            this->tlmWrite_IOExpander1_AICode4(value);
            this->tlmWrite_IOExpander1_AIVoltage4(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_5:
            this->tlmWrite_IOExpander1_AICode5(value);
            this->tlmWrite_IOExpander1_AIVoltage5(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_6:
            this->tlmWrite_IOExpander1_AICode6(value);
            this->tlmWrite_IOExpander1_AIVoltage6(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_7:
            this->tlmWrite_IOExpander1_AICode7(value);
            this->tlmWrite_IOExpander1_AIVoltage7(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_8:
            this->tlmWrite_IOExpander1_AICode8(value);
            this->tlmWrite_IOExpander1_AIVoltage8(voltage);
        break;
        default:
        break;
      }
    }
    else if (expander == IO::Expander2) {
      switch (channel) {
        case Drv::IOExpander::Channel::Channel_1:
            this->tlmWrite_IOExpander2_AICode1(value);
            this->tlmWrite_IOExpander2_AIVoltage1(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_2:
            this->tlmWrite_IOExpander2_AICode2(value);
            this->tlmWrite_IOExpander2_AIVoltage2(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_3:
            this->tlmWrite_IOExpander2_AICode3(value);
            this->tlmWrite_IOExpander2_AIVoltage3(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_4:
            this->tlmWrite_IOExpander2_AICode4(value);
            this->tlmWrite_IOExpander2_AIVoltage4(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_5:
            this->tlmWrite_IOExpander2_AICode5(value);
            this->tlmWrite_IOExpander2_AIVoltage5(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_6:
            this->tlmWrite_IOExpander2_AICode6(value);
            this->tlmWrite_IOExpander2_AIVoltage6(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_7:
            this->tlmWrite_IOExpander2_AICode7(value);
            this->tlmWrite_IOExpander2_AIVoltage7(voltage);
        break;
        case Drv::IOExpander::Channel::Channel_8:
            this->tlmWrite_IOExpander2_AICode8(value);
            this->tlmWrite_IOExpander2_AIVoltage8(voltage);
        break;
        default:
        break;
    }
    }
    
  }

  void IOExpanderTester::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid;
    Fw::Logic state = Fw::Logic::LOW;
    Drv::IOExpander::DigitalSingleOutputData data;
    bool quality;

    switch (id) {
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT1:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT1(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_1);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT2:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT2(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_2);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT3:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT3(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_3);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT4:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT4(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_4);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT5:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT5(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_5);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT6:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT6(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_6);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT7:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT7(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_7);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER1_DIGITALOUTPUT8:
        state = this->paramGet_IOEXPANDER1_DIGITALOUTPUT8(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_8);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;  
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT1:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT1(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_1);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT2:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT2(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_2);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT3:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT3(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_3);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT4:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT4(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_4);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT5:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT5(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_5);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT6:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT6(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_6);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT7:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT7(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_7);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;
      case PARAMID_IOEXPANDER2_DIGITALOUTPUT8:
        state = this->paramGet_IOEXPANDER2_DIGITALOUTPUT8(isValid);
        FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
        data.setChannel(Drv::IOExpander::Channel::Channel_8);
        data.setState(state);
        this->writeDigitalSingleOutput_out(0, data, quality);
        break;    
      default:
        break;  
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void IOExpanderTester::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    if (thresholdsAreSet) {
      Drv::IOExpander::NumOfThresholdHitData numOfThresholdHitData;
      
      this->getNumOfThresholdHit1_out(0, Drv::IOExpander::Channel::Channel_3, numOfThresholdHitData);
      
      this->tlmWrite_IOExpander1_AI3_maxThshldCnt(numOfThresholdHitData.getThresholdsHitCounter().getMax());
      this->tlmWrite_IOExpander1_AI3_minThshldCnt(numOfThresholdHitData.getThresholdsHitCounter().getMin());
    }
      switch(testerMode) {
        case Tester::Mode0:
          Mode0();
        break;

        case Tester::Mode1:
          Mode1();
        break;

        case Tester::Mode2:
          Mode2();
        break;

        case Tester::Mode3:
          Mode3();
        break;

        default:
        break;
      }
  }

  bool IOExpanderTester::digitalOutputSetPattern(Pattern &pattern) { 
    bool qualityBit;
    Drv::IOExpander::DigitalSingleOutputData digitalSingleOutputData;

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_1);
    digitalSingleOutputData.setState(pattern.Ch1);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_2);
    digitalSingleOutputData.setState(pattern.Ch2);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_3);
    digitalSingleOutputData.setState(pattern.Ch3);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_4);
    digitalSingleOutputData.setState(pattern.Ch4);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_5);
    digitalSingleOutputData.setState(pattern.Ch5);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_6);
    digitalSingleOutputData.setState(pattern.Ch6);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_7);
    digitalSingleOutputData.setState(pattern.Ch7);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    digitalSingleOutputData.setChannel(Drv::IOExpander::Channel::Channel_8);
    digitalSingleOutputData.setState(pattern.Ch8);
    this->writeDigitalSingleOutput_out(0, digitalSingleOutputData, qualityBit);
    if (qualityBit == false) {
      return false;
    }

    return true;
  }

  bool IOExpanderTester::digitalInputReceivePattern(IO expander, Pattern &pattern) {
    Fw::Logic state = Fw::Logic::LOW;
    Drv::IOExpander::DigitalSingleInputData data;

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_1, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch1 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_1, state);
    }
    else {
      return false;
    }
    
    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_2, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch2 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_2, state);
    }
    else {
      return false;
    }
    
    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_3, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch3 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_3, state);
    }
    else {
      return false;
    }

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_4, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch4 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_4, state);
    }
    else {
      return false;
    }

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_5, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch5 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_5, state);
    }
    else {
      return false;
    }

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_6, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch6 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_6, state);
    }
    else {
      return false;
    }

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_7, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch7 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_7, state);
    }
    else {
      return false;
    }

    this->readDigitalSingleInput_out(0, Drv::IOExpander::Channel::Channel_8, data);
    if (data.getIsValid()) {
      state = data.getState();
      pattern.Ch8 = state;
      updateDigitalInputTelemetry(expander, Drv::IOExpander::Channel::Channel_8, state);
    }
    else {
      return false;
    }

    return true;
  }

  bool IOExpanderTester::analogInputUpdateValues(IO expander) {
    U16 adcCode = 0;
    F32 voltageValue = 0.0;

    Drv::IOExpander::ReadAnaloglInputData data;
    if (expander == IO::Expander1) {
      
      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_1, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_1, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_2, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_2, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_3, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_3, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_4, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_4, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_5, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_5, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_6, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_6, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_7, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_7, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput1_out(0, Drv::IOExpander::Channel::Channel_8, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_8, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }
    }

    if (expander == IO::Expander2) {
      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_1, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_1, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_2, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_2, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_3, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_3, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_4, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_4, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_5, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_5, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_6, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_6, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_7, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_7, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }

      this->readAnalogInput2_out(0, Drv::IOExpander::Channel::Channel_8, data);
      if (data.getIsValid()) {
        updateAnalogInputTelemetry(expander, Drv::IOExpander::Channel::Channel_8, data.getAdcCode(), data.getVoltageValue());
      }
      else {
        return false;
      }
    }
    
    return true;
  }

  // IOExpander1 pins configured as digitalInputs, IOExpander2 pins configured as digitalOutputs
  void IOExpanderTester::Mode0() {
    if (switchPattern) {
      switchPattern = false;
      Pattern pattern;
      pattern.Ch1 = Fw::Logic::HIGH;
      pattern.Ch2 = Fw::Logic::LOW; 
      pattern.Ch3 = Fw::Logic::HIGH;
      pattern.Ch4 = Fw::Logic::LOW;
      pattern.Ch5 = Fw::Logic::HIGH;
      pattern.Ch6 = Fw::Logic::LOW;
      pattern.Ch7 = Fw::Logic::HIGH;
      pattern.Ch8 = Fw::Logic::LOW;
      
      bool status = digitalOutputSetPattern(pattern);
      if (status == false) {
        this->log_WARNING_HI_TestError();
      }

      Pattern actualPattern;
      status = digitalInputReceivePattern(IO::Expander1, actualPattern);
      if (status == false) {
        this->log_WARNING_HI_TestError();
      }

      if ((pattern.Ch1 != actualPattern.Ch1) || (pattern.Ch2 != actualPattern.Ch2) ||
          (pattern.Ch3 != actualPattern.Ch3) || (pattern.Ch4 != actualPattern.Ch4) ||
          (pattern.Ch5 != actualPattern.Ch5) || (pattern.Ch6 != actualPattern.Ch6) ||
          (pattern.Ch7 != actualPattern.Ch7) || (pattern.Ch8 != actualPattern.Ch8)) 
      {
        this->log_WARNING_HI_TestError();
      }
    }
    else {
      switchPattern = true;
      Pattern pattern;
      pattern.Ch1 = Fw::Logic::LOW;
      pattern.Ch2 = Fw::Logic::HIGH; 
      pattern.Ch3 = Fw::Logic::LOW;
      pattern.Ch4 = Fw::Logic::HIGH;
      pattern.Ch5 = Fw::Logic::LOW;
      pattern.Ch6 = Fw::Logic::HIGH;
      pattern.Ch7 = Fw::Logic::LOW;
      pattern.Ch8 = Fw::Logic::HIGH;
      
      bool status = digitalOutputSetPattern(pattern);
      if (status == false) {
        this->log_WARNING_HI_TestError();
      }
      
      Pattern actualPattern;
      status = digitalInputReceivePattern(IO::Expander1, actualPattern);
      if (status == false) {
        this->log_WARNING_HI_TestError();
      }

      if ((pattern.Ch1 != actualPattern.Ch1) || (pattern.Ch2 != actualPattern.Ch2) ||
          (pattern.Ch3 != actualPattern.Ch3) || (pattern.Ch4 != actualPattern.Ch4) ||
          (pattern.Ch5 != actualPattern.Ch5) || (pattern.Ch6 != actualPattern.Ch6) ||
          (pattern.Ch7 != actualPattern.Ch7) || (pattern.Ch8 != actualPattern.Ch8)) 
      {
        this->log_WARNING_HI_TestError();
      }
    }
  }

  // IOExpander1 pins configured as digitalInputs, IOExpander2 pins configured as digitalOutputs
  void IOExpanderTester::ModeMultiple0() {
    if (switchPattern) {
      switchPattern = false;
      Drv::IOExpander::Ports configPorts;
      configPorts.setChannel1(true);
      configPorts.setChannel2(true);
      configPorts.setChannel3(true);
      configPorts.setChannel4(true);
      configPorts.setChannel5(true);
      configPorts.setChannel6(true);
      configPorts.setChannel7(true);
      configPorts.setChannel8(true);

      Drv::IOExpander::PortsStates outPortsStates;
      outPortsStates.setChannel1(Fw::Logic::HIGH);
      outPortsStates.setChannel2(Fw::Logic::HIGH);
      outPortsStates.setChannel3(Fw::Logic::HIGH);
      outPortsStates.setChannel4(Fw::Logic::HIGH);
      outPortsStates.setChannel5(Fw::Logic::HIGH);
      outPortsStates.setChannel6(Fw::Logic::HIGH);
      outPortsStates.setChannel7(Fw::Logic::HIGH);
      outPortsStates.setChannel8(Fw::Logic::HIGH);

      bool isValid;
      Drv::IOExpander::DigitalMultipleOutputsData outputData;
      outputData.setPorts(configPorts);
      outputData.setStates(outPortsStates);
      this->writeDigitalMultipleOutputs_out(0, outputData, isValid);
      if (isValid == false) {
        this->log_WARNING_HI_TestError();
      }
      Drv::IOExpander::PortsStates inPortsStates = outputData.getStates();
      Drv::IOExpander::DigitalMultipleInputsData inputData;
      this->readDigitalMultipleInputs_out(0, configPorts, inputData);
      if (inputData.getIsValid() == false) {
        this->log_WARNING_HI_TestError();
      }
      if ((outPortsStates.getChannel1() != inPortsStates.getChannel1()) || (outPortsStates.getChannel2() != inPortsStates.getChannel2()) ||
          (outPortsStates.getChannel3() != inPortsStates.getChannel3()) || (outPortsStates.getChannel4() != inPortsStates.getChannel4()) ||
          (outPortsStates.getChannel5() != inPortsStates.getChannel5()) || (outPortsStates.getChannel6() != inPortsStates.getChannel6()) ||
          (outPortsStates.getChannel7() != inPortsStates.getChannel7()) || (outPortsStates.getChannel8() != inPortsStates.getChannel8()))
      {
        this->log_WARNING_HI_TestError();
      }
      else {
        Fw::Logic state = inPortsStates.getChannel1();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_1, state);

        state = inPortsStates.getChannel2();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_2, state);

        state = inPortsStates.getChannel3();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_3, state);

        state = inPortsStates.getChannel4();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_4, state);

        state = inPortsStates.getChannel5();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_5, state);

        state = inPortsStates.getChannel6();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_6, state);

        state = inPortsStates.getChannel7();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_7, state);

        state = inPortsStates.getChannel8();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_8, state);
      }
    }
    else {
      switchPattern = true;
      Drv::IOExpander::Ports portsConfig;
      portsConfig.setChannel1(true);
      portsConfig.setChannel2(true);
      portsConfig.setChannel3(true);
      portsConfig.setChannel4(true);
      portsConfig.setChannel5(true);
      portsConfig.setChannel6(true);
      portsConfig.setChannel7(true);
      portsConfig.setChannel8(true);

      Drv::IOExpander::PortsStates outPortsStates;
      outPortsStates.setChannel1(Fw::Logic::LOW);
      outPortsStates.setChannel2(Fw::Logic::LOW);
      outPortsStates.setChannel3(Fw::Logic::LOW);
      outPortsStates.setChannel4(Fw::Logic::LOW);
      outPortsStates.setChannel5(Fw::Logic::LOW);
      outPortsStates.setChannel6(Fw::Logic::LOW);
      outPortsStates.setChannel7(Fw::Logic::LOW);
      outPortsStates.setChannel8(Fw::Logic::LOW);
      Drv::IOExpander::DigitalMultipleOutputsData outputData;
      outputData.setPorts(portsConfig);
      outputData.setStates(outPortsStates);
      bool isValid;
      this->writeDigitalMultipleOutputs_out(0, outputData, isValid);
      if (isValid == false) {
        this->log_WARNING_HI_TestError();
      }

      Drv::IOExpander::DigitalMultipleInputsData inputData;
      this->readDigitalMultipleInputs_out(0, portsConfig, inputData);
      Drv::IOExpander::PortsStates inPortsStates = inputData.getStates();

      if ((outPortsStates.getChannel1() != inPortsStates.getChannel1()) || (outPortsStates.getChannel2() != inPortsStates.getChannel2()) ||
          (outPortsStates.getChannel3() != inPortsStates.getChannel3()) || (outPortsStates.getChannel4() != inPortsStates.getChannel4()) ||
          (outPortsStates.getChannel5() != inPortsStates.getChannel5()) || (outPortsStates.getChannel6() != inPortsStates.getChannel6()) ||
          (outPortsStates.getChannel7() != inPortsStates.getChannel7()) || (outPortsStates.getChannel8() != inPortsStates.getChannel8()))
      {
        this->log_WARNING_HI_TestError();
      }
      else {
        Fw::Logic state = inPortsStates.getChannel1();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_1, state);

        state = inPortsStates.getChannel2();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_2, state);

        state = inPortsStates.getChannel3();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_3, state);

        state = inPortsStates.getChannel4();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_4, state);

        state = inPortsStates.getChannel5();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_5, state);

        state = inPortsStates.getChannel6();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_6, state);

        state = inPortsStates.getChannel7();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_7, state);

        state = inPortsStates.getChannel8();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_8, state);
      }
    }
  }
  
  void IOExpanderTester::ModeChessMultiple0() {
    if (switchPattern) {
      switchPattern = false;
      Drv::IOExpander::Ports configPorts;
      configPorts.setChannel1(true);
      configPorts.setChannel2(false);
      configPorts.setChannel3(true);
      configPorts.setChannel4(false);
      configPorts.setChannel5(true);
      configPorts.setChannel6(false);
      configPorts.setChannel7(true);
      configPorts.setChannel8(false);

      Drv::IOExpander::PortsStates outPortsStates;
      outPortsStates.setChannel1(Fw::Logic::HIGH);
      outPortsStates.setChannel2(Fw::Logic::LOW);
      outPortsStates.setChannel3(Fw::Logic::HIGH);
      outPortsStates.setChannel4(Fw::Logic::LOW);
      outPortsStates.setChannel5(Fw::Logic::HIGH);
      outPortsStates.setChannel6(Fw::Logic::LOW);
      outPortsStates.setChannel7(Fw::Logic::HIGH);
      outPortsStates.setChannel8(Fw::Logic::LOW);

      Drv::IOExpander::DigitalMultipleOutputsData outputData;
      outputData.setPorts(configPorts);
      outputData.setStates(outPortsStates);
      bool isValid;
      this->writeDigitalMultipleOutputs_out(0, outputData, isValid);
      if (isValid == false) {
        this->log_WARNING_HI_TestError();
      }

      Drv::IOExpander::DigitalMultipleInputsData inputData;
      this->readDigitalMultipleInputs_out(0, configPorts, inputData);
      Drv::IOExpander::PortsStates inPortsStates = inputData.getStates();

      if ((outPortsStates.getChannel1() != inPortsStates.getChannel1()) ||
          (outPortsStates.getChannel3() != inPortsStates.getChannel3()) ||
          (outPortsStates.getChannel5() != inPortsStates.getChannel5()) ||
          (outPortsStates.getChannel7() != inPortsStates.getChannel7()))
      {
        this->log_WARNING_HI_TestError();
      }
      else {
        Fw::Logic state = inPortsStates.getChannel1();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_1, state);

        state = inPortsStates.getChannel2();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_2, state);

        state = inPortsStates.getChannel3();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_3, state);

        state = inPortsStates.getChannel4();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_4, state);

        state = inPortsStates.getChannel5();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_5, state);

        state = inPortsStates.getChannel6();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_6, state);

        state = inPortsStates.getChannel7();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_7, state);

        state = inPortsStates.getChannel8();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_8, state);
      }
    }
    else {
      switchPattern = true;
      Drv::IOExpander::Ports portsConfig;
      portsConfig.setChannel1(true);
      portsConfig.setChannel2(false);
      portsConfig.setChannel3(true);
      portsConfig.setChannel4(false);
      portsConfig.setChannel5(true);
      portsConfig.setChannel6(false);
      portsConfig.setChannel7(true);
      portsConfig.setChannel8(false);

      Drv::IOExpander::PortsStates outPortsStates;
      outPortsStates.setChannel1(Fw::Logic::LOW);
      outPortsStates.setChannel3(Fw::Logic::LOW);
      outPortsStates.setChannel5(Fw::Logic::LOW);
      outPortsStates.setChannel7(Fw::Logic::LOW);

      Drv::IOExpander::DigitalMultipleOutputsData outputData;
      outputData.setPorts(portsConfig);
      outputData.setStates(outPortsStates);
      bool isValid;
      this->writeDigitalMultipleOutputs_out(0, outputData, isValid);
      if (isValid == false) {
        this->log_WARNING_HI_TestError();
      }

      Drv::IOExpander::DigitalMultipleInputsData inputData;
      this->readDigitalMultipleInputs_out(0, portsConfig, inputData);
      Drv::IOExpander::PortsStates inPortsStates = inputData.getStates();

      if ((outPortsStates.getChannel1() != inPortsStates.getChannel1()) ||
          (outPortsStates.getChannel3() != inPortsStates.getChannel3()) ||
          (outPortsStates.getChannel5() != inPortsStates.getChannel5()) ||
          (outPortsStates.getChannel7() != inPortsStates.getChannel7()))
      {
        this->log_WARNING_HI_TestError();
      }
      else {
        Fw::Logic state = inPortsStates.getChannel1();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_1, state);

        state = inPortsStates.getChannel2();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_2, state);

        state = inPortsStates.getChannel3();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_3, state);

        state = inPortsStates.getChannel4();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_4, state);

        state = inPortsStates.getChannel5();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_5, state);

        state = inPortsStates.getChannel6();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_6, state);

        state = inPortsStates.getChannel7();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_7, state);

        state = inPortsStates.getChannel8();
        updateDigitalInputTelemetry(IO::Expander1, Drv::IOExpander::Channel::Channel_8, state);
      }
    }
  }

  // IOExpander2 pins configured as digitalInputs, IOExpander1 pins configured as digitalOutputs
  void IOExpanderTester::Mode1() {
    Pattern pattern;
    pattern.Ch1 = Fw::Logic::HIGH;
    pattern.Ch2 = Fw::Logic::LOW; 
    pattern.Ch3 = Fw::Logic::HIGH;
    pattern.Ch4 = Fw::Logic::LOW;
    pattern.Ch5 = Fw::Logic::HIGH;
    pattern.Ch6 = Fw::Logic::LOW;
    pattern.Ch7 = Fw::Logic::HIGH;
    pattern.Ch8 = Fw::Logic::LOW;
    
    bool status = false;

    status = digitalOutputSetPattern(pattern);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }
    
    Pattern actualPattern;
    status = digitalInputReceivePattern(IO::Expander2, actualPattern);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }

    if ((pattern.Ch1 != actualPattern.Ch1) || (pattern.Ch2 != actualPattern.Ch2) ||
        (pattern.Ch3 != actualPattern.Ch3) || (pattern.Ch4 != actualPattern.Ch4) ||
        (pattern.Ch5 != actualPattern.Ch5) || (pattern.Ch6 != actualPattern.Ch6) ||
        (pattern.Ch7 != actualPattern.Ch7) || (pattern.Ch8 != actualPattern.Ch8)) 
    {
      this->log_WARNING_HI_TestError();
    }

    pattern.Ch1 = Fw::Logic::LOW;
    pattern.Ch2 = Fw::Logic::HIGH; 
    pattern.Ch3 = Fw::Logic::LOW;
    pattern.Ch4 = Fw::Logic::HIGH;
    pattern.Ch5 = Fw::Logic::LOW;
    pattern.Ch6 = Fw::Logic::HIGH;
    pattern.Ch7 = Fw::Logic::LOW;
    pattern.Ch8 = Fw::Logic::HIGH;
    
    status = digitalOutputSetPattern(pattern);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }

    status = digitalInputReceivePattern(IO::Expander2, actualPattern);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }

    if ((pattern.Ch1 != actualPattern.Ch1) || (pattern.Ch2 != actualPattern.Ch2) ||
        (pattern.Ch3 != actualPattern.Ch3) || (pattern.Ch4 != actualPattern.Ch4) ||
        (pattern.Ch5 != actualPattern.Ch5) || (pattern.Ch6 != actualPattern.Ch6) ||
        (pattern.Ch7 != actualPattern.Ch7) || (pattern.Ch8 != actualPattern.Ch8)) 
    {
      this->log_WARNING_HI_TestError();
    }
  }
  
  // IOExpander1 pins configured as analogInputs,  IOExpander2 pins configured as digitalOutputs
  void IOExpanderTester::Mode2() {
    switchPattern = false;
    Pattern pattern;
    pattern.Ch1 = Fw::Logic::HIGH;
    pattern.Ch2 = Fw::Logic::HIGH; 
    pattern.Ch3 = Fw::Logic::HIGH;
    pattern.Ch4 = Fw::Logic::LOW;
    pattern.Ch5 = Fw::Logic::HIGH;
    pattern.Ch6 = Fw::Logic::HIGH;
    pattern.Ch7 = Fw::Logic::HIGH;
    pattern.Ch8 = Fw::Logic::HIGH;

    bool status = digitalOutputSetPattern(pattern);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }

    status = analogInputUpdateValues(IO::Expander1);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }
  }
  
  // IOExpander2 pins configured as analogInputs,  IOExpander1 pins configured as analogInputs
  void IOExpanderTester::Mode3() {
    this->calibrateADCOffset1_out(0);
    bool status = analogInputUpdateValues(IO::Expander1);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }

    this->calibrateADCOffset2_out(0);
    status = analogInputUpdateValues(IO::Expander2);
    if (status == false) {
      this->log_WARNING_HI_TestError();
    }
  }
}
