// ======================================================================
// \title  ExtBatManager.cpp
// \author user
// \brief  cpp file for ExtBatManager component implementation class
// ======================================================================

#include "Components/ExtBatManager/ExtBatManager.hpp"
#include "FpConfig.hpp"
#include <etl/expected.h>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ExtBatManager ::
    ExtBatManager(const char* const compName) :
      ExtBatManagerComponentBase(compName)
  {
    extBatVoltageData.setIsValid(false);
  }

  ExtBatManager ::
    ~ExtBatManager()
  {

  }

  void ExtBatManager ::configure(const Thresholds &thresholds) {
    FW_ASSERT((thresholds.Min < thresholds.Max), thresholds.Max);
    FW_ASSERT((thresholds.Min >= minPossibleThresholdMiliVolts), thresholds.Min);
    FW_ASSERT((thresholds.Max <= maxPossibleThresholdMiliVolts), thresholds.Max);
    extBatThresholds.Min = thresholds.Min;
    extBatThresholds.Max = thresholds.Max;

    F32 voltageValue = 0.0;
    bool initialMeasurementsSuccess{true};
    for (U32 i = 0; i < numOfMeasurementsForAveraging; i++) {
      if (receiveInstantaneousVoltageMeasurement(voltageValue) == false) {
        initialMeasurementsSuccess = false;
      }
    }
    
    if (initialMeasurementsSuccess == false) {
      this->log_WARNING_LO_NotFullInit();
    }
  }
      
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void ExtBatManager::getVoltage_handler(
        NATIVE_INT_TYPE portNum,
        Drv::ExtBat::VoltageData& voltageData)
  {
    voltageData = this->extBatVoltageData;
  }

  void ExtBatManager::run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    F32 voltageValue                 = 0.0;
    F32 averagedVoltage              = 0.0;
    bool isVoltageMeasurementSuccess = receiveInstantaneousVoltageMeasurement(voltageValue);
    bool isAveragingSuccess          = receiveAveragedVoltage(averagedVoltage);

    if (isVoltageMeasurementSuccess && isAveragingSuccess) {
      voltageValid = true;
    }
    else {
      voltageValid = false;
    }

    if (isVoltageMeasurementSuccess) {
      bool isOvervoltage = checkOvervoltage(voltageValue);
      if (isOvervoltage) {
        voltageValid = false;
        overvoltageAlarm = true;
  
        if (overvoltageReportedFlag == false) {
          overvoltageReportedFlag = true;
          this->log_WARNING_HI_OvervoltageAlarm();
        }
      }
      else {
        overvoltageAlarm = false;
        overvoltageReportedFlag = false;
      }

      bool isUndervoltage = checkUndervoltage(voltageValue);
      if (isUndervoltage) {
        voltageValid = false;
        undervoltageAlarm = true;

        if (undervoltageReportedFlag == false) {
          undervoltageReportedFlag = true;
          this->log_WARNING_HI_UndervoltageAlarm();
        }
      }
      else {
        undervoltageAlarm = false;
        undervoltageReportedFlag = false;
      }
    }
    
    if (voltageValid == false) {
      this->log_WARNING_HI_VoltageInvalid();
    }

    this->lock();
    extBatVoltageData.setIsValid(voltageValid);
    extBatVoltageData.setInstantaneousVoltage(voltageValue);
    extBatVoltageData.setAverageVoltage(averagedVoltage);
    extBatVoltageData.setOvervoltageAlarm(overvoltageAlarm);
    extBatVoltageData.setUndervoltageAlarm(undervoltageAlarm);
    this->unLock();

    this->tlmWrite_InstantaneousVoltage(extBatVoltageData.getInstantaneousVoltage());
    this->tlmWrite_AverageVoltage(extBatVoltageData.getAverageVoltage());
    this->tlmWrite_IsValid(extBatVoltageData.getIsValid());
    this->tlmWrite_UndervoltageAlarm(extBatVoltageData.getUndervoltageAlarm());
    this->tlmWrite_OvervoltageAlarm(extBatVoltageData.getOvervoltageAlarm());
  }

  etl::expected<F32, ExtBatManager::ErrorCode> ExtBatManager::getVoltage() {
    Fw::Logic pinSet = Fw::Logic::HIGH;
    this->enablePin_out(0, pinSet);

    Os::Task::delay(delayForBatteryMeasurementMs);
    
    F32 voltageValue = 0.0;
    auto voltageMeasurementStatus = this->getVoltageValue_out(0, voltageValue);

    Fw::Logic pinReset = Fw::Logic::LOW;
    this->enablePin_out(0, pinReset);

    if (voltageMeasurementStatus != Drv::ADC::Status::SUCCESS) {
      this->log_WARNING_LO_MeasurementError();
      return etl::unexpected(ErrorCode::VoltageMeasurementError);
    }
    return calculateBatteryVoltage(voltageValue);
  }

  F32 ExtBatManager::calculateBatteryVoltage(F32 inputVoltage) {
    return maxBatteryVoltage * inputVoltage / maxInputVoltage;
  }

  bool ExtBatManager::receiveAveragedVoltage(F32& result) {
    bool isNumOfMeasurementsEnough = (bufferOfMeasurements.size() == numOfMeasurementsForAveraging);
    if (isNumOfMeasurementsEnough) {
      F32 averagedVoltage = 0.0;
      for (const auto& measurement : bufferOfMeasurements) {
        averagedVoltage += measurement;
      }
      averagedVoltage /= numOfMeasurementsForAveraging;
      result = averagedVoltage;
      return true;
    }
    return false;
  }

  bool ExtBatManager::receiveInstantaneousVoltageMeasurement(F32& voltageValue) {
    auto voltageValueResult = getVoltage();
    bool isVoltageReceiveSuccess = voltageValueResult.has_value();
    
    if (isVoltageReceiveSuccess) {
      voltageValue = voltageValueResult.value();
      bufferOfMeasurements.push(voltageValue);
      return true;
    }
    return false;
  }

  bool ExtBatManager::checkOvervoltage(const F32& voltageValue) {
    bool isOvervoltage = (voltageValue > (static_cast<F32>(extBatThresholds.Max) / scaleMiliVoltsToVolts));
    return isOvervoltage;
  }

  bool ExtBatManager::checkUndervoltage(const F32& voltageValue) {
    bool isUndervoltage = (voltageValue < (static_cast<F32>(extBatThresholds.Min) / scaleMiliVoltsToVolts));
    return isUndervoltage;
  }
}
