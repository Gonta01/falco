// ======================================================================
// \title  STS21.cpp
// \author user
// \brief  cpp file for STS21 component implementation class
// ======================================================================

#include "Components/STS21/STS21.hpp"
#include "FpConfig.hpp"
#include "etl/private/bitset_new.h"
#include "etl/expected.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  STS21 ::
    STS21(const char* const compName) :
      STS21ComponentBase(compName),
      inited(false)
  {

  }

  STS21 ::
    ~STS21()
  {

  }

  void STS21::configure(F32 k, F32 b) {
    inited = true;

    calibrCoeffK = k;
    calibrCoeffB = b;

    auto resolution = STS21::MeasurementResolution::Resolution_14bit;
    // set resolution bits 0, 7
    etl::bitset<userRegisterLen, uint8_t> userRegisterBitset;
    switch (resolution) {
        case STS21::MeasurementResolution::Resolution_14bit:
            userRegisterBitset.reset(MeasurementResolutionOneBit);
            userRegisterBitset.reset(MeasurementResolutionTwoBit);
            break;
        case STS21::MeasurementResolution::Resolution_13bit:
            userRegisterBitset.reset(MeasurementResolutionOneBit);
            userRegisterBitset.set(MeasurementResolutionTwoBit);
            break;
        case STS21::MeasurementResolution::Resolution_12bit:
            userRegisterBitset.set(MeasurementResolutionOneBit);
            userRegisterBitset.reset(MeasurementResolutionTwoBit);
            break;
        case STS21::MeasurementResolution::Resolution_11bit:
            userRegisterBitset.set(MeasurementResolutionOneBit);
            userRegisterBitset.set(MeasurementResolutionTwoBit);
            break;

        default:
            inited = false;
            this->log_WARNING_HI_WrongResolutionConfiguration();
            break;
    }

    userRegisterBitset.reset(EndOfBatteryBit);
    userRegisterBitset.reset(EnableOnChipHeater);
    userRegisterBitset.set(DisableOTPReloadBit);

    // write user register
    if (uint8_t const userRegister = userRegisterBitset.value<uint8_t>(); !writeUserReg(userRegister)) {
        inited = false;
        this->log_WARNING_HI_CommunicationError();
    }

  }

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------
Drv::Sensor::Status STS21::getTemperature_handler(NATIVE_INT_TYPE portNum, F32& temperature) {
    // get temperature value
    auto triggerResult = triggerMeasurement();
    if (triggerResult.error() != STS21::ErrorCode::NoError) {
        errorReport(triggerResult.error());
        return Drv::Sensor::Status::FAILED;
    }
    Os::Task::delay(measurementMaxTime);
    auto measurementResult = getMeasurement();

    if (measurementResult.has_value() == false) {
        errorReport(measurementResult.error());
        return Drv::Sensor::Status::FAILED;
    }

    temperature = calibrCoeffK * measurementResult.value() + calibrCoeffB;

    return Drv::Sensor::Status::SUCCESS;
}

void STS21::errorReport(STS21::ErrorCode errorCode) {
    switch (errorCode) {
        case STS21::ErrorCode::ICCommError:
            this->log_WARNING_HI_CommunicationError();
        break;
        case STS21::ErrorCode::NotInited:
            this->log_WARNING_HI_NotInited();
        break;
        case STS21::ErrorCode::WrongResolutionConfiguration:
            this->log_WARNING_HI_WrongResolutionConfiguration();
        break;
        default:
            this->log_WARNING_HI_UnknownError();
        break;
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------
etl::expected<float, STS21::ErrorCode> STS21::getTemperatureBlocking() {
  if (!inited) {
      return etl::unexpected(STS21::ErrorCode::NotInited);
  }
  // Trigger T measurement
  STS21::ErrorCode const errorCode = triggerMeasurement().error();
  if (errorCode == STS21::ErrorCode::NoError) {
      // Wait for ACK after measurement is completed
      while (true) {
          // Get measured data
          if (auto res = getMeasurement(); res.has_value()) {
              return res.value();
          }
      }
  }
  return etl::unexpected(errorCode);
}

etl::expected<float, STS21::ErrorCode> STS21::getMeasurement() {
  if (!inited) {
      return etl::unexpected(STS21::ErrorCode::NotInited);
  }
  // get measured data
  if (uint16_t measuredData = 0; read(&measuredData)) {
      // convert to Celsius
      constexpr float STS21_COEFFICIENT_1 = -46.85;
      constexpr float STS21_COEFFICIENT_2 = 175.72;
      constexpr float STS21_TEMPERATURE_DATA_SIZE = 0x10000; // 2^numOfBits (2^16, no matter which resolution)
      
      return static_cast<float>(STS21_COEFFICIENT_1 +
              STS21_COEFFICIENT_2 * (static_cast<float>(measuredData) / STS21_TEMPERATURE_DATA_SIZE));
  }
  return etl::unexpected(STS21::ErrorCode::ICCommError);
}

etl::expected<void, STS21::ErrorCode> STS21::triggerMeasurement() {
  if (!inited) {
      return etl::unexpected(STS21::ErrorCode::NotInited);
  }

  // Trigger T measurement, no hold master
  if (!writeCmd(STS21_COMMAND::TriggerMeasurement)) {
      return etl::unexpected(STS21::ErrorCode::ICCommError);
  }
  return etl::unexpected(STS21::ErrorCode::NoError);
}

//*********************************************************
//    Helper READ / WRITE FUNCTIONS:
bool STS21::writeUserReg(uint8_t regVal) {
  etl::array<uint8_t, 2> arr = {static_cast<uint8_t>(STS21_COMMAND::WriteUserReg), regVal};
  Fw::Buffer buffer(arr.data(), arr.size());
  
  bool writeResult = (write_out(0, STS21Addr, buffer, writeTimeout) == Drv::I2C::Status::SUCCESS);

  return writeResult;
}

bool STS21::writeCmd(STS21_COMMAND cmd) {
    etl::array<uint8_t, 1> arr = {static_cast<uint8_t>(cmd)};
    
    Fw::Buffer buffer(arr.data(), arr.size());
    
    bool writeResult = (write_out(0, STS21Addr, buffer, writeTimeout) == Drv::I2C::Status::SUCCESS);
    
    return writeResult;
}

bool STS21::read(uint16_t *data) {
    bool readSuccess = false;
    constexpr uint8_t highByteOffset = 8;

    etl::array<uint8_t, 3> readBuff{};
    Fw::Buffer buffer(readBuff.data(), readBuff.size());

    if (read_out(0, STS21Addr, buffer, readTimeout) == Drv::I2C::Status::SUCCESS) {
        auto bufferData = buffer.getData();
        for (int16_t i = 0; i < readBuff.size(); i++) {
            readBuff.at(i) = *bufferData;
            bufferData++;
        }
        if (isDataBuffValid(readBuff)) {
            U8 readDataLSBMask = 0b1111'1100; 
            *data = etl::rotate_left<uint16_t>(readBuff[0], highByteOffset) + (readDataLSBMask & readBuff[1]);
            readSuccess = true;
        }
    }

    return readSuccess;
}

bool STS21::isDataBuffValid(const etl::span<uint8_t> &dataBuff) {
    uint8_t crc = 0;
    //calculates 8-Bit checksum with given polynomial
    for (uint8_t const &byteCtr: dataBuff) {
        crc ^= byteCtr;
        constexpr uint8_t loopCnt = 8;
        for (uint8_t bit = loopCnt; bit > 0; --bit) {
            constexpr uint8_t crcMask = 0x80;
            if ((crc & crcMask) != 0) {
                crc = etl::rotate_left<uint16_t>(crc) ^ polynomial;
            } else {
                crc = etl::rotate_left<uint16_t>(crc);
            }
        }
    }
    return (crc == 0);
}

}
