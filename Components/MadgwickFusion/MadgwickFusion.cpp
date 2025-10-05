// ======================================================================
// \title  MadgwickFusion.cpp
// \author user
// \brief  cpp file for MadgwickFusion component implementation class
// ======================================================================

#include "Components/MadgwickFusion/MadgwickFusion.hpp"
#include "FpConfig.hpp"
#include "Fusion.h"
#include "etl/string.h"
#include "etl/algorithm.h"
#include "etl/to_string.h"
#include <cstdio>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  MadgwickFusion ::
    MadgwickFusion(const char* const compName) :
      MadgwickFusionComponentBase(compName)
  {

  }

  MadgwickFusion ::
    ~MadgwickFusion()
  {

  }

  void MadgwickFusion ::configure() {
    advancedExampleInit();
  }
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  // Function to convert a float to a string with 3 decimal places

  void MadgwickFusion ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    advancedExampleRun();
  }

  bool MadgwickFusion::sendDataFor3DVisualization(const FusionQuaternion& quaternion) {
    etl::string<64> my_string;

    my_string.clear();
    my_string.append("S:");
    etl::to_string(quaternion.element.w, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.x, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.y, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.z, my_string, etl::format_spec().precision(3), true);
    my_string.append("\n\r");
    
    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), my_string.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 2);
    
    if (writeStatus == Drv::UART::WriteStatus::FAILED) {
      this->log_WARNING_HI_DataSendTimeout();
    }

    return true;
  }

  bool MadgwickFusion::sendDataFor2DVisualization(const FusionEuler& euler) {

  }

  bool MadgwickFusion::sendRawDataForCalibration(const FusionVector& accel, const FusionVector& gyro, const FusionVector& magn) {
    etl::string<128> my_string;

    my_string.clear();

    my_string.append("Raw:");
    etl::to_string(static_cast<I32>(accel.axis.x), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(accel.axis.y), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(accel.axis.z), my_string, true);
    my_string.append(",");

    etl::to_string(static_cast<I32>(gyro.axis.x), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(gyro.axis.y), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(gyro.axis.z), my_string, true);
    my_string.append(",");

    etl::to_string(static_cast<I32>(magn.axis.x * 10), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(magn.axis.y* 10), my_string, true);
    my_string.append(",");
    etl::to_string(static_cast<I32>(magn.axis.z* 10), my_string, true);
    my_string.append("\r\n");

    my_string.append("Uni:");
    etl::to_string(accel.axis.x, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(accel.axis.y, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(accel.axis.z, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");

    etl::to_string(gyro.axis.x, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(gyro.axis.y, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(gyro.axis.z, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");

    etl::to_string(magn.axis.x, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(magn.axis.y, my_string, etl::format_spec().precision(3), true);
    my_string.append(",");
    etl::to_string(magn.axis.z, my_string, etl::format_spec().precision(3), true);
    my_string.append("\r\n");

    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), my_string.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 2);
    
    if (writeStatus == Drv::UART::WriteStatus::FAILED) {
      this->log_WARNING_HI_DataSendTimeout();
    }

    return true;
  }

  void MadgwickFusion::simpleExampleInit() {
    FusionAhrsInitialise(&ahrs);
  }

  void MadgwickFusion::simpleExampleRun() {
    this->getIMUData_out(0, imuData);
    if (imuData.getAccelValid() && imuData.getGyroValid()) {
      const FusionVector accelVector {imuData.getAccel().getX() / g, imuData.getAccel().getY() / g, imuData.getAccel().getZ() / g};
      const FusionVector gyroVector  {imuData.getGyro().getX(), imuData.getGyro().getY(), imuData.getGyro().getZ()};
      FusionAhrsUpdateNoMagnetometer(&ahrs, gyroVector, accelVector, samplePeriod);
      const FusionQuaternion quaternion = FusionAhrsGetQuaternion(&ahrs);

      bool isDataSent = sendDataFor3DVisualization(quaternion);
    }
    else {
      this->log_WARNING_HI_DataInvalid();
    }
  }
  
  void MadgwickFusion::advancedExampleInit() {
    FusionOffsetInitialise(&offset, SAMPLE_RATE);
    FusionAhrsInitialise(&ahrs);

    // Set AHRS algorithm settings
    const FusionAhrsSettings settings = {
            .convention = FusionConventionNwu,
            .gain = 0.5f,
            .gyroscopeRange = 2000.0f, /* replace this with actual gyroscope range in degrees/s */
            .accelerationRejection = 10.0f,
            .magneticRejection = 10.0f,
            .recoveryTriggerPeriod = 5 * SAMPLE_RATE, /* 5 seconds */
    };
    FusionAhrsSetSettings(&ahrs, &settings);
  }
  
  void MadgwickFusion::advancedExampleRun() {
    // Acquire latest sensor data
    this->getIMUData_out(0, imuData);
    
    FusionVector accelerometer = {0.0f, 0.0f, 1.0f}; // replace this with actual accelerometer data in g
    FusionVector gyroscope = {0.0f, 0.0f, 0.0f}; // replace this with actual gyroscope data in degrees/s
    FusionVector magnetometer = {1.0f, 0.0f, 0.0f}; // replace this with actual magnetometer data in arbitrary units

    if (imuData.getAccelValid() && imuData.getGyroValid() && imuData.getMagnValid()) {
      const FusionVector accelVector {imuData.getAccel().getX() / g, imuData.getAccel().getY() / g, imuData.getAccel().getZ() / g};
      const FusionVector gyroVector  {imuData.getGyro().getX(), imuData.getGyro().getY(), imuData.getGyro().getZ()};
      const FusionVector magnVector  {imuData.getMagn().getX(), imuData.getMagn().getY(), imuData.getMagn().getZ()};

      accelerometer.axis.x = accelVector.axis.x;
      accelerometer.axis.y = accelVector.axis.y;
      accelerometer.axis.z = accelVector.axis.z;

      gyroscope.axis.x     = gyroVector.axis.x;
      gyroscope.axis.y     = gyroVector.axis.y;
      gyroscope.axis.z     = gyroVector.axis.z;

      magnetometer.axis.x =  magnVector.axis.x;
      magnetometer.axis.y = -magnVector.axis.y; // "-" is added, so that accelgyro axis and magnetometer axis are aligned
      magnetometer.axis.z = -magnVector.axis.z; // "-" is added, so that accelgyro axis and magnetometer axis are aligned

      // Apply calibration
      gyroscope = FusionCalibrationInertial(gyroscope, gyroscopeMisalignment, gyroscopeSensitivity, gyroscopeOffset);
      accelerometer = FusionCalibrationInertial(accelerometer, accelerometerMisalignment, accelerometerSensitivity, accelerometerOffset);
      magnetometer = FusionCalibrationMagnetic(magnetometer, softIronMatrix, hardIronOffset);

      // Update gyroscope offset correction algorithm
      gyroscope = FusionOffsetUpdate(&offset, gyroscope);

      const float deltaTime = 0.01;

      // Update gyroscope AHRS algorithm
      FusionAhrsUpdate(&ahrs, gyroscope, accelerometer, magnetometer, deltaTime);

      // Print algorithm outputs
      const FusionQuaternion quaternion = FusionAhrsGetQuaternion(&ahrs);
      const FusionEuler euler = FusionQuaternionToEuler(quaternion);
      const FusionVector earth = FusionAhrsGetEarthAcceleration(&ahrs);
      // bool isDataSent = sendDataFor3DVisualization(quaternion);
      FusionAhrsInternalStates FusionAhrsInternalStates = FusionAhrsGetInternalStates(&ahrs);
      FusionAhrsFlags fusionAhrsFlags = FusionAhrsGetFlags(&ahrs);
      sendDataForDebug(quaternion, FusionAhrsInternalStates, fusionAhrsFlags);
    }
    else {
      this->log_WARNING_HI_DataInvalid();
    }

  }

  bool MadgwickFusion::sendDataForDebug(const FusionQuaternion& quaternion, const FusionAhrsInternalStates& ahrsInternalStates, const FusionAhrsFlags& ahrsFlags) {

    my_string.clear();
    my_string.append("S:");
    etl::to_string(quaternion.element.w, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.x, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.y, my_string, etl::format_spec().precision(3), true);
    my_string.append("; ");
    etl::to_string(quaternion.element.z, my_string, etl::format_spec().precision(3), true);
    my_string.append("\n\r");
    
    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), my_string.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 2);
    
    if (writeStatus == Drv::UART::WriteStatus::FAILED) {
      this->log_WARNING_HI_DataSendTimeout();
    }

    return true;
  }


}
