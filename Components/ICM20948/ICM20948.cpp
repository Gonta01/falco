// ======================================================================
// \title  ICM20948.cpp
// \author user
// \brief  cpp file for ICM20948 component implementation class
// ======================================================================

#include "Components/ICM20948/ICM20948.hpp"
#include "ICM20948Interface.hpp"
#include "FpConfig.hpp"
#include "etl/private/bitset_new.h"
#include <cstdint>
#include <etl/array.h>
#include <etl/expected.h>
#include <etl/span.h>
#include <etl/vector.h>
#include <etl/memory.h>

namespace Components {
  using ErrorCode = Interfaces::ICM20948::ErrorCode;
  using AccelXYZ = Interfaces::ICM20948::AccelData;
  using GyroXYZ = Interfaces::ICM20948::GyroData;
  using MagnXYZ = Interfaces::ICM20948::MagnData;
  using QuatQ0Q1Q2Q3 = Interfaces::ICM20948::Quaternion;

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ICM20948 ::
    ICM20948(const char* const compName) :
      ICM20948ComponentBase(compName)
  {

  }

  ICM20948 ::
    ~ICM20948()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  Drv::IMU::Status ICM20948 ::
    pollQuatData_handler(NATIVE_INT_TYPE portNum)
  {
    if (retrieveQuaternionData == false) {
        return Drv::IMU::Status::FAILED;
    }

    bool pollingSuccess = pollDMPData();
    if (pollingSuccess == false) {
        return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948 ::
    pollRawData_handler(NATIVE_INT_TYPE portNum)
  {
    if (retrieveQuaternionData == true) {
        return Drv::IMU::Status::FAILED;
    }

    bool pollingSuccess = pollRawData();
    if (pollingSuccess == false) {
      return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948::getAccelData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::AccelData& accelData
    )
  {
    if (retrieveQuaternionData) {
        return Drv::IMU::Status::FAILED;
    }

    auto accelDataResult = getAccelData();

    if (accelDataResult.has_value()) {
      auto retrievedAccelData = accelDataResult.value();
      accelData.setX(retrievedAccelData.x);
      accelData.setY(retrievedAccelData.y);
      accelData.setZ(retrievedAccelData.z);
    } else {
      errorReport(accelDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948::getGyroData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::GyroData& gyroData
    )
  {
    if (retrieveQuaternionData) {
        return Drv::IMU::Status::FAILED;
    }

    auto gyroDataResult = getGyroData();

    if (gyroDataResult.has_value()) {
      auto retrievedGyroData = gyroDataResult.value();
      gyroData.setX(retrievedGyroData.x);
      gyroData.setY(retrievedGyroData.y);
      gyroData.setZ(retrievedGyroData.z);
    } else {
      errorReport(gyroDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948::getMagnData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::MagnData& magnData
    )
  {
    if (retrieveQuaternionData) {
        return Drv::IMU::Status::FAILED;
    }
    auto magneticDataResult =  getMagneticData();

    if (magneticDataResult.has_value()) {
      auto retrievedMagneticData = magneticDataResult.value();
      magnData.setX(retrievedMagneticData.x);
      magnData.setY(retrievedMagneticData.y);
      magnData.setZ(retrievedMagneticData.z);
    } else {
      errorReport(magneticDataResult.error());
      return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948::getQuatData_handler(
        NATIVE_INT_TYPE portNum,
        Drv::IMU::QuatData& quatData
    )
  {
    if (retrieveQuaternionData == false) {
        return Drv::IMU::Status::FAILED;
    }

    auto quaternionDataResult = getQuatData();
    if(quaternionDataResult.has_value())
    {
        auto retrivedQuaternionData = quaternionDataResult.value();
        quatData.setQ0(retrivedQuaternionData.Q0);
        quatData.setQ1(retrivedQuaternionData.Q1);
        quatData.setQ2(retrivedQuaternionData.Q2);
        quatData.setQ3(retrivedQuaternionData.Q3);
    } else {
        errorReport(quaternionDataResult.error());
        return Drv::IMU::Status::FAILED;
    }

    return Drv::IMU::Status::SUCCESS;
  }

  Drv::IMU::Status ICM20948::getCalibrationData_handler(NATIVE_INT_TYPE portNum, Drv::IMU::CalibrationData& calibData) {
    calibData = calibrationData;
  }

void Components::ICM20948::errorReport(ErrorCode errorCode) {
    switch (errorCode) {
        case ICM20948::ErrorCode::NoError:
        break;
        case ICM20948::ErrorCode::NotInited:
            this->log_WARNING_HI_NotInited();
        break;
        case ICM20948::ErrorCode::FailedWriteToIMURegister:
            this->log_WARNING_HI_FailedWriteToIMURegister();
        break;
        case ICM20948::ErrorCode::FailedReadIMURegister:
            this->log_WARNING_HI_FailedReadIMURegister();
        break;
        case ICM20948::ErrorCode::ReadNotEqualTOWrittenRegister:
            this->log_WARNING_HI_ReadNotEqualTOWrittenRegister();
        break;
        default:
            this->log_WARNING_HI_UnknownError();
        break;
    }
  }

void  ICM20948::configure(Mode mode) {
  ErrorCode errorCode;
  
  switch(mode) {
    case Mode::NO_FUSION: {
        errorCode = rawInit();
        retrieveQuaternionData = false;
        break;
    }

    case Mode::FUSION: {
        errorCode = dmpInit();
        retrieveQuaternionData = true;
        break;
    }

    default:

    break;
  }

  errorReport(errorCode);
}

ErrorCode Components::ICM20948::rawInit() {
    initializationComplete = false;
    resetRegisterBank();

    auto initResult = checkMapValidity();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = isConnected();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = deviceReset();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configClockSource();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = enableGyroAndAccel();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = enableFIFO(false);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = disableDMP();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = setConfig(AccelFullScale::Scale4g, GyroFullScale::Scale2000dps);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configureFIFO(true);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configureInterruptPin(false, false, true);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configureDataReadyInterrupt(false);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configureGyroSampleRate();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = configureAccelSampleRate();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = initMagnetometer();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = calibrateGyroOffsets();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = enableFIFO(true);
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initResult = resetFIFO();
    if (initResult != ErrorCode::NoError) {
        return initResult;
    }

    initializationComplete = true;
    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::dmpInit() { 
    initializationComplete = false;
    resetRegisterBank();

    ErrorCode error = initializeDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }
    
    // Enable the DMP orientation sensor
    error = enableDMPOrientationSensor();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configuring DMP to output data at multiple ODRs
    error = setDMPODRrate();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Enable the FIFO
    error = enableFIFO(true);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Enable the DMP
    error = enableDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Reset DMP
    error = resetDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Reset FIFO
    error = resetFIFO();
    if (error != ErrorCode::NoError) {
        return error;
    }

    initializationComplete = true;
    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::initializeDMP() {
    ErrorCode error = configureMagnReadingForDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = configureMagnMeasurementForDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }

     // Set the ODR configuration to 1100/2^4 = 68.75Hz
    constexpr U8 mstODRconfig = 0x04;
    if (!writeReg(AddressID::I2C_MST_ODR_CONFIG, mstODRconfig)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    error = configClockSource();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Enable accel and gyro sensors through PWR_MGMT_2
    constexpr U8 pwrMgmt2Value = 0x40; 
    if (!writeReg(AddressID::PWR_MGMT_2, pwrMgmt2Value)) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    
    // Place _only_ I2C_Master in Low Power Mode (cycled) via LP_CONFIG
    constexpr U8 mstI2cDutyCycle = (1U << 6U); 
    if (!writeReg(AddressID::LP_CONFIG, mstI2cDutyCycle)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Disable the FIFO
    error = enableFIFO(false);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Disable the DMP
    error = disableDMP();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // If something goes wrong, pay attention to these lines
    // The gyro DLPF is enabled by default (GYRO_CONFIG_1 = 0x01);
    error = setConfig(AccelFullScale::Scale4g, GyroFullScale::Scale2000dps);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Turn off what goes into the FIFO through FIFO_EN_1, FIFO_EN_2
    constexpr U8 zero = 0U;
    if (!writeReg(AddressID::FIFO_EN_1, zero)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    if (!writeReg(AddressID::FIFO_EN_2, zero)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Turn off data ready interrupt through INT_ENABLE_1
    error = configureDataReadyInterrupt(false);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Reset FIFO through FIFO_RST
    // !!! review this step again if any issues encountered. In reference example write after write of 0x1f, write of 0x1e is performed
    error = resetFIFO();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // set gyro sample rate
    error = configureGyroSampleRate();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // set accel sample rate
    error = configureAccelSampleRate();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Setup DMP start address through PRGM_STRT_ADDRH/PRGM_STRT_ADDRL
    error = setDMPStartAddress();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Now load the DMP firmware
    error = loadDMPFirmware();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Write the 2 byte Firmware Start Value to ICM PRGM_STRT_ADDRH/PRGM_STRT_ADDRL
    error = setDMPStartAddress();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Set the Hardware Fix Disable register to 0x48
    uint8_t fix = 0x48;
    if (!writeReg(AddressID::HW_FIX_DISABLE, fix)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Set the Single FIFO Priority Select register to 0xE4
    uint8_t fifoPrio = 0xE4;
    if (!writeReg(AddressID::SINGLE_FIFO_PRIORITY_SEL, fifoPrio)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Configure Accel scaling to DMP
    // The DMP scales accel raw data internally to align 1g as 2^25
    // In order to align internal accel raw data 2^25 = 1g write 0x04000000 when FSR is 4g

    // Write accScale to ACC_SCALE DMP register
    etl::array<uint8_t, 4> accScale = {0x04, 0x00, 0x00, 0x00};
    error = writeDMPMemory(ACC_SCALE, accScale);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Write accScale2 to ACC_SCALE2 DMP register
    etl::array<uint8_t, 4> accScale2 = {0x00, 0x04, 0x00, 0x00};
    error = writeDMPMemory(ACC_SCALE2, accScale2);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure Compass mount matrix and scale to DMP
    // The mount matrix write to DMP register is used to align the compass axes with accel/gyro.
    // This mechanism is also used to convert hardware unit to uT. The value is expressed as 1uT = 2^30.
    // Each compass axis will be converted as below:
    // X = raw_x * CPASS_MTX_00 + raw_y * CPASS_MTX_01 + raw_z * CPASS_MTX_02
    // Y = raw_x * CPASS_MTX_10 + raw_y * CPASS_MTX_11 + raw_z * CPASS_MTX_12
    // Z = raw_x * CPASS_MTX_20 + raw_y * CPASS_MTX_21 + raw_z * CPASS_MTX_22
    // The AK09916 produces a 16-bit signed output in the range +/-32752 corresponding to +/-4912uT. 1uT = 6.66 ADU.
    // 2^30 / 6.66666 = 161061273 = 0x9999999
    etl::array<uint8_t, 4> mountMultiplierZero = {0x00, 0x00, 0x00, 0x00};
    etl::array<uint8_t, 4> mountMultiplierPlus = {0x09, 0x99, 0x99, 0x99};  // Value taken from InvenSense Nucleo example
    etl::array<uint8_t, 4> mountMultiplierMinus = {0xF6, 0x66, 0x66, 0x67}; // Value taken from InvenSense Nucleo example

    error = writeDMPMemory(CPASS_MTX_00, mountMultiplierPlus);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_01, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_02, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_10, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_11, mountMultiplierMinus);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_12, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_20, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_21, mountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(CPASS_MTX_22, mountMultiplierMinus);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the B2S Mounting Matrix
    etl::array<uint8_t, 4> b2sMountMultiplierZero = {0x00, 0x00, 0x00, 0x00};
    etl::array<uint8_t, 4> b2sMountMultiplierPlus = {0x40, 0x00, 0x00, 0x00}; // Value taken from InvenSense Nucleo example
    error = writeDMPMemory(B2S_MTX_00, b2sMountMultiplierPlus);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_01, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_02, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_10, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_11, b2sMountMultiplierPlus);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_12, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_20, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_21, b2sMountMultiplierZero);
    if (error != ErrorCode::NoError) {
        return error;
    }
    error = writeDMPMemory(B2S_MTX_22, b2sMountMultiplierPlus);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the DMP Gyro Scaling Factor
    // @param[in] gyro_div Value written to GYRO_SMPLRT_DIV register, where
    //            0=1125Hz sample rate, 1=562.5Hz sample rate, ... 4=225Hz sample rate, ...
    //            10=102.2727Hz sample rate, ... etc.
    // @param[in] gyro_level 0=250 dps, 1=500 dps, 2=1000 dps, 3=2000 dps    
    error = setGyroSF(19, 3);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the Gyro full scale
    // 2000dps : 2^28
    // 1000dps : 2^27
    //  500dps : 2^26
    //  250dps : 2^25
    etl::array<uint8_t, 4> gyroFullScale = {0x10, 0x00, 0x00, 0x00}; // 2000dps : 2^28
    error = writeDMPMemory(GYRO_FULLSCALE, gyroFullScale);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the Accel Only Gain: 15252014 (225Hz) 30504029 (112Hz) 61117001 (56Hz)
    etl::array<uint8_t, 4> accelOnlyGain = {0x03, 0xA4, 0x92, 0x49}; // 56Hz
    error = writeDMPMemory(ACCEL_ONLY_GAIN, accelOnlyGain);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the Accel Alpha Var: 1026019965 (225Hz) 977872018 (112Hz) 882002213 (56Hz)
    etl::array<uint8_t, 4> accelAlphaVar = {0x34, 0x92, 0x49, 0x25}; // 56Hz
    error = writeDMPMemory(ACCEL_ALPHA_VAR, accelAlphaVar);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Configure the Accel A Var: 47721859 (225Hz) 95869806 (112Hz) 191739611 (56Hz)
    etl::array<uint8_t, 4> accelAVar = {0x0B, 0x6D, 0xB6, 0xDB}; // 56Hz
    error = writeDMPMemory(ACCEL_A_VAR, accelAVar);
    if (error != ErrorCode::NoError) {
        return error;
    }
    
    // Configure the Accel Cal Rate
    etl::array<uint8_t, 2> accelCalRate = {0x00, 0x00}; // Value taken from InvenSense Nucleo example
    error = writeDMPMemory(ACCEL_CAL_RATE, accelCalRate);
    if (error != ErrorCode::NoError) {
        return error;
    }
    
    // Configure the Compass Time Buffer. The I2C Master ODR Configuration (see above) sets the magnetometer read rate to 68.75Hz.
    // Let's set the Compass Time Buffer to 69 (Hz).
    etl::array<uint8_t, 2> compassRate = {0x00, 0x45}; // 69Hz
    error = writeDMPMemory(CPASS_TIME_BUFFER, compassRate);
    if (error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::enableDMPOrientationSensor() {
    // Convert sensor from enum inv_icm20948_sensor to Android numbering

    uint8_t androidSensor = 3; // Android sensor orientation
    uint16_t delta = androidSensorToControlBits[androidSensor];
    if (delta == 0xFFFF) {
        return ErrorCode::NotInited;
    }

    uint32_t androidSensorAsBitMask = (1L << androidSensor);
    if (androidSensor < 32) // Sensors 0-31
    {
      androidSensorAsBitMask = 1L << androidSensor;
      enabledAndroid0 |= androidSensorAsBitMask; // Set the bit to enable the sensor
    }
    else // Sensors 32-
    {
      androidSensorAsBitMask = 1L << (androidSensor - 32);
      enabledAndroid1 |= androidSensorAsBitMask; // Set the bit to enable the sensor
    }
    
    uint16_t inv_event_control = 0; // Use this to store the value for MOTION_EVENT_CTL
    uint16_t data_rdy_status = 0;   // Use this to store the value for DATA_RDY_STATUS
    delta = 0;
    // Now we know androidSensor is valid, reconstruct the value for DATA_OUT_CTL1 from _enabled_Android_0 and _enabled_Android_1
    for (int i = 0; i < 32; i++) {
      androidSensorAsBitMask = 1L << i;
      if ((enabledAndroid0 & androidSensorAsBitMask) > 0) // Check if the Android sensor (0-31) is enabled
      {
        delta |= androidSensorToControlBits[i]; // If it is, or the required bits into delta
      }
      if ((enabledAndroid1 & androidSensorAsBitMask) > 0) // Check if the Android sensor (32-) is enabled
      {
        delta |= androidSensorToControlBits[i + 32]; // If it is, or the required bits into delta
      }

      if (((androidSensorAsBitMask & INV_NEEDS_ACCEL_MASK) > 0) || ((androidSensorAsBitMask & INV_NEEDS_ACCEL_MASK1) > 0))
      {
        data_rdy_status |= DMPDataReadyAccel;
        inv_event_control |= DMPMotionEventControlAccelCalibr;
      }
      if (((androidSensorAsBitMask & INV_NEEDS_GYRO_MASK) > 0) || ((androidSensorAsBitMask & INV_NEEDS_GYRO_MASK1) > 0))
      {
        data_rdy_status |= DMP_Data_ready_Gyro;
        inv_event_control |= DMP_Motion_Event_Control_Gyro_Calibr;
      }
      if (((androidSensorAsBitMask & INV_NEEDS_COMPASS_MASK) > 0) || ((androidSensorAsBitMask & INV_NEEDS_COMPASS_MASK1) > 0))
      {
        data_rdy_status |= DMP_Data_ready_Secondary_Compass;
        inv_event_control |= DMP_Motion_Event_Control_Compass_Calibr;
      }
    }

    // Make sure chip is awake
    ErrorCode error = wakeup();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Make sure chip is not in low power state    
    error = lowPowerDisable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Check if Accel, Gyro/Gyro_Calibr or Compass_Calibr/Quat9/GeoMag/Compass are to be enabled. If they are then we need to request the accuracy data via header2.
    uint16_t delta2 = 0;
    if ((delta & DMP_Data_Output_Control_1_Accel) > 0) {
      delta2 |= DMP_Data_Output_Control_2_Accel_Accuracy;
    }
    if (((delta & DMP_Data_Output_Control_1_Gyro_Calibr) > 0) || ((delta & DMP_Data_Output_Control_1_Gyro) > 0)) {
      delta2 |= DMP_Data_Output_Control_2_Gyro_Accuracy;
    }
    if (((delta & DMP_Data_Output_Control_1_Compass_Calibr) > 0) || ((delta & DMP_Data_Output_Control_1_Compass) > 0) || ((delta & DMP_Data_Output_Control_1_Quat9) > 0) || ((delta & DMP_Data_Output_Control_1_Geomag) > 0)) {
      delta2 |= DMP_Data_Output_Control_2_Compass_Accuracy;
    }
    
    // TODO: Add DMP_Data_Output_Control_2_Pickup etc. if required
    etl::array<uint8_t, 2> data_output_control_reg;
    data_output_control_reg.at(0) = static_cast<uint8_t>(delta >> 8);
    data_output_control_reg.at(1) = static_cast<uint8_t>(delta & 0xff);
    error = writeDMPMemory(DATA_OUT_CTL1, data_output_control_reg);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Write the sensor control bits into memory address DATA_OUT_CTL1
    data_output_control_reg.at(0) = static_cast<uint8_t>(delta2 >> 8);
    data_output_control_reg.at(1) = static_cast<uint8_t>(delta2 & 0xff);
    error = writeDMPMemory(DATA_OUT_CTL2, data_output_control_reg);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Write the 'header2' sensor control bits into memory address DATA_OUT_CTL2
    data_output_control_reg.at(0) = static_cast<uint8_t>(data_rdy_status >> 8);
    data_output_control_reg.at(1) = static_cast<uint8_t>(data_rdy_status & 0xff);
    error = writeDMPMemory(DATA_RDY_STATUS, data_output_control_reg);
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Check which extra bits need to be set in the Motion Event Control register
    if ((delta & DMP_Data_Output_Control_1_Quat9) > 0) {
      inv_event_control |= DMP_Motion_Event_Control_9axis;
    }
    if (((delta & DMP_Data_Output_Control_1_Step_Detector) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_0) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_1) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_2) > 0)) {
      inv_event_control |= DMP_Motion_Event_Control_Pedometer_Interrupt;
    }
    if ((delta & DMP_Data_Output_Control_1_Geomag) > 0) {
      inv_event_control |= DMP_Motion_Event_Control_Geomag;
    }

    // Set the MOTION_EVENT_CTL register
    data_output_control_reg.at(0) = static_cast<uint8_t>(inv_event_control >> 8);
    data_output_control_reg.at(1) = static_cast<uint8_t>(inv_event_control & 0xff);
    error = writeDMPMemory(MOTION_EVENT_CTL, data_output_control_reg);
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = lowPowerEnable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::setDMPODRrate() {
    // In order to set an ODR for a given sensor data, write 2-byte value to DMP using key defined above for a particular sensor.
    // Setting value can be calculated as follows:
    // Value = (DMP running rate (225Hz) / ODR ) - 1
    // E.g. For a 25Hz ODR rate, value= (225/25) - 1 = 8.
    uint32_t interval = 0;

    // Set the ODR registers and clear the ODR counter

    // In order to set an ODR for a given sensor data, write 2-byte value to DMP using key defined above for a particular sensor.
    // Setting value can be calculated as follows:
    // Value = (DMP running rate (225Hz) / ODR ) - 1
    // E.g. For a 25Hz ODR rate, value= (225/25) -1 = 8.

    // During run-time, if an ODR is changed, the corresponding rate counter must be reset.
    // To reset, write 2-byte {0,0} to DMP using keys below for a particular sensor:

    etl::array<uint8_t, 2> odr_reg_val;
    odr_reg_val.at(0) = static_cast<uint8_t>(interval >> 8);
    odr_reg_val.at(1) = static_cast<uint8_t>(interval & 0xff);

    etl::array<uint8_t, 2> odr_count_zero = {0x00, 0x00};

    // Make sure chip is awake
    ErrorCode error = wakeup();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Make sure chip is not in low power state    
    error = lowPowerDisable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = writeDMPMemory(ODR_QUAT9, odr_reg_val);
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = writeDMPMemory(ODR_CNTR_QUAT9, odr_count_zero);
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = lowPowerEnable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::setDMPStartAddress(uint16_t address) {
    etl::array<uint8_t, 2> startAddrBuff;
    startAddrBuff.at(0) = static_cast<U8>(address >> 8U);
    startAddrBuff.at(1) = static_cast<U8>(address && 0xFF);

    if (!burstWriteRegs(AddressID::PRGM_START_ADDRH, startAddrBuff)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::loadDMPFirmware() {
    // Make sure chip is awake
    ErrorCode error = wakeup();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Make sure chip is not in low power state
    error = lowPowerDisable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    // Write DMP memory
    const uint8_t *data = dmp3_image;
    int32_t  writeSize = 0;

    uint16_t size = sizeof(dmp3_image);
    uint16_t memAddr = dmpLoadStart;
    while (size > 0) {
        if (size <= maxDMPFirmwareBytesForTransaction) {
            writeSize = size;
        }
        else {
            writeSize = maxDMPFirmwareBytesForTransaction;
        }
        if ((memAddr & 0xff) + writeSize > 0x100)
        {
          // Moved across a bank
          writeSize = (memAddr & 0xff) + writeSize - 0x100;
        }

        // Conduct write operation
        ErrorCode error = writeDMPMemory(memAddr, etl::span(const_cast<uint8_t *>(data), writeSize));
        if (error != ErrorCode::NoError) {
            return error;
        }

        data += writeSize;
        size -= writeSize;
        memAddr += writeSize;
    }

    // Verify DMP memory
    data = dmp3_image;
    size = sizeof(dmp3_image);
    memAddr = dmpLoadStart;
    int32_t flag = 0;
    while (size > 0)
    {
      if (size <= maxDMPFirmwareBytesForTransaction) {
        writeSize = size;
      }
      else {
        writeSize = maxDMPFirmwareBytesForTransaction;
      }

      if ((memAddr & 0xff) + writeSize > 0x100)
      {
        // Moved across a bank
        writeSize = (memAddr & 0xff) + writeSize - 0x100;
      }

      etl::array<uint8_t, maxDMPFirmwareBytesForTransaction> dataCmp{0};
      ErrorCode error = readDMPMemory(memAddr, etl::span(dataCmp.data(), writeSize));
      if (error != ErrorCode::NoError) {
          flag++;
      }

      // Compare the data
      if (etl::mem_compare(const_cast<uint8_t *>(data), writeSize, dataCmp.data())) {
        return ErrorCode::ReadNotEqualTOWrittenRegister;
      }

      data += writeSize;
      size -= writeSize;
      memAddr += writeSize;
    }

    // Put chip in low power state
    error = lowPowerEnable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    if (flag) {
        return ErrorCode::FailedReadIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::writeDMPMemory(uint16_t reg, etl::span<uint8_t> writeBuffer) {
    if (!selectRegisterBank(RegisterBankSel::UserBank0)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    uint8_t lBankSelected = (reg >> 8);
    if (!writeReg(AddressID::MEM_BANK_SEL, lBankSelected)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto length = writeBuffer.size();
    uint32_t bytesWritten = 0;

    while (bytesWritten < length)
    {
      uint8_t lStartAddrSelected = (reg & 0xff);

      /* Sets the starting read or write address for the selected memory, inside of the selected page (see MEM_SEL Register).
             Contents are changed after read or write of the selected memory.
             This register must be written prior to each access to initialize the register to the proper starting address.
             The address will auto increment during burst transactions.  Two consecutive bursts without re-initializing the start address would skip one address. */
      if (!writeReg(AddressID::MEM_START_ADDR, lStartAddrSelected)) {
          return ErrorCode::FailedWriteToIMURegister;
      }

      uint32_t thisLen      = 0;
      if (length - bytesWritten <= maxDMPFirmwareBytesForTransaction) {
        thisLen = length - bytesWritten;
      }
      else {
        thisLen = maxDMPFirmwareBytesForTransaction;
      }

      /* Write data */
      etl::array<uint8_t, maxDMPFirmwareBytesForTransaction> buff;
      etl::copy(writeBuffer.data() + bytesWritten, writeBuffer.data() + bytesWritten + thisLen, buff.begin());
      
      if (!burstWriteRegs(AddressID::MEM_R_W, etl::span<uint8_t>(buff.data(), thisLen))) {
        return ErrorCode::FailedWriteToIMURegister;
      }

      bytesWritten += thisLen;
      reg += thisLen;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::readDMPMemory(uint16_t reg, etl::span<uint8_t> readBuffer) {
    if (!selectRegisterBank(RegisterBankSel::UserBank0)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    uint8_t lBankSelected = (reg >> 8);
    if (!writeReg(AddressID::MEM_BANK_SEL, lBankSelected)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto length = readBuffer.size();
    uint32_t bytesRead = 0;
    while (bytesRead < length)
    {
      uint8_t lStartAddrSelected = (reg & 0xff);

      /* Sets the starting read or write address for the selected memory, inside of the selected page (see MEM_SEL Register).
             Contents are changed after read or write of the selected memory.
             This register must be written prior to each access to initialize the register to the proper starting address.
             The address will auto increment during burst transactions.  Two consecutive bursts without re-initializing the start address would skip one address. */

      if (!writeReg(AddressID::MEM_START_ADDR, lStartAddrSelected)) {
          return ErrorCode::FailedWriteToIMURegister;
      }
    
      uint32_t thisLen = 0;
      if (length - bytesRead <= maxDMPFirmwareBytesForTransaction) {
        thisLen = length - bytesRead;
      }
      else {
        thisLen = maxDMPFirmwareBytesForTransaction;
      }

      /* Write data */
      etl::span<uint8_t> buff(readBuffer.data() + bytesRead, thisLen);
      if (!burstReadRegs(AddressID::MEM_R_W, buff)) {
        return ErrorCode::FailedWriteToIMURegister;
      }

      bytesRead += thisLen;
      reg += thisLen;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::setGyroSF(uint8_t div, int32_t gyroLevel) {
    gyroLevel = 4;
    // First read the TIMEBASE_CORRECTION_PLL register from Bank 1
    uint8_t unsignedPll = 0; // Signed. Typical value is 0x18

    if (!readReg(AddressID::TIMEBASE_CORRECTION_PLL, unsignedPll)) {
        return ErrorCode::FailedReadIMURegister;
    }
    int8_t pll = static_cast<int8_t>(unsignedPll);
    
    int32_t gyroSf = 0;
    constexpr uint64_t  MagicConstant = 264446880937391LL;
    constexpr uint64_t  MagicConstantScale = 100000LL;
    uint64_t ResultLL = 0;

    if(pll & 0x80) {
        ResultLL = (MagicConstant * (int64_t)(1ULL << gyroLevel) * (1 + div) / (1270 - (pll & 0x7F)) / MagicConstantScale);
    }
    else {
        ResultLL = (MagicConstant * (int64_t)(1ULL << gyroLevel) * (1 + div) / (1270 + pll) / MagicConstantScale);
    }

    if (ResultLL > 0x7FFFFFFF) {
        gyroSf = 0x7FFFFFFF;
    }
    else {
        gyroSf = (int32_t)ResultLL;
    }

    // Finally, write the value to the DMP GYRO_SF register
    etl::array<uint8_t, 4> gyroSfReg;
    gyroSfReg.at(0) = static_cast<uint8_t>(gyroSf >> 24);
    gyroSfReg.at(1) = static_cast<uint8_t>(gyroSf >> 16);
    gyroSfReg.at(2) = static_cast<uint8_t>(gyroSf >> 8);
    gyroSfReg.at(3) = static_cast<uint8_t>(gyroSf & 0xff);

    ErrorCode error = writeDMPMemory(GYRO_SF, gyroSfReg);
    if (error != ErrorCode::NoError) {
        return error;
    }
    
    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureMagnReadingForDMP() {
    /**
     * Configure I2C slave 0 to perform ten bytes reading from magnetometer AK09916
     */
    // Set the peripheral address and the Rw flag | write to ADDR_I2C_SLV0_ADDR
    constexpr U8 transferReadMask = 0b1000'0000;
    U8 periphAddr = AK09916Address | transferReadMask;
    if (!writeReg(AddressID::I2C_SLV0_ADDR, periphAddr)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Set address to start with | write to ADDR_I2C_SLV0_REG
    U8 startAddr = MAGN_ADDR_RSV2;
    if (!writeReg(AddressID::I2C_SLV0_REG, startAddr)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Setup control address     | write to ADDR_I2C_SLV0_CTRL
    constexpr U8 length = 10;
    constexpr U8 grp = (1U << 4U);
    constexpr U8 byteSwap = (1U << 6U);
    constexpr U8 enable = (1U << 7U);
    U8 ctrlConfig = (length | grp | byteSwap | enable);
    if (!writeReg(AddressID::I2C_SLV0_CTRL, ctrlConfig)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureMagnMeasurementForDMP() {
    /**
     * Configure I2C slave 1 to to single measurement triggering for magnetometer AK09916
     */
    U8 periphAddr = AK09916Address;
    if (!writeReg(AddressID::I2C_SLV1_ADDR, periphAddr)) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    
    // Setup data out | write to I2C_SLV1_DO
    constexpr U8 singleMeasurement = (1U << 0U);
    U8 dataOut = singleMeasurement;
    if (!writeReg(AddressID::I2C_SLV1_DO, dataOut)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Set address to start with | write to ADDR_I2C_SLV1_REG
    U8 startAddr = MAGN_ADDR_CNTL2;
    if (!writeReg(AddressID::I2C_SLV1_REG, startAddr)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // Setup control address     | write to ADDR_I2C_SLV1_CTRL
    constexpr U8 length = 1;
    constexpr U8 enable = (1U << 7U);
    U8 ctrlConfig = (length | enable);
    if (!writeReg(AddressID::I2C_SLV1_CTRL, ctrlConfig)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

bool Components::ICM20948::pollRawData() {
    if (!initializationComplete) {
        return false;
    }

    if (!checkAndUpdateFIFO()) {
        return false;
    }

    accelDataHandling();
    gyroDataHandling();
    temperatureDataHandling();
    magneticDataHandling();

    return true;
}

bool Components::ICM20948::pollDMPData() {
    if (!initializationComplete) {
        return false;
    }

    etl::array<uint8_t, numOfFIFOCountRegs> FIFOCount{0};
    if (!burstReadRegs(AddressID::FIFO_COUNTH, FIFOCount)) {
        return false;
    }
  
    uint16_t bytesAvailable = static_cast<uint16_t>(FIFOCount.at(0) << u8bitsOffset) | FIFOCount.at(1);
    if (bytesAvailable < dmpHeaderBytes) {
        return false;
    }

    // Read the header (2 bytes)
    header1DMPData = 0;
    etl::array<uint8_t, dmpHeaderBytes> headerBuffer{0};
    if (!burstReadRegs(AddressID::FIFO_R_W, headerBuffer)) {
        return false;
    }
    header1DMPData = static_cast<uint16_t>(headerBuffer.at(0) << u8bitsOffset) | headerBuffer.at(1); // MSB first
    bytesAvailable -= dmpHeaderBytes; // Decrement the count

    // If the header indicates a header2 is present then read that now
    
    // If the header2 bit is set
    if ((header1DMPData & DMP_header_bitmap_Header2) > 0) {
        // Check if we need to read the FIFO count again
        if (bytesAvailable < dmpHeaderBytes) {
            if (!burstReadRegs(AddressID::FIFO_COUNTH, FIFOCount)) {
                return false;
            }
            uint16_t bytesAvailable = static_cast<uint16_t>(FIFOCount.at(0) << u8bitsOffset) | FIFOCount.at(1);
            // Exit if no header2 is available
            if (bytesAvailable < dmpHeaderBytes) {
                return false;
            }
        }

        // Read the header (2 bytes)
        header2DMPData = 0; // Clear the existing header2
        if (!burstReadRegs(AddressID::FIFO_R_W, headerBuffer)) {
            return false;
        }
        header2DMPData = static_cast<uint16_t>(headerBuffer.at(0) << u8bitsOffset) | headerBuffer.at(1); // MSB first
        bytesAvailable -= dmpHeaderBytes; // Decrement the count
    }

    // case DMP_header_bitmap_Quat9:
    if ((header1DMPData & DMP_header_bitmap_Quat9) > 0)
    {
        if (bytesAvailable < dmpQuat9Bytes) // Check if we need to read the FIFO count again
        {
          if (!burstReadRegs(AddressID::FIFO_COUNTH, FIFOCount)) {
              return false;
          }
          uint16_t bytesAvailable = static_cast<uint16_t>(FIFOCount.at(0) << u8bitsOffset) | FIFOCount.at(1);
          // Exit if quaternion is not available
          if (bytesAvailable < dmpQuat9Bytes) {
              return false;
          }
        }
        etl::array<uint8_t, dmpQuat9Bytes> quatBuffer{0};
        if (!burstReadRegs(AddressID::FIFO_R_W, quatBuffer)) {
          return false;
        }

        int32_t q1 = static_cast<int32_t>(static_cast<uint32_t>(
                    static_cast<uint32_t>(quatBuffer.at(0) << u24bitsOffset)
                    | static_cast<uint32_t>(quatBuffer.at(1) << u16bitsOffset) 
                    | static_cast<uint32_t>(quatBuffer.at(2) << u8bitsOffset) 
                    | quatBuffer.at(3)
                    ));

        int32_t q2 = static_cast<int32_t>(static_cast<uint32_t>(
                    static_cast<uint32_t>(quatBuffer.at(4) << u24bitsOffset)
                    | static_cast<uint32_t>(quatBuffer.at(5) << u16bitsOffset) 
                    | static_cast<uint32_t>(quatBuffer.at(6) << u8bitsOffset) 
                    | quatBuffer.at(7)
                    ));

        int32_t q3 = static_cast<int32_t>(static_cast<uint32_t>(
                    static_cast<uint32_t>(quatBuffer.at(8) << u24bitsOffset)
                    | static_cast<uint32_t>(quatBuffer.at(9) << u16bitsOffset) 
                    | static_cast<uint32_t>(quatBuffer.at(10) << u8bitsOffset) 
                    | quatBuffer.at(11)
                    ));

        quaternionResult.Accuracy = static_cast<int32_t>(static_cast<uint32_t>(
            static_cast<uint32_t>(quatBuffer.at(12) << u8bitsOffset)
            | static_cast<uint32_t>(quatBuffer.at(13))
            ));

        // scale values
        quaternionResult.Q1 = static_cast<double>(q1) / quaternionScaler;
        quaternionResult.Q2 = static_cast<double>(q2) / quaternionScaler;
        quaternionResult.Q3 = static_cast<double>(q3) / quaternionScaler;

        // Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
        // In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
        quaternionResult.Q0 = sqrt(1.0 - (quaternionResult.Q1 * quaternionResult.Q1 + quaternionResult.Q2 * quaternionResult.Q2 + quaternionResult.Q3 * quaternionResult.Q3));

        bytesAvailable -= dmpQuat9Bytes; // Decrement the count
    }

    if (resetFIFO() != ErrorCode::NoError) {
        return false;
    }

    return true;
}

etl::expected<AccelXYZ, ErrorCode> Components::ICM20948::getAccelData() {
    if (!initializationComplete) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    return accelDataResult;
}

etl::expected<GyroXYZ, ErrorCode> Components::ICM20948::getGyroData() {
    if (!initializationComplete) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    return gyroDataResult;
}

etl::expected<MagnXYZ, ErrorCode> Components::ICM20948::getMagneticData() {
    if (!initializationComplete) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    return magneticDataResult;
}

etl::expected<float, ErrorCode> Components::ICM20948::getTemperatureData() const {
    if (!initializationComplete) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    return temperatureResult;
}

etl::expected<QuatQ0Q1Q2Q3, ErrorCode> Components::ICM20948::getQuatData() {
    if (!initializationComplete) {
        return etl::unexpected(ErrorCode::NotInited);
    }

    return quaternionResult;
}


ErrorCode Components::ICM20948::checkMapValidity() {
    if (mapOfAddresses.size() != numOfAddresses) {
        return ErrorCode::NotInited;
    }
    return ErrorCode::NoError;
}

bool Components::ICM20948::readReg(AddressID reg, uint8_t &value) {
    auto itMapValue = mapOfAddresses.at(reg);
    if (!selectRegisterBank(itMapValue.UserBank)) {
        return false;
    }
    
    etl::array<uint8_t, 1> addrArr{static_cast<uint8_t>(itMapValue.datasheetAddress)};
    Fw::Buffer addrBuffer(addrArr.data(), addrArr.size());
    if (this->write_out(0, devAddr, addrBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }

    etl::array<uint8_t, 1> readBuff{};
    Fw::Buffer readBuffer(readBuff.data(), readBuff.size());
    if (this->read_out(0, devAddr, readBuffer, readTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }
    value = readBuff.front();

    return true;
}

bool Components::ICM20948::burstReadRegs(AddressID reg, etl::span<uint8_t> buff) {
    auto itMapValue = mapOfAddresses.at(reg);
    if (!selectRegisterBank(itMapValue.UserBank)) {
        return false;
    }

    etl::array<uint8_t, 1> addrArr{static_cast<uint8_t>(itMapValue.datasheetAddress)};
    Fw::Buffer addrBuffer(addrArr.data(), addrArr.size());
    if (this->write_out(0, devAddr, addrBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }
    
    Fw::Buffer readBuffer(buff.data(), buff.size());
    if (this->read_out(0, devAddr, readBuffer, readTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }

    return true;
}

bool Components::ICM20948::writeReg(AddressID reg, uint8_t value) {
    auto itMapValue = mapOfAddresses.at(reg);
    if (!selectRegisterBank(itMapValue.UserBank)) {
        return false;
    }

    etl::array<uint8_t, 2> writeArr{static_cast<uint8_t>(itMapValue.datasheetAddress), value};
    Fw::Buffer writeBuffer(writeArr.data(), writeArr.size());
    
    return (this->write_out(0, devAddr, writeBuffer, writeTimeout) == Drv::I2C::Status::SUCCESS);
}

bool Components::ICM20948::burstWriteRegs(AddressID reg, etl::span<uint8_t> buff) {
    constexpr auto maxSerialCommunicationSize = maxDMPFirmwareBytesForTransaction + 1; // additional byte for address id transaction
    etl::vector<uint8_t, maxSerialCommunicationSize> writeVector;
    if (buff.size() + 1 > maxSerialCommunicationSize) {
        return false;
    }

    auto itMapValue = mapOfAddresses.at(reg);
    if (!selectRegisterBank(itMapValue.UserBank)) {
        return false;
    }
    
    writeVector.push_back(itMapValue.datasheetAddress);
    for (auto i = 0; i < buff.size(); i++) {
      auto buffElement = *(buff.begin() + i);  
      writeVector.push_back(buffElement);  
    }

    Fw::Buffer writeBuffer(writeVector.data(), writeVector.size());
    
    if (this->write_out(0, devAddr, writeBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }

    return true;
}

bool Components::ICM20948::selectRegisterBank(RegisterBankSel regBank) {
    if (regBank != activatedCurrentBank) {
        const uint8_t userBankOffset = 4U;
        const uint8_t userBankMask = 0b11;
        auto registerValue = static_cast<uint8_t>(static_cast<uint8_t>(regBank) & userBankMask) << userBankOffset;
        
        etl::array<uint8_t, 2> regBankArr{ADDR_REG_BANK_SEL, static_cast<uint8_t>(registerValue)};
        Fw::Buffer regBankBuffer(regBankArr.data(), regBankArr.size());

        if (this->write_out(0, devAddr, regBankBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
            return false;
        }
        activatedCurrentBank = regBank;
    }

    return true;
}

void Components::ICM20948::resetRegisterBank() {
    activatedCurrentBank = RegisterBankSel::Common;
}

ErrorCode Components::ICM20948::checkRegister(AddressID addressID, uint8_t expectedValue) {
    uint8_t checkRegister = 0;
    if (!readReg(addressID, checkRegister)) {
        return ErrorCode::FailedReadIMURegister;
    }

    if (checkRegister != expectedValue) {
        return ErrorCode::ReadNotEqualTOWrittenRegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::isConnected() {
    uint8_t whoAmIValue{0};
    if (!readReg(AddressID::WHO_AM_I, whoAmIValue)) {
        return ErrorCode::FailedReadIMURegister;
    }

    if (expectedWhoAmI != whoAmIValue) {
        return ErrorCode::NotInited;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::deviceReset() {
    resetRegisterBank();

    // disable the master I2C to avoid hanging the slaves
    constexpr uint8_t I2CMasterEnableRegOffset = 5U;
    constexpr uint8_t disableI2CMaster = 0U;
    auto registerValue = static_cast<uint8_t>(disableI2CMaster << I2CMasterEnableRegOffset);

    if (!writeReg(AddressID::USER_CTRL, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // reset device
    constexpr uint8_t deviceResetOffset = 7U;
    constexpr uint8_t deviceResetValue = 1U;
    registerValue = deviceResetValue << deviceResetOffset;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    // some delay (100 ms)
    constexpr uint16_t delay100Ms{100};
    Os::Task::delay(delay100Ms);
    // clearing the bit wakes the chip from sleep mode
    constexpr uint8_t sleepBitOffset = 6U;
    constexpr uint8_t wakeChipFromSleep = 0U << sleepBitOffset;
    constexpr auto SLEEP = wakeChipFromSleep;
    registerValue = SLEEP;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    constexpr uint8_t delay10Ms{10};
    Os::Task::delay(delay10Ms);

    auto checkResult = checkRegister(AddressID::PWR_MGMT_1, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configClockSource() {
    constexpr uint8_t clockSelectOffset = 0U;
    constexpr uint8_t autoSelect = 1U << clockSelectOffset;
    auto registerValue = autoSelect;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::PWR_MGMT_1, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::enableGyroAndAccel() {
    etl::bitset<register8BitSize, uint8_t> registerValue;
    constexpr uint8_t enableAccelAndGyro = 0U;
    registerValue = enableAccelAndGyro;

    // Set the reserved bit 6 (pressure sensor disable?); refer to SparkFun_ICM-20948_ArduinoLibrary
    constexpr uint8_t reserved6BitSet = 1U << 6U;
    registerValue |= reserved6BitSet;

    if (!writeReg(AddressID::PWR_MGMT_2, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::PWR_MGMT_2, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::enableFIFO(bool activate) {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t userControlConfig = 0;
    if (!readReg(AddressID::USER_CTRL, userControlConfig)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = userControlConfig;

    if (activate) {
        constexpr uint8_t enableFIFO = 0b0100'0000;
        registerValue |= enableFIFO;

    } else {
        constexpr uint8_t disableFIFO = 0b1011'1111;
        registerValue &= disableFIFO;
    }

    if (!writeReg(AddressID::USER_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::USER_CTRL, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::enableDMP() {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t userControlConfig = 0;
    if (!readReg(AddressID::USER_CTRL, userControlConfig)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = userControlConfig;

    constexpr uint8_t enableDMP = 0b1000'0000;
    registerValue |= enableDMP;

    if (!writeReg(AddressID::USER_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::USER_CTRL, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::disableDMP() {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t userControlConfig = 0;
    if (!readReg(AddressID::USER_CTRL, userControlConfig)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = userControlConfig;

    constexpr uint8_t disableDMP = 0b0111'1111;
    registerValue &= disableDMP;

    if (!writeReg(AddressID::USER_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::USER_CTRL, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::resetDMP() {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t userControlConfig = 0;
    if (!readReg(AddressID::USER_CTRL, userControlConfig)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = userControlConfig;

    constexpr uint8_t disableDMP = 0b0000'1000;
    registerValue |= disableDMP;

    if (!writeReg(AddressID::USER_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

Components::ICM20948::ErrorCode Components::ICM20948::setConfig(AccelFullScale accelScale, GyroFullScale gyroScale) {
    auto configResult = configureGyroscope(gyroScale);
    if (configResult != ErrorCode::NoError) {
        return configResult;
    }

    configResult = configureAccelerometer(accelScale);
    if (configResult != ErrorCode::NoError) {
        return configResult;
    }

    return ErrorCode::NoError;
}

Components::ICM20948::ErrorCode Components::ICM20948::configureGyroscope(GyroFullScale scale) {
    switch (scale) {
        case GyroFullScale::Scale250dps: {
            const float scaleFactorFor250dps = 131;
            gyroScaleFactor = scaleFactorFor250dps;
            break;
        }
        case GyroFullScale::Scale500dps: {
            const float scaleFactorFor500dps = 65.5;
            gyroScaleFactor = scaleFactorFor500dps;
            break;
        }
        case GyroFullScale::Scale1000dps: {
            const float scaleFactorFor1000dps = 32.8;
            gyroScaleFactor = scaleFactorFor1000dps;
            break;
        }
        case GyroFullScale::Scale2000dps: {
            const float scaleFactorFor2000dps = 16.4;
            gyroScaleFactor = scaleFactorFor2000dps;
            break;
        }
        default:
            break;
    }

    etl::bitset<register8BitSize, uint8_t> registerValue;

    constexpr uint8_t gyroFullScaleOffset = 1U;
    constexpr uint8_t gyroFullScaleMask = 0b11;
    auto gyroFullScale = static_cast<uint8_t>(scale);
    auto GYRO_FS_SEL = static_cast<uint8_t>(gyroFullScale & gyroFullScaleMask) << gyroFullScaleOffset;
    registerValue |= GYRO_FS_SEL;

    constexpr uint8_t gyroDLPFMask = 0b1;
    constexpr uint8_t enableGyroDLPF = 1U;
    constexpr auto GYRO_FCHOICE = static_cast<uint8_t>(enableGyroDLPF & gyroDLPFMask);
    registerValue |= GYRO_FCHOICE;

    constexpr uint8_t gyroDLPFConfigOffset = 3U;
    constexpr uint8_t gyroDLPFConfigMask = 0b111;

    constexpr uint8_t NBW230Hz = 0U;
    constexpr auto GYRO_DLPFCFG = static_cast<uint8_t>(NBW230Hz & gyroDLPFConfigMask) << gyroDLPFConfigOffset;

    registerValue |= GYRO_DLPFCFG;

    if (!writeReg(AddressID::GYRO_CONFIG_1, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::GYRO_CONFIG_1, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return Components::ICM20948::ErrorCode::NoError;
}

Components::ICM20948::ErrorCode Components::ICM20948::configureAccelerometer(AccelFullScale scale) {
    switch (scale) {
        case AccelFullScale::Scale2g: {
            const float scaleFactorFor2g = 16'384.0;
            accelScaleFactor = scaleFactorFor2g;
            break;
        }
        case AccelFullScale::Scale4g: {
            const float scaleFactorFor4g = 8'192.0;
            accelScaleFactor = scaleFactorFor4g;
            break;
        }
        case AccelFullScale::Scale8g: {
            const float scaleFactorFor8g = 4'096.0;
            accelScaleFactor = scaleFactorFor8g;
            break;
        }
        case AccelFullScale::Scale16g: {
            const float scaleFactorFor16g = 2'048.0;
            accelScaleFactor = scaleFactorFor16g;
            break;
        }
        default:
            break;
    }

    etl::bitset<register8BitSize, uint8_t> registerValue;

    constexpr uint8_t accelFullScaleOffset = 1U;
    constexpr uint8_t accelFullScaleMask = 0b11;
    const auto accelFullScale = static_cast<uint8_t>(scale);
    auto ACCEL_FS_SEL = static_cast<uint8_t>(accelFullScale & accelFullScaleMask) << accelFullScaleOffset;
    registerValue |= ACCEL_FS_SEL;

    constexpr uint8_t accelDLPFOffset = 0U;
    constexpr uint8_t accelDLPFMask = 0b1;
    constexpr uint8_t enableAccelDLPF = 1U;
    constexpr auto ACCEL_FCHOICE = static_cast<uint8_t>(enableAccelDLPF & accelDLPFMask) << accelDLPFOffset;
    registerValue |= ACCEL_FCHOICE;

    constexpr uint8_t accelDLPFConfigOffset = 3U;
    constexpr uint8_t accelDLPFConfigMask = 0b111;
    constexpr uint8_t NBW265HZ = 0U;
    constexpr auto ACCEL_DLPFCFG = static_cast<uint8_t>(NBW265HZ & accelDLPFConfigMask) << accelDLPFConfigOffset;

    registerValue |= ACCEL_DLPFCFG;

    if (!writeReg(AddressID::ACCEL_CONFIG, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::ACCEL_CONFIG, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureGyroSampleRate() {
    constexpr uint8_t gyroSampleRateDivMask = 0b1111'1111;
    // constexpr uint8_t gyroSampleRateDivValue = 110U; // 10 Hz
    // constexpr uint8_t gyroSampleRateDivValue = 0x13; // 55 Hz
    constexpr uint8_t gyroSampleRateDivValue = 0; // 1.1 kHz
    // constexpr uint8_t gyroSampleRateDivValue = 2; // 367 Hz
    // constexpr uint8_t gyroSampleRateDivValue = 0xA; // 100 Hz

    constexpr auto GYRO_SMPLRT_DIV = static_cast<uint8_t>(gyroSampleRateDivValue & gyroSampleRateDivMask);
    auto registerValue = GYRO_SMPLRT_DIV;

    if (!writeReg(AddressID::GYRO_SMPLRT_DIV, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::GYRO_SMPLRT_DIV, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureAccelSampleRate() {
    constexpr uint8_t accelSampleRateDivMask = 0b1111'1111;
    // constexpr uint8_t accelSampleRateDivValue = 110U; // 10 Hz
    // constexpr uint8_t accelSampleRateDivValue = 0x13; // 56.25 Hz
    constexpr uint8_t accelSampleRateDivValue = 0; // 1.125 kHz
    // constexpr uint8_t accelSampleRateDivValue = 2; // 367 Hz
    // constexpr uint8_t accelSampleRateDivValue = 0xA; // 100 Hz


    auto ACCEL_SMPLRT_DIV = static_cast<uint8_t>(accelSampleRateDivValue & accelSampleRateDivMask);
    auto registerValue = ACCEL_SMPLRT_DIV;

    if (!writeReg(AddressID::ACCEL_SMPLRT_DIV_2, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::ACCEL_SMPLRT_DIV_2, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureFIFO(bool enableInterrupt) {
    uint8_t registerValue = 0;

    if (enableInterrupt) {
        constexpr uint8_t enableFifoOverflowInterrupt = 0x1;
        registerValue = enableFifoOverflowInterrupt;
    } else {
        constexpr uint8_t disableFifoOverflowInterrupt = 0U;
        registerValue = disableFifoOverflowInterrupt;
    }

    if (!writeReg(AddressID::INT_ENABLE_2, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::INT_ENABLE_2, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    constexpr uint8_t SLV_0_FIFO_EN = 1U;
    registerValue = SLV_0_FIFO_EN;
    if (!writeReg(AddressID::FIFO_EN_1, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    checkResult = checkRegister(AddressID::FIFO_EN_1, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    constexpr uint8_t enableAllInternalSensorsFIFOs = 0b11111;
    registerValue = enableAllInternalSensorsFIFOs;
    if (!writeReg(AddressID::FIFO_EN_2, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    checkResult = checkRegister(AddressID::FIFO_EN_2, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    constexpr uint8_t snapshotMode = 0b1;
    registerValue = snapshotMode;
    if (!writeReg(AddressID::FIFO_MODE, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    checkResult = checkRegister(AddressID::FIFO_MODE, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureDataReadyInterrupt(bool activate) {
    uint8_t registerValue = 0;

    if (activate) {
        constexpr uint8_t enableDataReadyInterrupt = 1U;
        registerValue = enableDataReadyInterrupt;
    } else {
        constexpr uint8_t disableDataReadyInterrupt = 0U;
        registerValue = disableDataReadyInterrupt;
    }

    if (!writeReg(AddressID::INT_ENABLE_1, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::INT_ENABLE_1, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureInterruptPin(bool logicLevel, bool open, bool latchEn) {
    uint8_t registerValue = 0;

    if (logicLevel) {
        constexpr uint8_t logicLevelLowValue = 1U;
        constexpr uint8_t logicLevelLowShift = 7U;
        registerValue |= (logicLevelLowValue << logicLevelLowShift);
    } else {
        constexpr uint8_t logicLevelHighValue = 0U;
        constexpr uint8_t logicLevelHighShift = 7U;
        registerValue |= (logicLevelHighValue << logicLevelHighShift);
    }

    if (open) {
        constexpr uint8_t openDrainValue = 1U;
        constexpr uint8_t openDrainShift = 6U;
        registerValue |= (openDrainValue << openDrainShift);
    } else {
        constexpr uint8_t pushPullValue = 0U;
        constexpr uint8_t pushPullShift = 6U;
        registerValue |= (pushPullValue << pushPullShift);
    }

    if (latchEn) {
        constexpr uint8_t heldValue = 1U;
        constexpr uint8_t heldShift = 5U;
        registerValue |= (heldValue << heldShift);
    } else {
        constexpr uint8_t pulseValue = 0U;
        constexpr uint8_t pulseShift = 5U;
        registerValue |= (pulseValue << pulseShift);
    }

    if (!writeReg(AddressID::INT_PIN_CFG, registerValue)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::INT_PIN_CFG, registerValue);
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::clearInterruptStatus() {
    uint8_t intStatus = 0;
    if (!readReg(AddressID::INT_STATUS, intStatus)) {
        return ErrorCode::FailedReadIMURegister;
    }

    uint8_t intStatus1 = 0;
    if (!readReg(AddressID::INT_STATUS_1, intStatus1)) {
        return ErrorCode::FailedReadIMURegister;
    }

    uint8_t intStatus2 = 0;
    if (!readReg(AddressID::INT_STATUS_2, intStatus2)) {
        return ErrorCode::FailedReadIMURegister;
    }

    uint8_t intStatus3 = 0;
    if (!readReg(AddressID::INT_STATUS_3, intStatus3)) {
        return ErrorCode::FailedReadIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::wakeup() {
    uint8_t pwrMgmtCfg = 0;
    if (!readReg(AddressID::PWR_MGMT_1, pwrMgmtCfg)) {
        return ErrorCode::FailedReadIMURegister;
    }
    etl::bitset<register8BitSize, uint8_t> registerValue;
    registerValue = pwrMgmtCfg;

    constexpr uint8_t sleepMask = 0b100'0000;
    registerValue &= ~sleepMask;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    return ErrorCode::NoError;
}
        
ErrorCode Components::ICM20948::sleep() {
    uint8_t pwrMgmtCfg = 0;
    if (!readReg(AddressID::PWR_MGMT_1, pwrMgmtCfg)) {
        return ErrorCode::FailedReadIMURegister;
    }
    etl::bitset<register8BitSize, uint8_t> registerValue;
    registerValue = pwrMgmtCfg;

    constexpr uint8_t sleepMask = 0b100'0000;
    registerValue |= sleepMask;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::lowPowerEnable() {
    uint8_t pwrMgmtCfg = 0;
    if (!readReg(AddressID::PWR_MGMT_1, pwrMgmtCfg)) {
        return ErrorCode::FailedReadIMURegister;
    }
    etl::bitset<register8BitSize, uint8_t> registerValue;
    registerValue = pwrMgmtCfg;

    constexpr uint8_t lpMask = 0b10'0000;
    registerValue |= lpMask;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    return ErrorCode::NoError;
}
        
ErrorCode Components::ICM20948::lowPowerDisable() {
    uint8_t pwrMgmtCfg = 0;
    if (!readReg(AddressID::PWR_MGMT_1, pwrMgmtCfg)) {
        return ErrorCode::FailedReadIMURegister;
    }
    etl::bitset<register8BitSize, uint8_t> registerValue;
    registerValue = pwrMgmtCfg;

    constexpr uint8_t lpMask = 0b10'0000;
    registerValue &= ~lpMask;

    if (!writeReg(AddressID::PWR_MGMT_1, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }
    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::resetFIFO() {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t fifoResetValue = 0;
    if (!readReg(AddressID::FIFO_RST, fifoResetValue)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = fifoResetValue;

    constexpr uint8_t fifoResetMask = 0b0001'1111;

    registerValue &= ~fifoResetMask;

    constexpr uint8_t assertSequence = 0x1f;
    registerValue |= static_cast<uint8_t>(assertSequence & fifoResetMask);
    if (!writeReg(AddressID::FIFO_RST, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    registerValue &= ~fifoResetMask;

    constexpr uint8_t deassertSequence = 0x0;
    registerValue |= static_cast<uint8_t>(deassertSequence & fifoResetMask);
    if (!writeReg(AddressID::FIFO_RST, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

bool Components::ICM20948::checkAndUpdateFIFO() {
    resetFIFO();
    Os::Task::delay(2); // if sample rate is modified, this timinig should be modified accordingly
    constexpr auto maxNumOfAttempts = 3;
    bool isDataMisaligned = true;
    U32 leftover = 0;

    for (auto i = 0; i < maxNumOfAttempts; i++) {
        etl::array<uint8_t, numOfFIFOCountRegs> FIFOCount{0};
        if (!burstReadRegs(AddressID::FIFO_COUNTH, FIFOCount)) {
            return false;
        }

        bytesAvailable = static_cast<uint16_t>(FIFOCount.at(0) << u8bitsOffset) | FIFOCount.at(1);

        numOfSamples = bytesAvailable / sampleSize;

        /**
         * In case if leftover > 0, the data retrieved from FIFO buffer is not guaranteed to be correct.
         * In ardupilot in order to detect FIFO sync errors, it is checked the temperature value isn't changed rapidly: https://github.com/ArduPilot/ardupilot/blob/3d199a9bc54b52b3ad037bd16341fef5a91c9766/libraries/AP_InertialSensor/AP_InertialSensor_Invensensev2.cpp#L552
         * Similar problem was described in article dedicated to MPU6050 FIFO buffer: https://www.fpaynter.com/2019/10/mpu6050-fifo-buffer-management-study/
         */
        leftover = bytesAvailable % sampleSize;
        isDataMisaligned = (leftover > 0);
        if (isDataMisaligned == false) {
            break;
        }

        resetFIFO();
        Os::Task::delay(2); // if sample rate is modified, this timinig should be modified accordingly
    }

    Fw::Logic interruptState; 
    this->intPin_out(0, interruptState);
    bool isFifoOverflowed = (interruptState == Fw::Logic::HIGH);

    if (isFifoOverflowed) {
        clearInterruptStatus();
    }

    if (isDataMisaligned || isFifoOverflowed) {
        return false;
    }
    
    if (numOfSamples == 0U) {
        return false;
    }

    if (!burstReadRegs(AddressID::FIFO_R_W, etl::span(packetBuffer.data(), sampleSize))) {
        return false;
    }

    return true;
}

void Components::ICM20948::accelDataHandling() {
    // read out the 6 bytes of acceleration data
    const uint8_t xAccelOutH = packetBuffer.at(0);
    const uint8_t xAccelOutL = packetBuffer.at(1);
    const uint8_t yAccelOutH = packetBuffer.at(2);
    const uint8_t yAccelOutL = packetBuffer.at(3);
    const uint8_t zAccelOutH = packetBuffer.at(4);
    const uint8_t zAccelOutL = packetBuffer.at(5);

    int16_t accelDataResultX =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(xAccelOutH) << u8bitsOffset) | xAccelOutL);
    int16_t accelDataResultY =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(yAccelOutH) << u8bitsOffset) | yAccelOutL);
    int16_t accelDataResultZ =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(zAccelOutH) << u8bitsOffset) | zAccelOutL);

    calibrationData.setAccelX(accelDataResultX);
    calibrationData.setAccelY(accelDataResultY);
    calibrationData.setAccelZ(accelDataResultZ);

    accelDataResult.x = accelDataResultX;
    accelDataResult.y = accelDataResultY;
    accelDataResult.z = accelDataResultZ;

    #ifdef APPLY_ACCEL_MAGNETO_CALIBRATION
        accelDataResult.x = (static_cast<float>(accelDataResult.x) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.y = (static_cast<float>(accelDataResult.y) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.z = (static_cast<float>(accelDataResult.z) / accelScaleFactor) * gravitationalAcceleration;

        // Parameters obtained from Magneto 1.2
        CalibrationParameters calibrationParametersMagneto = {
        { {(1.006529),(-0.008474),(-0.001234)}, {(-0.008474),(1.007249),(-0.003999)}, {(-0.001234),(-0.003999),(0.979124)} },
            -0.032960f,
            0.004517f,
            0.198124f
        };

        applyCalibrationParameters(accelDataResult.x, accelDataResult.y, accelDataResult.z, calibrationParametersMagneto);

    #elif defined(APPLY_ACCEL_FREEIMU_CALIBRATION)
        CalibrationParametersFreeIMU freeIMUCalibrationParameters = { 
            7947.356,
            8286.980,
            8369.229,
            11,
            -80,
            193
        };

        applyCalibrationParametersFreeIMU(accelDataResult.x, accelDataResult.y, accelDataResult.z, freeIMUCalibrationParameters);

        accelDataResult.x = (static_cast<float>(accelDataResult.x) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.y = (static_cast<float>(accelDataResult.y) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.z = (static_cast<float>(accelDataResult.z) / accelScaleFactor) * gravitationalAcceleration;
    #else
        accelDataResult.x = (static_cast<float>(accelDataResult.x) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.y = (static_cast<float>(accelDataResult.y) / accelScaleFactor) * gravitationalAcceleration;
        accelDataResult.z = (static_cast<float>(accelDataResult.z) / accelScaleFactor) * gravitationalAcceleration; 
    #endif
    
}

void Components::ICM20948::gyroDataHandling() {
    // read out the 6 bytes of gyroscope data
    const uint8_t xGyroOutH = packetBuffer.at(6);
    const uint8_t xGyroOutL = packetBuffer.at(7);
    const uint8_t yGyroOutH = packetBuffer.at(8);
    const uint8_t yGyroOutL = packetBuffer.at(9);
    const uint8_t zGyroOutH = packetBuffer.at(10);
    const uint8_t zGyroOutL = packetBuffer.at(11);

    int16_t gyroDataResultX =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(xGyroOutH) << u8bitsOffset) | xGyroOutL);
    int16_t gyroDataResultY =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(yGyroOutH) << u8bitsOffset) | yGyroOutL);
    int16_t gyroDataResultZ =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(zGyroOutH) << u8bitsOffset) | zGyroOutL);

    calibrationData.setGyroX(gyroDataResultX);
    calibrationData.setGyroY(gyroDataResultY);
    calibrationData.setGyroZ(gyroDataResultZ);

    // scale values
    gyroDataResult.x = static_cast<float>(gyroDataResultX) / gyroScaleFactor;
    gyroDataResult.y = static_cast<float>(gyroDataResultY) / gyroScaleFactor;
    gyroDataResult.z = static_cast<float>(gyroDataResultZ) / gyroScaleFactor;
    
    gyroDataResult.x -= GyroXOffset;
    gyroDataResult.y -= GyroYOffset;
    gyroDataResult.z -= GyroZOffset;
}

void Components::ICM20948::temperatureDataHandling() {
    // read out the 2 bytes of temperature data
    const uint8_t tempOutH = packetBuffer.at(12);
    const uint8_t tempOutL = packetBuffer.at(13);

    auto tempOut =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(tempOutH) << u8bitsOffset) | tempOutL);

    temperatureResult =
            ((static_cast<float>(tempOut) - temperatureOffset) / temperatureSensitivity) + temperatureConstant;
}

void Components::ICM20948::magneticDataHandling() {
    // read out the 6 bytes of gyro data
    const uint8_t xMagnOutH = packetBuffer.at(15);
    const uint8_t xMagnOutL = packetBuffer.at(14);
    const uint8_t yMagnOutH = packetBuffer.at(17);
    const uint8_t yMagnOutL = packetBuffer.at(16);
    const uint8_t zMagnOutH = packetBuffer.at(19);
    const uint8_t zMagnOutL = packetBuffer.at(18);

    int16_t magneticDataResultX =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(xMagnOutH) << u8bitsOffset) | xMagnOutL);
    int16_t magneticDataResultY =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(yMagnOutH) << u8bitsOffset) | yMagnOutL);
    int16_t magneticDataResultZ =
            static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(zMagnOutH) << u8bitsOffset) | zMagnOutL);
    
    magneticDataResult.x = magneticDataResultX;
    magneticDataResult.y = magneticDataResultY;
    magneticDataResult.z = magneticDataResultZ;

    #ifdef APPLY_MAGN_MAGNETO_CALIBRATION
        magneticDataResult.x = magneticDataResult.x*getSensitivity();
        magneticDataResult.y = magneticDataResult.y*getSensitivity();
        magneticDataResult.z = magneticDataResult.z*getSensitivity();

        // Parameters obtained from Magneto 1.2
        CalibrationParameters calibrationParametersMagneto = {
        { {(1.056275),(-0.007618),(0.000281)}, {(-0.007618),(1.012344),(0.000060)}, {(0.000281),(0.000060),(1.021847)} },
            4.594600f,
            -17.084129f,
            11.033411f
        };

        applyCalibrationParameters(magneticDataResult.x, magneticDataResult.y, magneticDataResult.z, calibrationParametersMotionCal);

    #elif defined(APPLY_MAGN_MOTIONCAL_CALIBRATION)
        magneticDataResult.x = magneticDataResult.x*getSensitivity();
        magneticDataResult.y = magneticDataResult.y*getSensitivity();
        magneticDataResult.z = magneticDataResult.z*getSensitivity();

        // Parameters obtained from Motion Sensor Calibration Tool
        CalibrationParameters calibrationParametersMotionCal = {
        { {(1.029),(-0.006),(0.0)}, {(-0.006),(0.981),(-0.013)}, {(0.0),(-0.013),(0.99)} },
            4.76f,
            -17.33f,
            10.99f
        };

        applyCalibrationParameters(magneticDataResult.x, magneticDataResult.y, magneticDataResult.z, calibrationParametersMotionCal);
    #elif defined(APPLY_MAGN_FREEIMU_CALIBRATION)
        CalibrationParametersFreeIMU freeIMUCalibrationParameters = { 
            299.351,
            317.740,
            312.497,
            32,
            -111,
            70
        };
        applyCalibrationParametersFreeIMU(magneticDataResult.x, magneticDataResult.y, magneticDataResult.z, freeIMUCalibrationParameters);

        magneticDataResult.x = magneticDataResult.x*getSensitivity();
        magneticDataResult.y = magneticDataResult.y*getSensitivity();
        magneticDataResult.z = magneticDataResult.z*getSensitivity();
    #else
        magneticDataResult.x = magneticDataResult.x*getSensitivity();
        magneticDataResult.y = magneticDataResult.y*getSensitivity();
        magneticDataResult.z = magneticDataResult.z*getSensitivity();
    #endif

    calibrationData.setMagnX(magneticDataResult.x);
    calibrationData.setMagnY(magneticDataResult.y);
    calibrationData.setMagnZ(magneticDataResult.z);

    // dummy magnetometer register with address 0x17 that is located before ST2 register is skipped

    constexpr uint8_t magneticSensorOverflowMask = 0b0000'1000;
    constexpr uint8_t magneticSensorOverflowOffset = 3U;
    auto magnetometerOverflowStatus = static_cast<uint8_t>(packetBuffer.at(21) & magneticSensorOverflowMask)
                                << magneticSensorOverflowOffset;
   setSensorOverflowStatus(magnetometerOverflowStatus);
}

ErrorCode Components::ICM20948::initMagnetometer() {
    constexpr bool disableBypass = false;
    auto initMagnResult = i2cMasterBypass(disableBypass);
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    constexpr bool enableMaster = true;
    initMagnResult = i2cMasterConfigure(enableMaster);
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    initMagnResult = resetMagnetometer();
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    initMagnResult = magnetometerWhoAmI();
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    initMagnResult = configureMagnetometer();
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    initMagnResult = startMagnetometer();
    if (initMagnResult != ErrorCode::NoError) {
        return initMagnResult;
    }

    return ErrorCode::NoError;
}

float Components::ICM20948::getSensitivity() {
    return AK09916Sensitivity;
}

void Components::ICM20948::setSensorOverflowStatus(uint8_t status) {
    AK09916Status2SensorOverflow = status;
}

uint8_t Components::ICM20948::getSensorOverflowStatus() const {
    return AK09916Status2SensorOverflow;
}

ErrorCode Components::ICM20948::i2cMasterBypass(bool enableBypass) {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t intPinCfg = 0;
    if (!readReg(AddressID::INT_PIN_CFG, intPinCfg)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = intPinCfg;

    constexpr uint8_t bypassEnableOffset = 1U;
    constexpr uint8_t bypassEnableMask = 0b1U;
    registerValue &= ~static_cast<uint8_t>(bypassEnableMask << bypassEnableOffset);

    if (enableBypass) {
        constexpr uint8_t bypassEnable = 1U << bypassEnableOffset;
        registerValue |= bypassEnable;
    } else {
        constexpr uint8_t bypassDisable = 0U << bypassEnableOffset;
        registerValue |= bypassDisable;
    }

    if (!writeReg(AddressID::INT_PIN_CFG, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::i2cMasterConfigure(bool enableMaster) {
    etl::bitset<register8BitSize, uint8_t> registerValue;

    uint8_t i2cMstCtrl = 0;
    if (!readReg(AddressID::I2C_MST_CTRL, i2cMstCtrl)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = i2cMstCtrl;

    constexpr uint8_t i2cMstCtrlMask = 0b1111;
    registerValue &= ~i2cMstCtrlMask;

    constexpr uint8_t i2cMstClkValue = 0x7;
    registerValue |= i2cMstClkValue;

    constexpr uint8_t i2cMstPNsrOffset = 4U;
    constexpr uint8_t i2cMstPNsrMask = 0b1;
    registerValue &= ~static_cast<uint8_t>(i2cMstPNsrMask << i2cMstPNsrOffset);

    constexpr uint8_t i2cMstPNsrValue = 1U;
    registerValue |= (i2cMstPNsrValue << i2cMstPNsrOffset);

    if (!writeReg(AddressID::I2C_MST_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    auto checkResult = checkRegister(AddressID::I2C_MST_CTRL, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    uint8_t userCtrl = 0;
    if (!readReg(AddressID::USER_CTRL, userCtrl)) {
        return ErrorCode::FailedReadIMURegister;
    }
    registerValue = userCtrl;

    constexpr uint8_t I2CMasterEnableOffset = 5U;
    constexpr uint8_t I2CMasterEnableMask = 1U;
    registerValue &= ~static_cast<uint8_t>(I2CMasterEnableMask << I2CMasterEnableOffset); // disable I2C Master

    if (enableMaster) {
        constexpr uint8_t enableI2CMaster = 1U;
        registerValue |= (enableI2CMaster << I2CMasterEnableOffset);
    } else {
        constexpr uint8_t disableI2CMaster = 0U;
        registerValue |= (disableI2CMaster << I2CMasterEnableOffset);
    }
    if (!writeReg(AddressID::USER_CTRL, registerValue.to_ulong())) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    checkResult = checkRegister(AddressID::USER_CTRL, registerValue.to_ulong());
    if (checkResult != ErrorCode::NoError) {
        return checkResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::resetMagnetometer() {
    constexpr uint8_t softReset = 1U;
    const MagnetWrite write{.reg = MAGN_ADDR_CNTL3, .value = softReset};
    auto writeResult = magnetometerWriteReg(write);

    if (writeResult != ErrorCode::NoError) {
        return writeResult;
    }

    /*
     * According to AK09916 datasheet after power-down mode is set (after reset this mode is activated), at
     * least 100 us is needed before setting another mode
     */
    constexpr uint16_t delay1Ms{1};
    Os::Task::delay(delay1Ms);

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::magnetometerWhoAmI() {
    constexpr uint8_t numOfBytesToRead = 2U;
    const MagnetRead read{.reg = MAGN_ADDR_WIA1, .length = numOfBytesToRead};
    auto readResult = magnetometerReadReg(read);

    if (readResult != ErrorCode::NoError) {
        return readResult;
    }

    etl::array<uint8_t, 2> whoAmI{0};
    if (!burstReadRegs(AddressID::EXT_SLV_SENS_DATA_00, whoAmI)) {
        return ErrorCode::FailedReadIMURegister;
    }

    if (whoAmI.at(0) != AK09916CompanyID) {
        return ErrorCode::NotInited;
    }
    if (whoAmI.at(1) != AK09916DeviceID) {
        return ErrorCode::NotInited;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::configureMagnetometer() {
    constexpr uint8_t continuousMeasurementMode4 = 0b01000;
    const MagnetWrite write{.reg = MAGN_ADDR_CNTL2, .value = continuousMeasurementMode4};
    auto writeResult = magnetometerWriteReg(write);
    if (writeResult != ErrorCode::NoError) {
        return writeResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::startMagnetometer() {
    constexpr uint8_t numOfBytesToRead = 8U;
    const MagnetRead read{.reg = MAGN_ADDR_HXL, .length = numOfBytesToRead};

    auto readResult = magnetometerReadReg(read);
    if (readResult != ErrorCode::NoError) {
        return readResult;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::magnetometerWriteReg(MagnetWrite write) {
    if (!writeReg(AddressID::I2C_SLV0_ADDR, AK09916Address)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    if (!writeReg(AddressID::I2C_SLV0_REG, write.reg)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    if (!writeReg(AddressID::I2C_SLV0_DO, write.value)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    constexpr uint8_t i2cSlave0Enable = 0x80;
    constexpr uint8_t numOfBytes = 0x1;
    if (!writeReg(AddressID::I2C_SLV0_CTRL, i2cSlave0Enable | numOfBytes)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::ICM20948::magnetometerReadReg(MagnetRead read) {
    constexpr uint8_t transferRead = 0x80;
    if (!writeReg(AddressID::I2C_SLV0_ADDR, transferRead | AK09916Address)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    if (!writeReg(AddressID::I2C_SLV0_REG, read.reg)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    constexpr uint8_t i2cSlave0Enable = 0x80;
    if (!writeReg(AddressID::I2C_SLV0_CTRL, i2cSlave0Enable | read.length)) {
        return ErrorCode::FailedWriteToIMURegister;
    }

    return ErrorCode::NoError;
}

bool Components::ICM20948::isMagnetometerOverflowed() const {
    return (getSensorOverflowStatus() == 1U);
}

ErrorCode Components::ICM20948::calibrateGyroOffsets() {
    uint32_t counter = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        etl::array<uint8_t, numOfRawDataGyroBytes> gyroDataBuffer{0};
        if (!burstReadRegs(AddressID::GYRO_XOUT_H, gyroDataBuffer)) {
            return ErrorCode::FailedReadIMURegister;
        }

        const uint8_t xGyroOutH = gyroDataBuffer.at(0);
        const uint8_t xGyroOutL = gyroDataBuffer.at(1);
        const uint8_t yGyroOutH = gyroDataBuffer.at(2);
        const uint8_t yGyroOutL = gyroDataBuffer.at(3);
        const uint8_t zGyroOutH = gyroDataBuffer.at(4);
        const uint8_t zGyroOutL = gyroDataBuffer.at(5);


        float xOffset = 
                static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(xGyroOutH) << u8bitsOffset) | xGyroOutL);
        float yOffset =
                static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(yGyroOutH) << u8bitsOffset) | yGyroOutL);
        float zOffset =
                static_cast<int16_t>(static_cast<uint16_t>(static_cast<uint16_t>(zGyroOutH) << u8bitsOffset) | zGyroOutL);

        // scale values
        xOffset = static_cast<float>(xOffset) / gyroScaleFactor;
        yOffset = static_cast<float>(yOffset) / gyroScaleFactor;
        zOffset = static_cast<float>(zOffset) / gyroScaleFactor;

        GyroXOffset += xOffset;
        GyroYOffset += yOffset;
        GyroZOffset += zOffset;
    }
    
    GyroXOffset /= 1000;
    GyroYOffset /= 1000;
    GyroZOffset /= 1000;

    return ErrorCode::NoError;
}

void Components::ICM20948::applyCalibrationParametersFreeIMU(float &x, float &y, float &z, const CalibrationParametersFreeIMU &calibrationParametersFreeIMU) {
    constexpr float numOfAxis = 3.0;
    float averageScale = (calibrationParametersFreeIMU.scaleX + calibrationParametersFreeIMU.scaleY + calibrationParametersFreeIMU.scaleZ) / numOfAxis;
    x = (x - calibrationParametersFreeIMU.offsetX) * (averageScale/calibrationParametersFreeIMU.scaleX);
    y = (y - calibrationParametersFreeIMU.offsetY) * (averageScale/calibrationParametersFreeIMU.scaleY);
    z = (z - calibrationParametersFreeIMU.offsetZ) * (averageScale/calibrationParametersFreeIMU.scaleZ);
}

void Components::ICM20948::applyCalibrationParameters(float &x, float &y, float &z, const CalibrationParameters &calibrationParameters) {
    float offsetedX = x - calibrationParameters.offsetX;
    float offsetedY = y - calibrationParameters.offsetY;
    float offsetedZ = z - calibrationParameters.offsetZ;

    float calibratedX = calibrationParameters.matrix[0][0]*offsetedX  + calibrationParameters.matrix[0][1]*offsetedY + calibrationParameters.matrix[0][2]*offsetedZ; 
    float calibratedY = calibrationParameters.matrix[1][0]*offsetedX  + calibrationParameters.matrix[1][1]*offsetedY + calibrationParameters.matrix[1][2]*offsetedZ; 
    float calibratedZ = calibrationParameters.matrix[2][0]*offsetedX  + calibrationParameters.matrix[2][1]*offsetedY + calibrationParameters.matrix[2][2]*offsetedZ; 

    x = calibratedX;
    y = calibratedY;
    z = calibratedZ;
}

}
