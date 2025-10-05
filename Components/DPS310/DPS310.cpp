// ======================================================================
// \title  DPS310.cpp
// \author user
// \brief  cpp file for DPS310 component implementation class
// ======================================================================

#include "Components/DPS310/DPS310.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  DPS310 ::
    DPS310(const char* const compName) :
      DPS310ComponentBase(compName),
      driverState(DriverState::DPS310_INIT)
  {

  }

  DPS310 ::
    ~DPS310()
  {

  }

  void DPS310::configure(F32 kt, F32 bt, F32 kp, F32 bp) {
    calibrCoeffTempK = kt;
    calibrCoeffTempB = bt;

    calibrCoeffPressK = kp;
    calibrCoeffPressB = bp;

    auto initializationResult = reset();
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    initializationResult = readCalibrationCoefficients();
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    initializationResult = readProdAndRevID();
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    initializationResult = configurePressureMeasurement(MeasurementRate::DPS310_2HZ, OversampleRate::DPS310_16SAMPLES);
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    initializationResult = configureTemperatureMeasurement(MeasurementRate::DPS310_2HZ, OversampleRate::DPS310_1SAMPLE);
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    initializationResult = generalConfiguration();
    if (initializationResult != ErrorCode::NoError) {
      errorReport(initializationResult);
    }

    auto startResult = startMeasurements();
    if (startResult != DPS310::ErrorCode::NoError) {
      errorReport(startResult);
    }
    
    auto prodIdResult = getProductID();
    if (prodIdResult.has_value() == false) {
      errorReport(prodIdResult.error());
    }

    auto revIdResult = getRevisionID();
    if (revIdResult.has_value() == false) {
      errorReport(revIdResult.error());
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  Drv::Sensor::Status DPS310::getPressure_handler(NATIVE_INT_TYPE portNum, F32& pressure) {
    auto pressureResult = getPressure();
    if (pressureResult.has_value() == false) {
      errorReport(pressureResult.error());
      return Drv::Sensor::Status::FAILED;  
    }
    
    pressure = calibrCoeffPressK * pressureResult.value() + calibrCoeffPressB;
    
    return Drv::Sensor::Status::SUCCESS;
  }

  Drv::Sensor::Status DPS310::getTemperature_handler(NATIVE_INT_TYPE portNum, F32& temperature) {
    auto temperatureResult = getTemperature();
    if (temperatureResult.has_value() == false) {
      errorReport(temperatureResult.error());
      return Drv::Sensor::Status::FAILED;
    }

    temperature = calibrCoeffTempK * temperatureResult.value() + calibrCoeffTempB;

    return Drv::Sensor::Status::SUCCESS;
  }

  void DPS310::errorReport(DPS310::ErrorCode errorCode) {
    switch (errorCode) {
        case DPS310::ErrorCode::NotInited:
            this->log_WARNING_HI_NotInited();
        break;
        case DPS310::ErrorCode::MeasurementTimeout:
            this->log_WARNING_HI_MeasurementTimeout();
        break;
        case DPS310::ErrorCode::FailedWriteToDPS310Register:
            this->log_WARNING_HI_FailedWriteToRegsiter();
        break;
        case DPS310::ErrorCode::FailedReadDPS310Register:
            this->log_WARNING_HI_FailedReadFromRegsiter();
        break;
        default:
            this->log_WARNING_HI_UnknownError();
        break;
    }
  }

  DPS310::ErrorCode DPS310::startMeasurements() {
    auto measurementConfiguration = startContinuousMeasurements();
    if (measurementConfiguration != ErrorCode::NoError) {
        return measurementConfiguration;
    }
    
    auto isMeasurementReady = waitUntilMeasurementReady();
    if (isMeasurementReady != ErrorCode::NoError) {
        return isMeasurementReady;
    }

    return ErrorCode::NoError;
  }

  DPS310::ErrorCode DPS310::stopMeasurements() {
      const uint8_t stopMeasurements = 0b000;
      if (!writeReg(DPS310_REG_MEASCFG, stopMeasurements)) {
          return ErrorCode::FailedWriteToDPS310Register;
      }

      return ErrorCode::NoError;
  }

  etl::expected<float, DPS310::ErrorCode> DPS310::getTemperature() {
    if (blockMeasurements) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    auto rawTemperature = getRawTemperature();
    if (!rawTemperature.has_value()) {
        return etl::unexpected(rawTemperature.error());
    }

    return calculateTemperature(rawTemperature.value());
  }

  etl::expected<float, DPS310::ErrorCode> DPS310::getPressure() {
      if (blockMeasurements) {
          return etl::unexpected(ErrorCode::NotInited);
      }

      auto rawTemperature = getRawTemperature();
      if (!rawTemperature.has_value()) {
          return etl::unexpected(rawTemperature.error());
      }

      auto rawPressure = getRawPressure();
      if (!rawPressure.has_value()) {
          return etl::unexpected(rawPressure.error());
      }

      const rawMeasurements measurements = {rawPressure.value(), rawTemperature.value()};

      return calculatePressure(measurements);
  }

  etl::expected<uint8_t, DPS310::ErrorCode> DPS310::getProductID() const {
      if (prodAndRevIdReady) {
          return productID;
      }
      return etl::unexpected(ErrorCode::NotInited);
  }

  etl::expected<uint8_t, DPS310::ErrorCode> DPS310::getRevisionID() const {
      if (prodAndRevIdReady) {
          return revisionID;
      }
      return etl::unexpected(ErrorCode::NotInited);
  }

  DPS310::ErrorCode DPS310::readProdAndRevID() {
      uint8_t prodAndRevIDReg = 0;

      if (!readReg(DPS310_REG_PRODREVID, prodAndRevIDReg)) {
          return ErrorCode::FailedReadDPS310Register;
      }

      const uint8_t prodIDMask = 0b1111;
      const uint8_t revIDMask = 0b1111'0000;
      const uint8_t revIDOffset = 4U;
      productID = prodAndRevIDReg & prodIDMask;
      revisionID = static_cast<uint8_t>(prodAndRevIDReg & revIDMask) >> revIDOffset;

      prodAndRevIdReady = true;

      return ErrorCode::NoError;
  }

  DPS310::ErrorCode DPS310::reset() {
      const uint8_t resetSequence = 0b1001;
      if (!writeReg(DPS310_REG_RESET, resetSequence)) {
          return ErrorCode::FailedWriteToDPS310Register;
      }

      auto isSensorReady = waitUntilSensorReady();
      if (!isSensorReady.has_value()) {
          return isSensorReady.error();
      }

      if (!isSensorReady.value()) {
          blockMeasurements = true;
          return ErrorCode::NotInited;
      }

      return ErrorCode::NoError;
  }

  bool DPS310::readReg(uint8_t reg, uint8_t &value) {
    etl::array<uint8_t, 1> arr = {reg};
    Fw::Buffer writeBuffer(arr.data(), arr.size());
    if (write_out(0, DPS310Addr, writeBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }

    etl::array<uint8_t, 1> readArr{};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    if (read_out(0, DPS310Addr, readBuffer, readTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }
    value = readArr.front();

    return true;
  }

  bool DPS310::writeReg(uint8_t reg, uint8_t value) {
    etl::array<uint8_t, 2> arr = {reg, value};
    Fw::Buffer writeBuffer(arr.data(), arr.size());
    return (write_out(0, DPS310Addr, writeBuffer, writeTimeout) == Drv::I2C::Status::SUCCESS);
  }

  int32_t DPS310::transformTwosComplement(int32_t val, uint8_t bits) {
      if ((static_cast<uint32_t>(val) & (static_cast<uint32_t>(1) << static_cast<uint32_t>(bits - 1))) != 0U) {
          val -= static_cast<int32_t>(static_cast<uint32_t>(1) << bits);
      }
      return val;
  }

  etl::expected<bool, DPS310::ErrorCode> DPS310::waitUntilCoefficientsAvailable() {
      const uint8_t coefRdyMask = 0b1000'0000;
      const uint8_t coefRdyOffset = 7U;
      uint8_t measConfigValue = 0;
      uint8_t coefRdy = 0;

      const uint8_t timeToCoefficientsAvailableMs = 50;
      Os::Task::delay(timeToCoefficientsAvailableMs);

      if (!readReg(DPS310_REG_MEASCFG, measConfigValue)) {
          return etl::unexpected(ErrorCode::FailedReadDPS310Register);
      }
      coefRdy = static_cast<uint8_t>(measConfigValue & coefRdyMask) >> coefRdyOffset;

      return coefRdy == 1;
  }

  etl::expected<bool, DPS310::ErrorCode> DPS310::waitUntilSensorReady() {
      const uint8_t sensorRdyMask = 0b0100'0000;
      const uint8_t sensorRdyOffset = 6U;
      uint8_t measConfigValue = 0;
      uint8_t sensorRdy = 0;

      const uint8_t timeToSensorReadyMs = 20;
      Os::Task::delay(timeToSensorReadyMs);

      if (!readReg(DPS310_REG_MEASCFG, measConfigValue)) {
          return etl::unexpected(ErrorCode::FailedReadDPS310Register);
      }
      sensorRdy = static_cast<uint8_t>(measConfigValue & sensorRdyMask) >> sensorRdyOffset;

      return sensorRdy == 1;
  }

  DPS310::ErrorCode DPS310::readCalibrationCoefficients() {
      auto areCoefficientsReady = waitUntilCoefficientsAvailable();

      if (!areCoefficientsReady.has_value()) {
          return areCoefficientsReady.error();
      }

      if (!areCoefficientsReady.value()) {
          blockMeasurements = true;
          return ErrorCode::NotInited;
      }

      etl::array<uint8_t, numOfCalibrCoeffRegisters> coefficients{};

      uint8_t coeffValue{0};
      for (uint8_t i = 0; i < numOfCalibrCoeffRegisters; i++) {
          if (!readReg(firstCalibrCoeffRegister + i, coeffValue)) {
              return ErrorCode::FailedReadDPS310Register;
          }
          coefficients.at(i) = coeffValue;
      }

      // Magic numbers used here are used to access elements in the array of two's complement calibration coefficients
      // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
      bits_c0 |= coefficients.at(0) << u4bitsOffset;
      bits_c0 |= static_cast<uint16_t>((static_cast<uint16_t>(coefficients.at(1)) >> u4bitsOffset)) & u4mask;
      c0 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c0.to_ulong()), coeff12BitSize));

      bits_c1 |= static_cast<uint16_t>((static_cast<uint16_t>(coefficients.at(1)) & u4mask)) << u8bitsOffset;
      bits_c1 |= coefficients.at(2) & u8mask;
      c1 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c1.to_ulong()), coeff12BitSize));

      bits_c00 |= coefficients.at(3) << u12bitsOffset;
      bits_c00 |= coefficients.at(4) << u4bitsOffset;
      bits_c00 |= (static_cast<uint32_t>(coefficients.at(5)) >> u4bitsOffset) & u4mask;
      c00 = transformTwosComplement(static_cast<int32_t>(bits_c00.to_ulong()), coeff20BitSize);

      bits_c10 |= (static_cast<uint32_t>(coefficients.at(5)) & u4mask) << u16bitsOffset;
      bits_c10 |= coefficients.at(6) << u8bitsOffset;
      bits_c10 |= coefficients.at(7) & u8mask;
      c10 = transformTwosComplement(static_cast<int32_t>(bits_c10.to_ulong()), coeff20BitSize);

      bits_c01 |= coefficients.at(8) << u8bitsOffset;
      bits_c01 |= coefficients.at(9) & u8mask;
      c01 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c01.to_ulong()), coeff16BitSize));

      bits_c11 |= coefficients.at(10) << u8bitsOffset;
      bits_c11 |= coefficients.at(11) & u8mask;
      c11 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c11.to_ulong()), coeff16BitSize));

      bits_c20 |= coefficients.at(12) << u8bitsOffset;
      bits_c20 |= coefficients.at(13);
      c20 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c20.to_ulong()), coeff16BitSize));

      bits_c21 |= coefficients.at(14) << u8bitsOffset;
      bits_c21 |= coefficients.at(15);
      c21 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c21.to_ulong()), coeff16BitSize));

      bits_c30 |= coefficients.at(16) << u8bitsOffset;
      bits_c30 |= coefficients.at(17) & u8mask;
      c30 = static_cast<int16_t>(transformTwosComplement(static_cast<int32_t>(bits_c30.to_ulong()), coeff16BitSize));
      // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

      return ErrorCode::NoError;
  }

  DPS310::ErrorCode DPS310::configurePressureMeasurement(MeasurementRate measurementRate, OversampleRate oversampleRate) {
      etl::bitset<register8BitSize, uint8_t> registerValue;

      auto PM_RATE = (static_cast<uint8_t>(measurementRate) << u4bitsOffset);
      registerValue |= PM_RATE;

      auto PM_PRC = static_cast<uint8_t>(oversampleRate);
      registerValue |= PM_PRC;

      if (oversampleRate > OversampleRate::DPS310_8SAMPLES) {
          needShiftForPressure = true;
      }

      if (!writeReg(DPS310_REG_PRSCFG, registerValue.to_ulong())) {
          return ErrorCode::FailedWriteToDPS310Register;
      }
      
      configureMeasurementConstants(timeDelayPrsRdyMs, scaleFactorkP, oversampleRate);
      return ErrorCode::NoError;
  }

  DPS310::ErrorCode DPS310::configureTemperatureMeasurement(MeasurementRate measurementRate, OversampleRate oversampleRate) {
      etl::bitset<register8BitSize, uint8_t> registerValue;

      auto TMP_RATE = (static_cast<uint8_t>(measurementRate) << u4bitsOffset);
      registerValue |= TMP_RATE;

      auto TMP_PRC = static_cast<uint8_t>(oversampleRate);
      registerValue |= TMP_PRC;

      if (oversampleRate > OversampleRate::DPS310_8SAMPLES) {
          needShiftForTemperature = true;
      }

      uint8_t coeffSource{0};
      readReg(DPS310_REG_TMPCOEFSRCE, coeffSource);
      const uint8_t tempExtMask = 0b1000'0000;
      auto TMP_EXT = (coeffSource & tempExtMask);
      registerValue |= TMP_EXT;

      if (!writeReg(DPS310_REG_TMPCFG, registerValue.to_ulong())) {
          return ErrorCode::FailedWriteToDPS310Register;
      }
      
      configureMeasurementConstants(timeDelayTempRdyMs, scaleFactorkT, oversampleRate);
      return ErrorCode::NoError;
  }

  void DPS310::configureMeasurementConstants(uint32_t &delayTimeToConfigure, int32_t &scaleFactor, OversampleRate oversampleRate) {
    switch (oversampleRate) {
        case OversampleRate::DPS310_1SAMPLE :
            scaleFactor = scaleFactorFor1Sample;
            delayTimeToConfigure = measureTimeFor1Sample;
            break;

        case OversampleRate::DPS310_2SAMPLES :
            scaleFactor = scaleFactorFor2Samples;
            delayTimeToConfigure = measureTimeFor2Samples;
            break;

        case OversampleRate::DPS310_4SAMPLES :
            scaleFactor = scaleFactorFor4Samples;
            delayTimeToConfigure = measureTimeFor4Samples;
            break;

        case OversampleRate::DPS310_8SAMPLES :
            scaleFactor = scaleFactorFor8Samples;
            delayTimeToConfigure = measureTimeFor8Samples;
            break;

        case OversampleRate::DPS310_16SAMPLES :
            scaleFactor = scaleFactorFor16Samples;
            delayTimeToConfigure = measureTimeFor16Samples;
            break;

        case OversampleRate::DPS310_32SAMPLES :
            scaleFactor = scaleFactorFor32Samples;
            delayTimeToConfigure = measureTimeFor32Samples;
            break;

        case OversampleRate::DPS310_64SAMPLES :
            scaleFactor = scaleFactorFor64Samples;
            delayTimeToConfigure = measureTimeFor64Samples;
            break;

        case OversampleRate::DPS310_128SAMPLES :
            scaleFactor = scaleFactorFor128Samples;
            delayTimeToConfigure = measureTimeFor128Samples;
            break;

        default:
            break;
    }
  }

  DPS310::ErrorCode DPS310::generalConfiguration() {
      etl::bitset<register8BitSize> cfgRegValue;

      if (needShiftForPressure) {
          const uint8_t P_SHIFT_ENABLE = 1U << 2U;
          cfgRegValue |= P_SHIFT_ENABLE;
      }

      if (needShiftForTemperature) {
          const uint8_t T_SHIFT_VALUE = 1U << 3U;
          cfgRegValue |= T_SHIFT_VALUE;
      }

      if (!writeReg(DPS310_REG_CFGREG, cfgRegValue.to_ulong())) {
          return ErrorCode::FailedWriteToDPS310Register;
      }

      return ErrorCode::NoError;
  }

  etl::expected<int32_t, DPS310::ErrorCode> DPS310::getRawTemperature() {
      etl::bitset<rawData24BitSize> twosComplementRawTemperature;
      uint8_t rawTemperatureByte = 0;

      for (uint8_t i = 0; i < DPS310_NUM_OF_RAW_BYTES; i++) {
          if (!readReg(DPS310_REG_TMPB0 - i, rawTemperatureByte)) {
              return etl::unexpected(ErrorCode::FailedReadDPS310Register);
          }
          twosComplementRawTemperature |= (rawTemperatureByte << static_cast<uint8_t>(u8bitsOffset * i));
      }

      const int32_t rawTemperature =
              transformTwosComplement(static_cast<int32_t>(twosComplementRawTemperature.to_ulong()), rawData24BitSize);

      return rawTemperature;
  }

  etl::expected<int32_t, DPS310::ErrorCode> DPS310::getRawPressure() {
      etl::bitset<rawData24BitSize> twosComplementRawPressure;
      uint8_t rawPressureByte = 0;

      for (uint8_t i = 0; i < DPS310_NUM_OF_RAW_BYTES; i++) {
          if (!readReg(DPS310_REG_PRSB0 - i, rawPressureByte)) {
              return etl::unexpected(ErrorCode::FailedReadDPS310Register);
          }
          twosComplementRawPressure |= (rawPressureByte << static_cast<uint8_t>(u8bitsOffset * i));
      }

      const int32_t rawPressure =
              transformTwosComplement(static_cast<int32_t>(twosComplementRawPressure.to_ulong()), rawData24BitSize);

      return rawPressure;
  }

  float DPS310::calculateTemperature(int32_t rawTemperature) const {
      const float rawTemperatureScaled = static_cast<float>(rawTemperature) / scaleFactorkT;

      const float formulaConstant = 0.5F;
      const float compensatedTemperature =
              static_cast<float>(c0) * formulaConstant + static_cast<float>(c1) * rawTemperatureScaled;

      return compensatedTemperature;
  }

  float DPS310::calculatePressure(rawMeasurements measurements) const {
      const float rawTemperatureScaled = static_cast<float>(measurements.rawTemperature) / scaleFactorkT;
      const float rawPressureScaled = static_cast<float>(measurements.rawPressure) / scaleFactorkP;

      const float compensatedPressure =
              static_cast<float>(c00) +
              rawPressureScaled *
                      (static_cast<float>(c10) +
                       rawPressureScaled * (static_cast<float>(c20) + rawPressureScaled * static_cast<float>(c30))) +
              rawTemperatureScaled * static_cast<float>(c01) +
              rawTemperatureScaled * rawPressureScaled *
                      (static_cast<float>(c11) + rawPressureScaled * static_cast<float>(c21));

      return compensatedPressure;
  }

  DPS310::ErrorCode DPS310::waitUntilMeasurementReady() {
    const uint8_t tempRdyMask = 0b0010'0000;
    const uint8_t tempRdyOffset = 5U;
    const uint8_t prsRdyMask = 0b0001'0000;
    const uint8_t prsRdyOffset = 4U;
    uint8_t measConfigValue = 0;
    uint8_t tempRdy = 0;
    uint8_t prsRdy = 0;

    const uint32_t totalTimeDelay = timeDelayTempRdyMs + timeDelayPrsRdyMs;
    const uint16_t maxAttempts = 3;
    uint16_t numOfAttempts = 0;
    
    while ((prsRdy != 1) || (tempRdy != 1)) {
        if (numOfAttempts == maxAttempts) {
            return ErrorCode::MeasurementTimeout;
        }
        Os::Task::delay(totalTimeDelay);

        if (!readReg(DPS310_REG_MEASCFG, measConfigValue)) {
            return ErrorCode::FailedReadDPS310Register;
        }
        tempRdy = static_cast<uint8_t>(measConfigValue & tempRdyMask) >> tempRdyOffset;
        prsRdy = static_cast<uint8_t>(measConfigValue & prsRdyMask) >> prsRdyOffset;
        numOfAttempts++;
    }

    return ErrorCode::NoError;
  }

  DPS310::ErrorCode DPS310::startContinuousMeasurements() {
      const uint8_t continuousMeasurements = 0b111;

      if (!writeReg(DPS310_REG_MEASCFG, continuousMeasurements)) {
          return ErrorCode::FailedWriteToDPS310Register;
      }

      return ErrorCode::NoError;
  }

}
