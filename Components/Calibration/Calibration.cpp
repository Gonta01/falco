// ======================================================================
// \title  Calibration.cpp
// \author user
// \brief  cpp file for Calibration component implementation class
// ======================================================================

#include "Components/Calibration/Calibration.hpp"
#include "FpConfig.hpp"
#include "etl/to_string.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Calibration ::
    Calibration(const char* const compName) :
      CalibrationComponentBase(compName)
  {

  }

  Calibration ::
    ~Calibration()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Calibration ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    switch(mode) {
      case CalibrationMode::FREE_IMU: {
        useFreeIMUCalibration();
        break;
      }

      case CalibrationMode::MAG_CAL: {
        useMagCalCalibration();
        break;
      }
      default: {
        break;
      }
    }
  }

bool Calibration::sendRawDataForCalibrationForFreeIMU(Drv::IMU::CalibrationData& rawData) {
    etl::array<U8, 20> rawDataBuffer;
    rawDataBuffer.at(0) = (rawData.getAccelX() & 0xFF);
    rawDataBuffer.at(1) = (rawData.getAccelX() >> 8) & 0xFF;

    rawDataBuffer.at(2) = (rawData.getAccelY() & 0xFF);
    rawDataBuffer.at(3) = (rawData.getAccelY() >> 8) & 0xFF;

    rawDataBuffer.at(4) = (rawData.getAccelZ() & 0xFF);
    rawDataBuffer.at(5) = (rawData.getAccelZ() >> 8) & 0xFF;

    rawDataBuffer.at(6) = (rawData.getGyroX() & 0xFF);
    rawDataBuffer.at(7) = (rawData.getGyroX() >> 8) & 0xFF;

    rawDataBuffer.at(8) = (rawData.getGyroY() & 0xFF);
    rawDataBuffer.at(9) = (rawData.getGyroY() >> 8) & 0xFF;

    rawDataBuffer.at(10) = (rawData.getGyroZ() & 0xFF);
    rawDataBuffer.at(11) = (rawData.getGyroZ() >> 8) & 0xFF;
    
    rawDataBuffer.at(12) = (rawData.getMagnX() & 0xFF);
    rawDataBuffer.at(13) = (rawData.getMagnX() >> 8) & 0xFF;

    rawDataBuffer.at(14) = (rawData.getMagnY() & 0xFF);
    rawDataBuffer.at(15) = (rawData.getMagnY() >> 8) & 0xFF;

    rawDataBuffer.at(16) = (rawData.getMagnZ() & 0xFF);
    rawDataBuffer.at(17) = (rawData.getMagnZ() >> 8) & 0xFF;

    rawDataBuffer.at(18) = '\r';
    rawDataBuffer.at(19) = '\n';
    Fw::Buffer fwBuffer(rawDataBuffer.data(), rawDataBuffer.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 10);

    return true;
  }

  bool Calibration::sendVersionAnswerForFreeIMU() {
    etl::string<128> my_string;
    my_string.clear();

    my_string.append("Falco v1");
    my_string.append("\n");
    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), my_string.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 10);
    
    return true;
  }

  void Calibration::useFreeIMUCalibration() {
    if (isSamplingStarted) {
     this->getData_out(0, imuData);
     calibrationData = imuData.getCalibration();
     sendRawDataForCalibrationForFreeIMU(calibrationData);
     samplingCnt++;
     if (samplingCnt == maxCount) {
        samplingCnt = 0;
        isSamplingStarted = false;
     }
   }

    etl::string<8> my_string = "";
    my_string.clear();

    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), 1);
    U32 numOfBytes = this->read_out(0, fwBuffer, 10);
    if (numOfBytes > 0) {
      constexpr uint8_t versionCmd = 'v';
      if (*my_string.begin() == versionCmd) {
        sendVersionAnswerForFreeIMU();
      }
      constexpr uint8_t sendRawDataCmd = 'b';
      if (*my_string.begin() == sendRawDataCmd) {
        etl::string<8> count_string = "";
        count_string.clear();
        Fw::Buffer countBuffer(reinterpret_cast<U8*>(count_string.data()), 1);
        constexpr U32 maxAttempts = 5;
        U32 numOfAttempts = 0;
        while (true) {
          if (numOfAttempts >= maxAttempts) {
            break;
          }
          U32 numOfReadBytes = this->read_out(0, countBuffer, 10);
          if (numOfReadBytes > 0) {
            maxCount = *count_string.begin();
            break;
          }
          numOfAttempts++;
        }
        isSamplingStarted = true;
      }
    }
  }

  void Calibration::useMagCalCalibration() {
    calibrationMessage.clear();
    calibrationMessage.append("Raw:");
    
    this->getData_out(0, imuData);
    calibrationData = imuData.getCalibration();

    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(0, calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(calibrationData.getMagnX(), calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(calibrationData.getMagnY(), calibrationMessage, etl::format_spec(), true);
    calibrationMessage.append(",");
    etl::to_string(calibrationData.getMagnZ(), calibrationMessage, etl::format_spec(), true);                              
    calibrationMessage.append("\n\r");

    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(calibrationMessage.data()), calibrationMessage.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 10);
  }

  bool Calibration::sendRawDataForCalibrationForMagCal(Drv::IMU::CalibrationData& rawData) {
    etl::array<U8, 20> rawDataBuffer;
    rawDataBuffer.at(0) = (rawData.getAccelX() & 0xFF);
    rawDataBuffer.at(1) = (rawData.getAccelX() >> 8) & 0xFF;

    rawDataBuffer.at(2) = (rawData.getAccelY() & 0xFF);
    rawDataBuffer.at(3) = (rawData.getAccelY() >> 8) & 0xFF;

    rawDataBuffer.at(4) = (rawData.getAccelZ() & 0xFF);
    rawDataBuffer.at(5) = (rawData.getAccelZ() >> 8) & 0xFF;

    rawDataBuffer.at(6) = (rawData.getGyroX() & 0xFF);
    rawDataBuffer.at(7) = (rawData.getGyroX() >> 8) & 0xFF;

    rawDataBuffer.at(8) = (rawData.getGyroY() & 0xFF);
    rawDataBuffer.at(9) = (rawData.getGyroY() >> 8) & 0xFF;

    rawDataBuffer.at(10) = (rawData.getGyroZ() & 0xFF);
    rawDataBuffer.at(11) = (rawData.getGyroZ() >> 8) & 0xFF;
    
    rawDataBuffer.at(12) = (rawData.getMagnX() & 0xFF);
    rawDataBuffer.at(13) = (rawData.getMagnX() >> 8) & 0xFF;

    rawDataBuffer.at(14) = (rawData.getMagnY() & 0xFF);
    rawDataBuffer.at(15) = (rawData.getMagnY() >> 8) & 0xFF;

    rawDataBuffer.at(16) = (rawData.getMagnZ() & 0xFF);
    rawDataBuffer.at(17) = (rawData.getMagnZ() >> 8) & 0xFF;

    rawDataBuffer.at(18) = '\r';
    rawDataBuffer.at(19) = '\n';
    Fw::Buffer fwBuffer(rawDataBuffer.data(), rawDataBuffer.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 10);
    return true;
  }

}
