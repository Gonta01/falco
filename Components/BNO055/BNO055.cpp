// ======================================================================
// \title  BNO055.cpp
// \author user
// \brief  cpp file for BNO055 component implementation class
// ======================================================================

#include "Components/BNO055/BNO055.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  BNO055 ::
    BNO055(const char* const compName) :
      BNO055ComponentBase(compName)
  {

  }

  BNO055 ::
    ~BNO055()
  {

  }

  Drv::IMU::Status BNO055::getAccelData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::AccelData& accelData
    )
  {
    auto accelDataResult = getAccelData();
    if (!accelDataResult.has_value()) {
      errorReport(accelDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    auto retrievedAccelData = accelDataResult.value(); 
    accelData.setX(retrievedAccelData.x);
    accelData.setY(retrievedAccelData.y);
    accelData.setZ(retrievedAccelData.z);

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status BNO055::getGyroData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::GyroData& gyroData
    )
  {
    auto gyroDataResult = getGyroData();
    if (!gyroDataResult.has_value()) {
      errorReport(gyroDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    auto retrievedGyroData = gyroDataResult.value();
    gyroData.setX(retrievedGyroData.x);
    gyroData.setY(retrievedGyroData.y);
    gyroData.setZ(retrievedGyroData.z);

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status BNO055::getMagnData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::MagnData& magnData
    )
  {
    auto magnDataResult = getMagneticData();
    if (!magnDataResult.has_value()) {
      errorReport(magnDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    auto retrievedMagnData = magnDataResult.value();
    magnData.setX(retrievedMagnData.x);
    magnData.setY(retrievedMagnData.y);
    magnData.setZ(retrievedMagnData.z);

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status BNO055::getRawData_handler(NATIVE_INT_TYPE portNum, Drv::IMU::RawData& rawData) {
    auto imuDataResult = getIMUData();

    if (!imuDataResult.has_value()) {
      errorReport(imuDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    auto retrievedIMUData = imuDataResult.value();
    Drv::IMU::AccelData accelData;
    accelData.setX(retrievedIMUData.accelData.x);
    accelData.setY(retrievedIMUData.accelData.y);
    accelData.setZ(retrievedIMUData.accelData.z);

    Drv::IMU::MagnData magnData;
    magnData.setX(retrievedIMUData.magnData.x);
    magnData.setY(retrievedIMUData.magnData.y);
    magnData.setZ(retrievedIMUData.magnData.z);

    Drv::IMU::GyroData gyroData;
    gyroData.setX(retrievedIMUData.gyroData.x - GyroXOffset);
    gyroData.setY(retrievedIMUData.gyroData.y - GyroYOffset);
    gyroData.setZ(retrievedIMUData.gyroData.z - GyroZOffset);

    rawData.setaccelData(accelData);
    rawData.setmagnData(magnData);
    rawData.setgyroData(gyroData);

    return Drv::IMU::Status::SUCCESS;
  }
  
  Drv::IMU::Status BNO055::getQuatData_handler(
      NATIVE_INT_TYPE portNum,
      Drv::IMU::QuatData& quatData
  )
  {
    auto quaternionsAnglesResult = getQuaternionAngles();
      if (!quaternionsAnglesResult.has_value()) {
        errorReport(quaternionsAnglesResult.error());
        return Drv::IMU::Status::FAILED;
      }

      auto retrievedQuaternionAngles = quaternionsAnglesResult.value();
      quatData.setQ0(retrievedQuaternionAngles.w);
      quatData.setQ1(retrievedQuaternionAngles.x);
      quatData.setQ2(retrievedQuaternionAngles.y);
      quatData.setQ3(retrievedQuaternionAngles.z);

      return Drv::IMU::Status::SUCCESS;
  }

  void BNO055::configure(Mode mode) {
    U8 numOfAttempts = 0;

    ErrorCode initStatus = ErrorCode::INIT_FAIL;
    while (initStatus != ErrorCode::OPERATION_SUCCESS) {
      if (numOfAttempts >= numOfReinitAttempts) {
        break;
      }
      Os::Task::delay(powerOnResetTime);
      initStatus = initialize(mode);
      numOfAttempts++;
    }
  }

  BNO055::ErrorCode BNO055::initialize(Mode mode) {
    /* Write the default page as zero*/
    constexpr U8 chosenPage = 0;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 stResult = 0;
    ErrorCode readStatus = readReg(stResultAddrPage0, stResult);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }

    ErrorCode testStatus = ErrorCode::OPERATION_SUCCESS;
    if(stResult != expectedStResult) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
      errorReport(testStatus);
      return testStatus;
    }

    constexpr U8 numOfRegsToReceive = 8;
    etl::array<U8, numOfRegsToReceive> readArr;
    Fw::Buffer receiveRegs(readArr.data(), readArr.size());
    readStatus = readRegs(chipIDAddrPage0, numOfRegsToReceive, receiveRegs); 
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }
    
    constexpr U8 expectedPageId = 0x00;
    U8 chipId                   = readArr.at(0);
    U8 accelRevId               = readArr.at(1);
    U8 magRevId                 = readArr.at(2);
    U8 gyroRevId                = readArr.at(3);
    U8 swRevIdLsb               = readArr.at(4);
    U8 swRevIdMsb               = readArr.at(5);
    U8 blRevID                  = readArr.at(6);
    U8 pageID                   = readArr.at(7);

    if (expectedChipID != chipId) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedAccID != accelRevId) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedMagID != magRevId) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedGyrID != gyroRevId) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedSwRevIdLsb != swRevIdLsb) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedSwRevIdMsb != swRevIdMsb) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedBlRevId != blRevID) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (expectedPageId != pageID) {
      testStatus = ErrorCode::SELF_TEST_FAIL;
    }
    if (testStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(testStatus);
      return testStatus;
    }
      
    /* set operation mode */
    // 1. get operation mode
    U8 currentOperationMode;
    readStatus = readReg(operationModeAddrPage0, currentOperationMode);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }
    // 2. if in config mode, set operation mode
    if (currentOperationMode == configMode) {
      Fw::Buffer writeBuff(defaultCalibArr.data(), defaultCalibArr.size());
      ErrorCode operationStatus = setCalibrationProfile(writeBuff);
      if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
        errorReport(operationStatus);
        return operationStatus;
      }
      calibrationFinished = true;

      if (mode == Mode::NO_FUSION) {
        operationStatus = noFusionConfiguration();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }

        operationStatus = setAMGMode();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }

        operationStatus = calibrateGyroOffsets();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }
      }
      else {
        operationStatus = setNDOFMode();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }
      }      
    }
    // 3. if not in config mode, set in config mode, then set operation mode
    else {
      ErrorCode writeStatus = setConfigMode();
      if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
        errorReport(writeStatus);
        return writeStatus;
      }
      
      Fw::Buffer writeBuff(defaultCalibArr.data(), defaultCalibArr.size());
      ErrorCode operationStatus = setCalibrationProfile(writeBuff);
      if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
        errorReport(operationStatus);
        return operationStatus;
      }
      calibrationFinished = true;

      if (mode == Mode::NO_FUSION) {
        operationStatus = noFusionConfiguration();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }

        operationStatus = setAMGMode();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }

        operationStatus = calibrateGyroOffsets();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }
      }
      else {
        operationStatus = setNDOFMode();
        if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
          errorReport(operationStatus);
          return operationStatus;
        }
      } 
    }

    readStatus = readReg(operationModeAddrPage0, currentOperationMode);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }

    if (currentOperationMode == amgMode) {
      initializationFinished = true;
      return ErrorCode::OPERATION_SUCCESS;
    }
    return ErrorCode::INIT_FAIL;
  }

  BNO055::ErrorCode BNO055::setPowerMode(PowerMode powerMode) {
    constexpr U8 chosenPage = 0;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(powerMode));

    writeStatus = writeReg(pwrModeAddrPage0, config);

    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }
    
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setOperatingMode(OperatingMode mode) {
    constexpr U8 chosenPage = 0;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(mode));

    writeStatus = writeReg(operationModeAddrPage0, config);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      return writeStatus;
    }
    Os::Task::delay(modeSwitchTime);
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setUnits(AccelUnits accelUnits, AngularRateUnits angRateUnits, EulerAnglesUnits eulerAngleUnits, TemperatureUnits tempUnits, OrientationModeUnits orientnation) {
    constexpr U8 chosenPage = 0;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(accelUnits) | static_cast<U8>(angRateUnits) | static_cast<U8>(eulerAngleUnits) | static_cast<U8>(tempUnits) | static_cast<U8>(orientnation));

    writeStatus = writeReg(unitSelPage0, config);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      return writeStatus;
    }

    if (accelUnits == AccelUnits::M_s2) {
      accDiv = accM_s2Div;
    }
    else {
      accDiv = accMgDiv;
    }

    if (angRateUnits == AngularRateUnits::Dps) {
      gyroDiv = gyroDpsDiv;
    }
    else {
      gyroDiv = gyroRpsDiv;
    }

    if (eulerAngleUnits == EulerAnglesUnits::Degrees) {
      eulerDiv = eulerDivDeg;
    }
    else {
      eulerDiv = eulerDivRad;
    }

    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setAccelConfig(AccelGRange range, AccelBandwidth bandwidth, AccelOperationMode mode) {
    constexpr U8 chosenPage = 1;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(mode) | static_cast<U8>(bandwidth) | static_cast<U8>(range));

    writeStatus = writeReg(accConfigAddrPage1, config);

    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }
    
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setGyroConfig(GyroDpsRange range, GyroBandwidth bandwidth, GyroPowerMode mode) {
    constexpr U8 chosenPage = 1;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(bandwidth) | static_cast<U8>(range));

    writeStatus = writeReg(gyrConfig0AddrPage1, config);

    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }
    
    config = static_cast<U8>(mode);
    writeStatus = writeReg(gyrConfig1AddrPage1, config);

    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setMagConfig(MagDataOutputRate rate, MagOperationMode operationMode, MagPowerMode powerMode) {
    constexpr U8 chosenPage = 1;
    ErrorCode writeStatus = writeReg(pageIDAddr, chosenPage);
    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }

    U8 config = (static_cast<U8>(powerMode) | static_cast<U8>(operationMode) | static_cast<U8>(rate));

    writeStatus = writeReg(magConfigAddrPage1, config);

    if (writeStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(writeStatus);
      return writeStatus;
    }
    
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::noFusionConfiguration() {
    auto operationStatus = ErrorCode::OPERATION_SUCCESS;

    operationStatus = setAccelConfig(AccelGRange::Range4G, AccelBandwidth::Bandwidth62_5Hz, AccelOperationMode::Normal);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(operationStatus);
      return operationStatus;
    }

    operationStatus = setGyroConfig(GyroDpsRange::Range2000dps, GyroBandwidth::Bandwidth32Hz, GyroPowerMode::Normal);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(operationStatus);
      return operationStatus;
    }

    operationStatus = setMagConfig(MagDataOutputRate::Rate20Hz, MagOperationMode::HighAccuracy, MagPowerMode::Normal);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(operationStatus);
      return operationStatus;
    }

    return operationStatus;
  }

  BNO055::ErrorCode BNO055::receiveBufferCleanup() {
    etl::array<uint8_t, 1> readBuff{};
    Fw::Buffer readBuffer(readBuff.data(), readBuff.size());
    
    U8 numOfTries = 0;
    while (this->read_out(0, readBuffer, readTimeout) != 0) {
      if (numOfTries == maxNumOfTries) {
        return ErrorCode::BUFFER_CLEAR_FAIL;
      }
      numOfTries++;
      Os::Task::delay(minWaitTime);
    }
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055 :: writeReg(U8 regAddr, U8 value) {
    constexpr U8 numOfRegs = 1;
    etl::array<U8, numOfRegs> chosenPageArr{value};
    Fw::Buffer chosenPageBuff(chosenPageArr.data(), chosenPageArr.size());

    ErrorCode writeStatus = writeRegs(regAddr, chosenPageBuff);

    return writeStatus;
  }

  BNO055::ErrorCode BNO055 :: readReg(U8 regAddr, U8& value) {
    constexpr U8 numOfRegs = 1;
    etl::array<U8, numOfRegs> readArr;
    Fw::Buffer readBuff(readArr.data(), readArr.size());
    
    ErrorCode readStatus = readRegs(regAddr, numOfRegs, readBuff);
    
    value = readArr.front();
    
    return readStatus;
  }

  BNO055::ErrorCode BNO055::writeRegsHandle(U8 regAddr, Fw::Buffer& buff) {
    ErrorCode cleanupStatus = receiveBufferCleanup();
    if (cleanupStatus != ErrorCode::OPERATION_SUCCESS) {
      return cleanupStatus;
    }

    ErrorCode sendStatus = sendWriteData(regAddr, etl::span<U8>(buff.getData(), buff.getSize()));
    if (sendStatus != ErrorCode::OPERATION_SUCCESS) {
      return sendStatus;
    }

    etl::span<U8> packetSpan(readPacket.data(), readPacket.size());
    auto receiveResult = receiveData(packetSpan);
    if (!receiveResult.has_value()) {
      return receiveResult.error();
    }
    U32 numOfReadBytes = receiveResult.value();

    etl::span<U8> receivedDataSpan(readPacket.data(), numOfReadBytes);

    ErrorCode responseStatus = handleWriteResponse(receivedDataSpan);
    
    return responseStatus;
  }

  BNO055::ErrorCode BNO055::writeRegs(U8 regAddr, Fw::Buffer& buff) {
    auto responseStatus = writeRegsHandle(regAddr, buff);

    auto commStatus = getCommStatus();
    if ((responseStatus == ErrorCode::COMMUNICATION_ERROR) && (commStatus == CommStatus::BUS_OVER_RUN_ERROR)) {
      responseStatus = writeRegsHandle(regAddr, buff);
    }

    return responseStatus;
  }

  BNO055::ErrorCode BNO055::readRegsHandle(U8 regAddr, size_t numOfRegs) {
    ErrorCode cleanupStatus = receiveBufferCleanup();
    if (cleanupStatus != ErrorCode::OPERATION_SUCCESS) {
      return cleanupStatus;
    }

    ErrorCode sendStatus = sendReadData(regAddr, numOfRegs);
    if (sendStatus != ErrorCode::OPERATION_SUCCESS) {
      return sendStatus;
    }

    etl::span<U8> receivePacketSpan(readPacket.data(), readPacket.size());
    auto receiveResult = receiveData(receivePacketSpan);
    if (!receiveResult.has_value()) {
      return receiveResult.error();
    }
    U32 numOfReadBytes = receiveResult.value();

    etl::span<U8> responseDataSpan(readPacket.data(), numOfReadBytes);
    ErrorCode responseStatus = handleReadResponse(responseDataSpan);
    
    return responseStatus;
  }

  BNO055::ErrorCode BNO055::readRegs(U8 regAddr, size_t numOfRegs, Fw::Buffer& buff) {
    auto responseStatus = readRegsHandle(regAddr, numOfRegs);

    auto commStatus = getCommStatus();
    if ((responseStatus == ErrorCode::COMMUNICATION_ERROR) && (commStatus == CommStatus::BUS_OVER_RUN_ERROR)) {
      responseStatus = readRegsHandle(regAddr, numOfRegs);
    }

    if (responseStatus != ErrorCode::OPERATION_SUCCESS) {
      return responseStatus;
    }
      
    auto ptrRegsBuff = buff.getData();
    auto ptrPacket = readPacket.data() + readSuccessResponseHeaderSize;
    for (size_t i = 0; i < buff.getSize(); i++) {
      *ptrRegsBuff = *ptrPacket;
      ptrRegsBuff++;
      ptrPacket++;
    }

    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::sendWriteData(U8 regAddr, etl::span<U8> buffer) {
    auto payloadSize = buffer.size_bytes();
    auto packetSize = sendHeaderSize;
    
    writePacket.at(0) = sendStartByte;
    writePacket.at(1) = sendWriteMode;
    writePacket.at(2) = regAddr;
    writePacket.at(3) = payloadSize;

    auto ptrPayload = writePacket.data() + packetSize;
    auto ptrBuff = buffer.data();
    for (U8 i = 0; i < payloadSize; i++) {
      *ptrPayload = *ptrBuff;
      ptrPayload++;
      ptrBuff++;
    }
    packetSize += payloadSize;
    
    Fw::Buffer packetBuffer(writePacket.data(), packetSize);
    Drv::UART::WriteStatus writeStatus = this->write_out(0, packetBuffer, writeTimeout);
    if (writeStatus != Drv::UART::WriteStatus::SUCCESS) {
      return ErrorCode::SEND_FAIL;
    }
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::sendReadData(U8 regAddr, size_t numOfRegs) {
    auto packetSize = sendHeaderSize;
    
    writePacket.at(0) = sendStartByte;
    writePacket.at(1) = sendReadMode;
    writePacket.at(2) = regAddr;
    writePacket.at(3) = numOfRegs;

    Fw::Buffer packetBuffer(writePacket.data(), packetSize);
    Drv::UART::WriteStatus writeStatus = this->write_out(0, packetBuffer, writeTimeout);
    if (writeStatus != Drv::UART::WriteStatus::SUCCESS) {
      return ErrorCode::SEND_FAIL;
    }
    return ErrorCode::OPERATION_SUCCESS;
  }

  etl::expected<size_t, BNO055::ErrorCode> BNO055::receiveData(etl::span<U8> buffer) {
    Fw::Buffer receiveBuffer(buffer.data(), buffer.size());

    U32 prevNumOfAvailableBytes = 0;
    U32 numOfAvailableBytes = 0;
    U8 numOfTries = 0;
    bool receiveStarted = false;

    while (true) {
      if (numOfTries == maxNumOfTries) {
        return etl::unexpected(ErrorCode::RECEIVE_FAIL);
      }
      numOfTries++;
      Os::Task::delay(minWaitTime);

      prevNumOfAvailableBytes = numOfAvailableBytes;
      numOfAvailableBytes = this->getNumOfBytes_out(0);
      
      if (receiveStarted) {
        bool receiveFinished = (prevNumOfAvailableBytes == numOfAvailableBytes);
        if (receiveFinished) {
          break;
        }
      }
      else {
        if (prevNumOfAvailableBytes != numOfAvailableBytes) {
          receiveStarted = true;
          numOfTries = 0;
        }
      }
    }

    U32 numOfReadBytes = this->read_out(0, receiveBuffer, readTimeout);

    return numOfReadBytes;
  }
    
  BNO055::ErrorCode BNO055::handleWriteResponse(etl::span<U8> buffer) {
    if (buffer.size_bytes() != ackResponseSize) {
      return ErrorCode::RESPONSE_SIZE_MISMATCH;
    }

    U8 header = buffer.front();
    bool isHeaderValid = (header == ackResponseHeader);
    if (!isHeaderValid) {
      return ErrorCode::RESPONSE_HEADER_MISMATCH;
    }
    commStatus = static_cast<CommStatus>(buffer.back());
    if (commStatus != CommStatus::WRITE_SUCCESS) {
      return ErrorCode::COMMUNICATION_ERROR;
    }
    return ErrorCode::OPERATION_SUCCESS;
  }
  
  BNO055::ErrorCode BNO055::handleReadResponse(etl::span<U8> buffer) {
    auto responseSize = buffer.size_bytes();
    if (responseSize < failureResponseSize) {
      return ErrorCode::RESPONSE_SIZE_MISMATCH;
    }
    
    U8 payloadLength = *(buffer.data() + 1);
    U8 header = buffer.front();

    bool isResponseSuccess = (header == readSuccessResponseHeader);
    bool isResponseFailure = (header == readFailureResponseHeader);

    if (isResponseSuccess) {
      if ((responseSize - readSuccessResponseHeaderSize) !=  payloadLength) {
        return ErrorCode::RESPONSE_SIZE_MISMATCH;
      }
      return ErrorCode::OPERATION_SUCCESS;
    }
    else if (isResponseFailure) {
      if (responseSize !=  failureResponseSize) {
        return ErrorCode::RESPONSE_SIZE_MISMATCH;
      }
      commStatus = static_cast<CommStatus>(buffer.back());
      return ErrorCode::COMMUNICATION_ERROR;
    }

    return ErrorCode::RESPONSE_HEADER_MISMATCH;
  }

  etl::expected<BNO055::EulerAngles, BNO055::ErrorCode> BNO055::getEulerAngles() {
    etl::array<uint8_t, eulerHrpDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readResult = readRegs(eulDataXLsbAddrPage0, eulerHrpDataSize, readBuffer);

    if (readResult != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readResult);
    }

    EulerAngles eulerAngles;
    eulerAngles.heading  = static_cast<float>(static_cast<I16>((readArr.at(1) << 8) | readArr.at(0))) / eulerDiv;
    eulerAngles.roll     = static_cast<float>(static_cast<I16>((readArr.at(3) << 8) | readArr.at(2)))  / eulerDiv;
    eulerAngles.pitch    = static_cast<float>(static_cast<I16>((readArr.at(5) << 8) | readArr.at(4)))  / eulerDiv;

    return eulerAngles;
  }

  etl::expected<BNO055::QuaternionAngles, BNO055::ErrorCode> BNO055::getQuaternionAngles() {
    etl::array<uint8_t, quaternionsDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readStatus = readRegs(quaDataWLsbAddrPage0, quaternionsDataSize, readBuffer);

    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readStatus);
    }

    QuaternionAngles quaternionAngles;
    quaternionAngles.w       = static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0)) / quatDiv;
    quaternionAngles.x       = static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2)) / quatDiv;
    quaternionAngles.y       = static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4)) / quatDiv;
    quaternionAngles.z       = static_cast<float>(static_cast<I16>(readArr.at(7) << 8) | readArr.at(6)) / quatDiv;

    return quaternionAngles;
  }

  etl::expected<BNO055::AccelData, BNO055::ErrorCode> BNO055::getAccelData() {
    etl::array<uint8_t, rawDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readStatus = readRegs(accDataXLsbAddrPage0, rawDataSize, readBuffer);

    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readStatus);
    }

    AccelData accelData;
    accelData.x       = static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0)) / accDiv;
    accelData.y       = static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2)) / accDiv;
    accelData.z       = static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4)) / accDiv;

    return accelData;
  }

  etl::expected<BNO055::GyroData, BNO055::ErrorCode> BNO055::getGyroData() {
    etl::array<uint8_t, rawDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readStatus = readRegs(gyrDataXLsbAddrPage0, rawDataSize, readBuffer);

    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readStatus);
    }

    GyroData gyroData;
    gyroData.x       = (static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0)) / gyroDiv) - GyroXOffset;
    gyroData.y       = (static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2)) / gyroDiv) - GyroYOffset;
    gyroData.z       = (static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4)) / gyroDiv) - GyroZOffset;

    return gyroData;
  }

  etl::expected<BNO055::MagnData, BNO055::ErrorCode> BNO055::getMagneticData() {
    etl::array<uint8_t, rawDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readStatus = readRegs(magDataXLsbAddrPage0, rawDataSize, readBuffer);

    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readStatus);
    }

    MagnData magnData;
    magnData.x       = static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0)) / magDiv;
    magnData.y       = static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2)) / magDiv;
    magnData.z       = static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4)) / magDiv;

    return magnData;
  }

  etl::expected<BNO055::IMUData, BNO055::ErrorCode> BNO055::getIMUData() {
    etl::array<uint8_t, fullRawDataSize> readArr{0};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    ErrorCode readStatus = readRegs(accDataXLsbAddrPage0, fullRawDataSize, readBuffer);

    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      return etl::unexpected(readStatus);
    }

    IMUData imuData;
    imuData.accelData.x = static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0))  / accDiv;
    imuData.accelData.y = static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2))  / accDiv;
    imuData.accelData.z = static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4))  / accDiv;

    imuData.magnData.x = static_cast<float>(static_cast<I16>(readArr.at(7) << 8) | readArr.at(6))   / magDiv;
    imuData.magnData.y = static_cast<float>(static_cast<I16>(readArr.at(9) << 8) | readArr.at(8))   / magDiv;
    imuData.magnData.z = static_cast<float>(static_cast<I16>(readArr.at(11) << 8) | readArr.at(10)) / magDiv;

    imuData.gyroData.x = static_cast<float>(static_cast<I16>(readArr.at(13) << 8) | readArr.at(12)) / gyroDiv;
    imuData.gyroData.y = static_cast<float>(static_cast<I16>(readArr.at(15) << 8) | readArr.at(14)) / gyroDiv;
    imuData.gyroData.z = static_cast<float>(static_cast<I16>(readArr.at(17) << 8) | readArr.at(16)) / gyroDiv;

    return imuData;
  }

  etl::expected<BNO055::CalibrationStatus, BNO055::ErrorCode> BNO055::getCalibrationStatus() {
    U8 calibStatResult = 0;
    ErrorCode readStatus = readReg(calibStatAddrPage0, calibStatResult);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return etl::unexpected(readStatus);
    }
    CalibrationStatus calibration;
    calibration.magCalibStatus = ((0b11 & calibStatResult) == 0b11);
    calibration.accCalibStatus = ((0b11'00 & calibStatResult) == 0b11'00);
    calibration.gyrCalibStatus = ((0b11'0000 & calibStatResult) == 0b11'0000);
    calibration.sysCalibStatus = ((0b11'000000 & calibStatResult) == 0b11'000000);

    return calibration;
  }

  void BNO055::errorReport(ErrorCode errorCode) {
    switch (errorCode) {
        case ErrorCode::RESPONSE_HEADER_MISMATCH:
            this->log_WARNING_HI_ResponseHeaderMismatchError();
        break;

        case ErrorCode::RESPONSE_SIZE_MISMATCH:
            this->log_WARNING_HI_ResponseSizeMismatchError();
        break;

        case ErrorCode::RECEIVE_FAIL:
            this->log_WARNING_HI_ReceiveFailError();
        break;

        case ErrorCode::SEND_FAIL:
            this->log_WARNING_HI_SendFailError();
        break;

        case ErrorCode::BUFFER_CLEAR_FAIL:
            this->log_WARNING_HI_BufferClearFailError();
        break;

        case ErrorCode::RESET_FAIL:
            this->log_WARNING_HI_ResetFailError();
        break;

        case ErrorCode::INIT_FAIL:
            this->log_WARNING_HI_NotInited();
        break;

        case ErrorCode::SELF_TEST_FAIL:
            this->log_WARNING_HI_SelfTestError();
        break;

        case ErrorCode::CALIBRATION_UNFINISHED:
            this->log_WARNING_LO_CalibrationUnfinished();
        break;
        
        case ErrorCode::COMMUNICATION_ERROR:
          commStatusReport(commStatus);
        break;
        
        default:
            this->log_WARNING_HI_UnknownError();
        break;
    }
  }

  void BNO055::commStatusReport(CommStatus commStatus) {
    switch (commStatus) {
      case CommStatus::READ_FAIL:
      this->log_WARNING_HI_ReadFailError();
      break;

      case CommStatus::WRITE_FAIL:
          this->log_WARNING_HI_WriteFailError();
      break;

      case CommStatus::REGMAP_INVALID_ADDRESS:
          this->log_WARNING_HI_RegmapInvalidAddressError();
      break;

      case CommStatus::REGMAP_WRITE_DISABLED:
          this->log_WARNING_HI_RegmapWriteDisabledError();
      break;

      case CommStatus::WRONG_START_BYTE:
          this->log_WARNING_HI_WrongStartByteError();
      break;

      case CommStatus::BUS_OVER_RUN_ERROR:
          // Bus overrun error isn't reported each time it occurs because there is no solution for this problem now
          busOverrunErrorCnt++;
          if (busOverrunErrorCnt % busOverrunErrorReportCnt == 0) {
            this->log_WARNING_HI_BusOverrunError(busOverrunErrorCnt);
          }
      break;

      case CommStatus::MAX_LENGTH_ERROR:
          this->log_WARNING_HI_MaxLengthError();
      break;

      case CommStatus::MIN_LENGTH_ERROR:
          this->log_WARNING_HI_MinLengthError();
      break;

      case CommStatus::RECEIVE_CHARACTER_TIMEOUT:
          this->log_WARNING_HI_ReceiveCharacterTimeoutError();
      break;

      default:
        this->log_WARNING_HI_UnknownError();
      break;
    }
  }

  BNO055::ErrorCode BNO055::isCalibrationFinished() {
    U8 calibStatResult = 0;
    ErrorCode readStatus = readReg(calibStatAddrPage0, calibStatResult);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }
    if (calibStatResult != calibStatExpectedResult) {
      return ErrorCode::CALIBRATION_UNFINISHED;
    }
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setCalibrationProfile(Fw::Buffer& buff) {
    // 1. switch to config mode
    // get current operation mode
    U8 currentOperationMode;
    ErrorCode readStatus = readReg(operationModeAddrPage0, currentOperationMode);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }

    // if not in config mode, set operation mode
    if (currentOperationMode != configMode) {
      ErrorCode status = setConfigMode();
      if (status != ErrorCode::OPERATION_SUCCESS) {
        errorReport(status);
        return status;
      }
    }

    // 2. write sensor offsets and sensor radius registers
    ErrorCode writeResult = writeRegs(accOffsetXLsbAddrPage0, buff);
    if (writeResult != ErrorCode::OPERATION_SUCCESS) {
      return writeResult;
    }

    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::readCalibrationProfile() {
    // 1. switch to config mode
    // get current operation mode
    U8 currentOperationMode;
    ErrorCode readStatus = readReg(operationModeAddrPage0, currentOperationMode);
    if (readStatus != ErrorCode::OPERATION_SUCCESS) {
      errorReport(readStatus);
      return readStatus;
    }

    // if not in config mode, set operation mode
    if (currentOperationMode != configMode) {
      ErrorCode status = setConfigMode();
      if (status != ErrorCode::OPERATION_SUCCESS) {
        errorReport(status);
        return status;
      }
    }

    // 2. read sensor offsets and sensor radius registers
    Fw::Buffer readBuffer(newCalibArr.data(), newCalibArr.size());
    ErrorCode readResult = readRegs(accOffsetXLsbAddrPage0, numOfCalibrationRegs, readBuffer);
    if (readResult != ErrorCode::OPERATION_SUCCESS) {
      return readResult;
    }

    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setConfigMode() {
    ErrorCode operationStatus = setOperatingMode(OperatingMode::ConfigMode);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      return operationStatus;
    }
    Os::Task::delay(modeSwitchTime);
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setNDOFMode() {
    ErrorCode operationStatus = setOperatingMode(OperatingMode::NDOF);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      return operationStatus;
    }
    Os::Task::delay(modeSwitchTime);
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::ErrorCode BNO055::setAMGMode() {
    ErrorCode operationStatus = setOperatingMode(OperatingMode::AMG);
    if (operationStatus != ErrorCode::OPERATION_SUCCESS) {
      return operationStatus;
    }
    Os::Task::delay(modeSwitchTime);
    return ErrorCode::OPERATION_SUCCESS;
  }

  BNO055::CommStatus BNO055::getCommStatus() {
    return commStatus;
  }

  BNO055::ErrorCode Components::BNO055::calibrateGyroOffsets() {
    uint32_t counter = 0;
    constexpr uint32_t numOfCalibrationDataOffsets = 1000;
    for (uint32_t i = 0; i < numOfCalibrationDataOffsets; i++) {
      etl::array<uint8_t, rawDataSize> readArr{0};
      Fw::Buffer readBuffer(readArr.data(), readArr.size());
      ErrorCode readStatus = readRegs(gyrDataXLsbAddrPage0, rawDataSize, readBuffer);

      if (readStatus == ErrorCode::OPERATION_SUCCESS) {
        GyroData gyroData;
        gyroData.x       = (static_cast<float>(static_cast<I16>(readArr.at(1) << 8) | readArr.at(0)) / gyroDiv);
        gyroData.y       = (static_cast<float>(static_cast<I16>(readArr.at(3) << 8) | readArr.at(2)) / gyroDiv);
        gyroData.z       = (static_cast<float>(static_cast<I16>(readArr.at(5) << 8) | readArr.at(4)) / gyroDiv);

        counter++;
        GyroXOffset += gyroData.x;
        GyroYOffset += gyroData.y;
        GyroZOffset += gyroData.z;
      }
    }
    
    GyroXOffset /= counter;
    GyroYOffset /= counter;
    GyroZOffset /= counter;

    return ErrorCode::OPERATION_SUCCESS;
  }
}
