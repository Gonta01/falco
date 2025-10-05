// ======================================================================
// \title  ICM20948.hpp
// \author user
// \brief  hpp file for ICM20948 component implementation class
// ======================================================================

#ifndef Components_ICM20948_HPP
#define Components_ICM20948_HPP

#include "Components/ICM20948/ICM20948ComponentAc.hpp"
#include "ICM20948Interface.hpp"
#include <cstdint>
#include <etl/span.h>
#include <etl/array.h>
#include <etl/expected.h>
#include <etl/map.h>

const uint8_t dmp3_image[] = {
#include "icm20948_img.dmp3a.h"
};

constexpr uint32_t ANDROID_SENSOR_NUM_MAX = 44;

const uint16_t androidSensorToControlBits[ANDROID_SENSOR_NUM_MAX] =
{
  // Data output control 1 register bit definition
  // 16-bit accel                                0x8000
  // 16-bit gyro                                 0x4000
  // 16-bit compass                              0x2000
  // 16-bit ALS                                  0x1000
  // 32-bit 6-axis quaternion                    0x0800
  // 32-bit 9-axis quaternion + heading accuracy 0x0400
  // 16-bit pedometer quaternion                 0x0200
  // 32-bit Geomag rv + heading accuracy         0x0100
  // 16-bit Pressure                             0x0080
  // 32-bit calibrated gyro                      0x0040
  // 32-bit calibrated compass                   0x0020
  // Pedometer Step Detector                     0x0010
  // Header 2                                    0x0008
  // Pedometer Step Indicator Bit 2              0x0004
  // Pedometer Step Indicator Bit 1              0x0002
  // Pedometer Step Indicator Bit 0              0x0001
  // Unsupported Sensors are 0xFFFF
  0xFFFF, // 0  Meta Data
  0x8008, // 1  Accelerometer
  0x0028, // 2  Magnetic Field
  0x0408, // 3  Orientation
  0x4048, // 4  Gyroscope
  0x1008, // 5  Light
  0x0088, // 6  Pressure
  0xFFFF, // 7  Temperature
  0xFFFF, // 8  Proximity <----------- fixme
  0x0808, // 9  Gravity
  0x8808, // 10 Linear Acceleration
  0x0408, // 11 Rotation Vector
  0xFFFF, // 12 Humidity
  0xFFFF, // 13 Ambient Temperature
  0x2008, // 14 Magnetic Field Uncalibrated
  0x0808, // 15 Game Rotation Vector
  0x4008, // 16 Gyroscope Uncalibrated
  0x0000, // 17 Significant Motion
  0x0018, // 18 Step Detector
  0x0010, // 19 Step Counter <----------- fixme
  0x0108, // 20 Geomagnetic Rotation Vector
  0xFFFF, // 21 ANDROID_SENSOR_HEART_RATE,
  0xFFFF, // 22 ANDROID_SENSOR_PROXIMITY,
  0x8008, // 23 ANDROID_SENSOR_WAKEUP_ACCELEROMETER,
  0x0028, // 24 ANDROID_SENSOR_WAKEUP_MAGNETIC_FIELD,
  0x0408, // 25 ANDROID_SENSOR_WAKEUP_ORIENTATION,
  0x4048, // 26 ANDROID_SENSOR_WAKEUP_GYROSCOPE,
  0x1008, // 27 ANDROID_SENSOR_WAKEUP_LIGHT,
  0x0088, // 28 ANDROID_SENSOR_WAKEUP_PRESSURE,
  0x0808, // 29 ANDROID_SENSOR_WAKEUP_GRAVITY,
  0x8808, // 30 ANDROID_SENSOR_WAKEUP_LINEAR_ACCELERATION,
  0x0408, // 31 ANDROID_SENSOR_WAKEUP_ROTATION_VECTOR,
  0xFFFF, // 32 ANDROID_SENSOR_WAKEUP_RELATIVE_HUMIDITY,
  0xFFFF, // 33 ANDROID_SENSOR_WAKEUP_AMBIENT_TEMPERATURE,
  0x2008, // 34 ANDROID_SENSOR_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED,
  0x0808, // 35 ANDROID_SENSOR_WAKEUP_GAME_ROTATION_VECTOR,
  0x4008, // 36 ANDROID_SENSOR_WAKEUP_GYROSCOPE_UNCALIBRATED,
  0x0018, // 37 ANDROID_SENSOR_WAKEUP_STEP_DETECTOR,
  0x0010, // 38 ANDROID_SENSOR_WAKEUP_STEP_COUNTER,
  0x0108, // 39 ANDROID_SENSOR_WAKEUP_GEOMAGNETIC_ROTATION_VECTOR
  0xFFFF, // 40 ANDROID_SENSOR_WAKEUP_HEART_RATE,
  0x0000, // 41 ANDROID_SENSOR_WAKEUP_TILT_DETECTOR,
  0x8008, // 42 Raw Acc
  0x4048, // 43 Raw Gyr
};

namespace Components {

  class ICM20948 :
    public ICM20948ComponentBase, Interfaces::ICM20948
  {
    public:
      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ICM20948 object
      ICM20948(
          const char* const compName //!< The component name
      );

      //! Destroy ICM20948 object
      ~ICM20948();

      enum class Mode {
        NO_FUSION,
        FUSION
      };

      void configure(Mode mode);

    PRIVATE:
      struct CalibrationParametersFreeIMU {
        float scaleX;
        float scaleY;
        float scaleZ;
        float offsetX;
        float offsetY;
        float offsetZ;
      };

      struct CalibrationParameters {
        float matrix[3][3];
        float offsetX;
        float offsetY;
        float offsetZ;
      };

      //! Handler implementation for pollQuatData
      Drv::IMU::Status pollQuatData_handler(
        NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for pollRawData
      Drv::IMU::Status pollRawData_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for getAccelData
      Drv::IMU::Status getAccelData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::AccelData& accelData
      ) override;

      //! Handler implementation for getGyroData
      Drv::IMU::Status getGyroData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::GyroData& gyroData
      ) override;

      //! Handler implementation for getMagnData
      Drv::IMU::Status getMagnData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::MagnData& magnData
      ) override;

      //! Handler implementation for getQuatData
      Drv::IMU::Status getQuatData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::QuatData& quatData
      ) override;
      
      //! Handler implementation for getCalibrationData
      Drv::IMU::Status getCalibrationData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::CalibrationData& calibData
      ) override;

      /**
       * Function to handle driver error and create event reports
       */
      void errorReport(ErrorCode errorCode);

      /**
      * Initialization of IMU to retrieve raw data 
      * @return error code
      */
      [[nodiscard]] ErrorCode rawInit();

      /**
      * Initialization of IMU to retrieve quaternion data with the usage of DMP 
      * @return error code
      */
      [[nodiscard]] ErrorCode dmpInit();

      /**
      * Initialization of DMP (Digital Motion Processor)
      * @return error code
      */
      [[nodiscard]] ErrorCode initializeDMP();
      
      /**
       * Enable DMP orientation sensor
       * @return error code
       */
      [[nodiscard]] ErrorCode enableDMPOrientationSensor();

      /**
       * Enable DMP orientation sensor
       * @return error code
       */
      [[nodiscard]] ErrorCode setDMPODRrate();

      /**
       * Polling of Inertial measurement unit's raw data from FIFO.
       * @return true if data is available, otherwise false
       */
      [[nodiscard]] bool pollRawData();

      /**
       * Polling of Inertial measurement unit's DMP data from FIFO.
       * @return true if data is available, otherwise false
       */
      [[nodiscard]] bool pollDMPData();

      /**
       * Get acceleration data from IMU
       * @return 3-axis acceleration in m/s^2, otherwise - error code
       */
      etl::expected<AccelData, ErrorCode> getAccelData();

      /**
       * Get angular velocity data from IMU
       * @return 3-axis angular velocity in degrees per second, otherwise - error code
       */
      etl::expected<GyroData, ErrorCode> getGyroData();

      /**
       * Get magnetic field data from IMU
       * @return 3-axis magnetic field in microTesla, otherwise - error code
       */
      etl::expected<MagnData, ErrorCode> getMagneticData();

      /**
       * Get quaternion data from IMU
       * @return quaternion , otherwise - error code
       */
      etl::expected<Quaternion, ErrorCode> getQuatData();
      /**
       * AK09916 datasheet states that magnetic sensor may overflow even though measurement data register is not saturated.
       * If magnetic sensor is overflowed, measurement data is not correct.
       * @return true, if magnetometer overflowed, otherwise - false
       */
      bool isMagnetometerOverflowed() const;

      /**
       * Get temperature data from IMU
       * @return temperature in °C, otherwise - error code
       */
      etl::expected<float, ErrorCode> getTemperatureData() const;

      bool retrieveQuaternionData = false;

      static constexpr U32 writeTimeout = 0xFFFF'FFFF;
      static constexpr U32 readTimeout = 0xFFFF'FFFF;
      
      bool initializationComplete{false};
      Drv::IMU::CalibrationData calibrationData{0,0,0, 0,0,0, 0,0,0};
      AccelData accelDataResult{.0F, .0F, .0F};
      GyroData gyroDataResult{.0F, .0F, .0F};
      MagnData magneticDataResult{.0F, .0F, .0F};
      Quaternion quaternionResult{0, 0, 0, 0};
      static constexpr double quaternionScaler{1073741824.0};
      
      float GyroXOffset{0.0F};
      float GyroYOffset{0.0F};
      float GyroZOffset{0.0F};

      float temperatureResult{0.F};

      static constexpr uint8_t expectedWhoAmI{0xEA};

      float gyroScaleFactor{0.0F};
      float accelScaleFactor{0.0F};
      float gravitationalAcceleration = 9.80665;

      static constexpr float temperatureSensitivity{333.87F}; // Condition: Untrimmed
      static constexpr float temperatureOffset{0.0F};         // Condition: 21°C
      static constexpr float temperatureConstant{21.0F};      // According to datasheet

      static constexpr uint8_t sampleSize{22U};
      static constexpr uint8_t numOfSamplesInBuffer{1U};
      static constexpr uint32_t packetBufferSize{2*sampleSize};

      static constexpr uint8_t maxBytesFromDMP{14};
      static constexpr uint8_t dmpHeaderBytes{2};
      static constexpr uint8_t dmpQuat9Bytes{14};

      uint16_t header1DMPData{0};
      uint16_t header2DMPData{0};

      etl::array<uint8_t, packetBufferSize> packetBuffer{0};

      etl::array<U32, 256> numOfElementsInFifoBuffer{0};

      static constexpr uint8_t register8BitSize{8U};
      static constexpr uint8_t u8bitsOffset{8U};
      static constexpr uint8_t u16bitsOffset{16U};
      static constexpr uint8_t u24bitsOffset{24U};

      static constexpr uint16_t dmpStartAddress {0x1000};
      static constexpr uint16_t dmpMemBankSize  {256};
      static constexpr uint8_t dmpLoadStart    {0x90};
      static constexpr uint32_t maxDMPFirmwareBytesForTransaction = 16;

      uint32_t enabledAndroid0; // Keep track of which Android sensors are enabled: 0-31
      uint32_t enabledAndroid1; // Keep track of which Android sensors are enabled: 32-
      // Determines which base sensor needs to be on based upon ANDROID_SENSORS 0-31
      static constexpr int32_t INV_NEEDS_ACCEL_MASK {static_cast<int32_t>((1L << 1) | (1L << 3) | (1L << 9) | (1L << 10) | (1L << 11) | (1L << 15) | (1L << 17) | (1L << 18) | (1L << 19) | (1L << 20) | (1L << 23) | (1L << 25) | (1L << 29) | (1L << 30) | (1L << 31))};
      static constexpr int32_t INV_NEEDS_GYRO_MASK {static_cast<int32_t>((1L << 3) | (1L << 4) | (1L << 9) | (1L << 10) | (1L << 11) | (1L << 15) | (1L << 16) | (1L << 25) | (1L << 26) | (1L << 29) | (1L << 30) | (1L << 31))};
      static constexpr int32_t INV_NEEDS_COMPASS_MASK {static_cast<int32_t>((1L << 2) | (1L << 3) | (1L << 11) | (1L << 14) | (1L << 20) | (1L << 24) | (1L << 25) | (1L << 31))};
      static constexpr int32_t INV_NEEDS_PRESSURE {(1L << 6) | (1L << 28)};

      // Determines which base sensor needs to be on based upon ANDROID_SENSORS 32-
      static constexpr int32_t INV_NEEDS_ACCEL_MASK1 {(1L << 3) | (1L << 5) | (1L << 6) | (1L << 7) | (1L << 9) | (1L << 10)}; // I.e. 35, 37, 38, 39, 41, 42
      static constexpr int32_t INV_NEEDS_GYRO_MASK1 {(1L << 3) | (1L << 4) | (1L << 11)}; // I.e. 35, 36, 43
      static constexpr int32_t INV_NEEDS_COMPASS_MASK1 {(1L << 2) | (1L << 7)}; // I.e. 34 and 39
      
      static constexpr uint32_t DMPDataReadyAccel = 0x0002;                        // Accel samples available
      static constexpr uint32_t DMPMotionEventControlAccelCalibr = 0x0200; 
      static constexpr uint32_t DMP_Data_ready_Gyro = 0x0001;                      // Gyro samples available
      static constexpr uint32_t DMP_Motion_Event_Control_Gyro_Calibr = 0x0100; 
      static constexpr uint32_t DMP_Data_ready_Secondary_Compass = 0x0008;         // Secondary compass samples available
      static constexpr uint32_t DMP_Motion_Event_Control_Compass_Calibr = 0x0080; 
      static constexpr uint32_t DMP_Data_Output_Control_1_Accel = 0x8000;          // 16-bit accel
      static constexpr uint32_t DMP_Data_Output_Control_1_Gyro_Calibr = 0x0040;    // 32-bit calibrated gyro
      static constexpr uint32_t DMP_Data_Output_Control_1_Gyro = 0x4000;           // 16-bit gyro
      static constexpr uint32_t DMP_Data_Output_Control_1_Compass_Calibr = 0x0020; // 32-bit calibrated compass
      static constexpr uint32_t DMP_Data_Output_Control_1_Compass = 0x2000;        // 16-bit compass
      static constexpr uint32_t DMP_Data_Output_Control_1_Quat9 = 0x0400;          // 32-bit 9-axis quaternion + heading accuracy
      static constexpr uint32_t DMP_Data_Output_Control_1_Geomag = 0x0100;         // 32-bit Geomag rv + heading accuracy
      static constexpr uint32_t DMP_Data_Output_Control_1_Step_Ind_0 = 0x0001;     // Pedometer Step Indicator Bit 0
      static constexpr uint32_t DMP_Data_Output_Control_1_Step_Ind_1 = 0x0002;     // Pedometer Step Indicator Bit 1
      static constexpr uint32_t DMP_Data_Output_Control_1_Step_Ind_2 = 0x0004;     // Pedometer Step Indicator Bit 2
      static constexpr uint32_t DMP_Data_Output_Control_1_Step_Detector = 0x0010;  // Pedometer Step Detector
      static constexpr uint32_t DMP_Data_Output_Control_2_Accel_Accuracy = 0x4000;
      static constexpr uint32_t DMP_Data_Output_Control_2_Gyro_Accuracy = 0x2000;
      static constexpr uint32_t DMP_Data_Output_Control_2_Compass_Accuracy = 0x1000;

      static constexpr uint32_t DMP_Motion_Event_Control_9axis = 0x0040;
      static constexpr uint32_t DMP_Motion_Event_Control_Pedometer_Interrupt = 0x2000;
      static constexpr uint32_t DMP_Motion_Event_Control_Geomag = 0x0008; // Geomag rv

      static constexpr uint32_t DMP_header_bitmap_Header2 = 0x0008;
      static constexpr uint32_t DMP_header_bitmap_Step_Detector = 0x0010;
      static constexpr uint32_t DMP_header_bitmap_Compass_Calibr = 0x0020;
      static constexpr uint32_t DMP_header_bitmap_Gyro_Calibr = 0x0040;
      static constexpr uint32_t DMP_header_bitmap_Pressure = 0x0080;
      static constexpr uint32_t DMP_header_bitmap_Geomag = 0x0100;
      static constexpr uint32_t DMP_header_bitmap_PQuat6 = 0x0200;
      static constexpr uint32_t DMP_header_bitmap_Quat9 = 0x0400;
      static constexpr uint32_t DMP_header_bitmap_Quat6 = 0x0800;
      static constexpr uint32_t DMP_header_bitmap_ALS = 0x1000;
      static constexpr uint32_t DMP_header_bitmap_Compass = 0x2000;
      static constexpr uint32_t DMP_header_bitmap_Gyro = 0x4000;
      static constexpr uint32_t DMP_header_bitmap_Accel = 0x8000;

      // Accel FSR
      // The DMP scales accel raw data internally to align 1g as 2^25.
      // To do this and output hardware unit again as configured FSR, write 0x4000000 to ACC_SCALE DMP register, and write 0x40000 to ACC_SCALE2 DMP register.
      static constexpr uint32_t ACC_SCALE  {30 * 16 + 0};   // 32-bit: Write accel scaling value for internal use
      static constexpr uint32_t ACC_SCALE2 {79 * 16 + 4};  // 32-bit: Write accel scaling down value
      
      // mounting matrix: all 32-bit
      static constexpr uint32_t CPASS_MTX_00 {23 * 16};      // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_01 {23 * 16 + 4};  // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_02 {23 * 16 + 8};  // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_10 {23 * 16 + 12}; // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_11 {24 * 16};      // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_12 {24 * 16 + 4};  // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_20 {24 * 16 + 8};  // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_21 {24 * 16 + 12}; // Compass mount matrix and scale
      static constexpr uint32_t CPASS_MTX_22 {25 * 16};      // Compass mount matrix and scale
      
      // B2S mounting matrix
      static constexpr uint32_t B2S_MTX_00 {208 * 16};
      static constexpr uint32_t B2S_MTX_01 {208 * 16 + 4};
      static constexpr uint32_t B2S_MTX_02 {208 * 16 + 8};
      static constexpr uint32_t B2S_MTX_10 {208 * 16 + 12};
      static constexpr uint32_t B2S_MTX_11 {209 * 16};
      static constexpr uint32_t B2S_MTX_12 {209 * 16 + 4};
      static constexpr uint32_t B2S_MTX_20 {209 * 16 + 8};
      static constexpr uint32_t B2S_MTX_21 {209 * 16 + 12};
      static constexpr uint32_t B2S_MTX_22 {210 * 16};

      // gains
      static constexpr uint32_t ACCEL_ONLY_GAIN{16 * 16 + 12}; // 32-bit: 15252014 (225Hz) 30504029 (112Hz) 61117001 (56Hz)
      static constexpr uint32_t GYRO_SF{19 * 16};              // 32-bit: gyro scaling factor

      // Gyro FSR
      static constexpr uint32_t GYRO_FULLSCALE{72 * 16 + 12};

      // parameters for accel calibration
      static constexpr uint32_t ACCEL_ALPHA_VAR {91 * 16};    // 32-bit: 1026019965 (225Hz) 977872018 (112Hz) 882002213 (56Hz)
      static constexpr uint32_t ACCEL_CAL_RATE {94 * 16 + 4}; // 16-bit: 0 (225Hz, 112Hz, 56Hz)
      static constexpr uint32_t ACCEL_A_VAR {92 * 16};        // 32-bit: 47721859 (225Hz) 95869806 (112Hz) 191739611 (56Hz)

      // Compass Cal params to be adjusted according to sampling rate
      static constexpr uint32_t CPASS_TIME_BUFFER{112 * 16 + 14};
      
      // data output control
      static constexpr uint32_t   DATA_OUT_CTL1{4 * 16};         // 16-bit: Data output control 1 register : configure DMP to output required data
      static constexpr uint32_t   DATA_OUT_CTL2 {4 * 16 + 2};    // 16-bit: Data output control 2 register : configure the BM, accel/gyro/compass accuracy and gesture such as Pick-up
      static constexpr uint32_t   DATA_RDY_STATUS {8 * 16 + 10}; // 16-bit: indicates to DMP which sensors are available
      static constexpr uint32_t   MOTION_EVENT_CTL{4 * 16 + 14}; // 16-bit: configure DMP for Android L and Invensense specific features

      static constexpr uint32_t   ODR_QUAT9       {10 * 16 + 8};       // ODR_QUAT9 Register for 9-axis quaternion ODR
      static constexpr uint32_t   ODR_CNTR_QUAT9  {8 * 16 + 8};   // ODR_CNTR_QUAT9 Register for 9-axis quaternion ODR counter

      /**
       * User Bank 0-3
       */
      static constexpr uint8_t ADDR_REG_BANK_SEL{0x7F};

      /**
       * User Bank 0 registers
       */
      static constexpr uint8_t ADDR_WHO_AM_I{0x00};
      static constexpr uint8_t ADDR_USER_CTRL{0x03};
      static constexpr uint8_t ADDR_LP_CONFIG{0x05};
      static constexpr uint8_t ADDR_PWR_MGMT_1{0x06};
      static constexpr uint8_t ADDR_PWR_MGMT_2{0x07};
      static constexpr uint8_t ADDR_INT_PIN_CFG{0x0F};
      static constexpr uint8_t ADDR_INT_ENABLE_1{0x11};
      static constexpr uint8_t ADDR_INT_ENABLE_2{0x12};
      static constexpr uint8_t ADDR_INT_STATUS{0x19};
      static constexpr uint8_t ADDR_INT_STATUS_1{0x1A};
      static constexpr uint8_t ADDR_INT_STATUS_2{0x1B};
      static constexpr uint8_t ADDR_INT_STATUS_3{0x1C};

      static constexpr uint8_t ADDR_SINGLE_FIFO_PRIORITY_SEL{0x26}; // undocumented register for DMP

      static constexpr uint8_t ADDR_ACCEL_XOUT_H{0x2D};
      static constexpr uint8_t ADDR_ACCEL_XOUT_L{0x2E};
      static constexpr uint8_t ADDR_ACCEL_YOUT_H{0x2F};
      static constexpr uint8_t ADDR_ACCEL_YOUT_L{0x30};
      static constexpr uint8_t ADDR_ACCEL_ZOUT_H{0x31};
      static constexpr uint8_t ADDR_ACCEL_ZOUT_L{0x32};
      static constexpr uint8_t numOfRawDataAccelBytes{6};

      static constexpr uint8_t ADDR_GYRO_XOUT_H{0x33};
      static constexpr uint8_t ADDR_GYRO_XOUT_L{0x34};
      static constexpr uint8_t ADDR_GYRO_YOUT_H{0x35};
      static constexpr uint8_t ADDR_GYRO_YOUT_L{0x36};
      static constexpr uint8_t ADDR_GYRO_ZOUT_H{0x37};
      static constexpr uint8_t ADDR_GYRO_ZOUT_L{0x38};
      static constexpr uint8_t numOfRawDataGyroBytes{6};

      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_00{0x3B};
      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_01{0x3C};
      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_02{0x3D};
      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_03{0x3E};
      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_04{0x3F};
      static constexpr uint8_t ADDR_EXT_SLV_SENS_DATA_05{0x40};
      static constexpr uint8_t numOfRawDataMagnBytes{6};


      static constexpr uint8_t ADDR_FIFO_EN_1{0x66};
      static constexpr uint8_t ADDR_FIFO_EN_2{0x67};
      static constexpr uint8_t ADDR_FIFO_RST{0x68};
      static constexpr uint8_t ADDR_FIFO_MODE{0x69};
      static constexpr uint8_t ADDR_FIFO_COUNTH{0x70};
      static constexpr uint8_t numOfFIFOCountRegs{2};

      static constexpr uint8_t ADDR_FIFO_R_W{0x72};
      static constexpr uint8_t ADDR_HW_FIX_DISABLE{0x75}; // undocumented register for DMP

      static constexpr uint8_t ADDR_MEM_START_ADDR  {0x7C}; // undocumented register for DMP
      static constexpr uint8_t ADDR_MEM_R_W         {0x7D}; // undocumented register for DMP
      static constexpr uint8_t ADDR_MEM_BANK_SEL    {0x7E}; // undocumented register for DMP

      /**
       * User Bank 1 registers
       */
      static constexpr uint8_t ADDR_TIMEBASE_CORRECTION_PLL{0x28};

      /**
       * User Bank 2 registers
       */
      static constexpr uint8_t ADDR_GYRO_SMPLRT_DIV{0x00};
      static constexpr uint8_t ADDR_GYRO_CONFIG_1{0x01};
      static constexpr uint8_t ADDR_ACCEL_SMPLRT_DIV_1{0x10};
      static constexpr uint8_t ADDR_ACCEL_SMPLRT_DIV_2{0x11};
      static constexpr uint8_t ADDR_ACCEL_CONFIG{0x14};
      static constexpr uint8_t ADDR_ACCEL_CONFIG_2{0x15};

      static constexpr uint8_t ADDR_PRGM_START_ADDRH{0x50}; // undocumented register for DMP
      static constexpr uint8_t ADDR_PRGM_START_ADDRL{0x51}; // undocumented register for DMP

      /**
       * User Bank 3 registers
       */
      static constexpr uint8_t ADDR_I2C_MST_ODR_CONFIG{0x00};
      static constexpr uint8_t ADDR_I2C_MST_CTRL{0x01};
      static constexpr uint8_t ADDR_I2C_SLV0_ADDR{0x03};
      static constexpr uint8_t ADDR_I2C_SLV0_REG{0x04};
      static constexpr uint8_t ADDR_I2C_SLV0_CTRL{0x05};
      static constexpr uint8_t ADDR_I2C_SLV0_DO{0x06};

      static constexpr uint8_t ADDR_I2C_SLV1_ADDR{0x07};
      static constexpr uint8_t ADDR_I2C_SLV1_REG{0x08};
      static constexpr uint8_t ADDR_I2C_SLV1_CTRL{0x09};
      static constexpr uint8_t ADDR_I2C_SLV1_DO{0x0A};
            
      uint16_t bytesAvailable;
      uint16_t numOfSamples;
      U32 overflowCnt = 0;
      
      /**
       * Who am I check of ICM-20948
       * @return error code
       */
      ErrorCode isConnected();

      /**
       * Perform reset of ICM-20948.
       * All ICM-20948 registers will be set to reset values
       * @return error code
       */
      ErrorCode deviceReset();

      /**
       * Configure the internal system clock source be automatically selected based on the power mode.
       * @return error code
       */
      ErrorCode configClockSource();

      /**
       * Enable gyroscope and accelerometer
       * @return error code
       */
      ErrorCode enableGyroAndAccel();

      /**
       * Function allows to enable and disable ICM-20948 FIFO buffer
       * @param activate - true, if enable FIFO, otherwise - false
       * @return error code
       */
      ErrorCode enableFIFO(bool activate);

      /**
       * Function allows to enable ICM-20948 DMP
       * @return error code
       */
      ErrorCode enableDMP();

      /**
       * Function allows to disable ICM-20948 DMP
       * @return error code
       */
      ErrorCode disableDMP();

      /**
       * Function allows to reset ICM-20948 DMP
       * @return error code
       */
      ErrorCode resetDMP();

      /**
       * Function to configure accelerometer and gyroscope of IMU
       * Configurations includes enabling of DLPF, choice of full scale range, low pass filter configuration
       * @param accelScale - accel full scale
       * @param gyroScale - gyro full scale
       * @return error code
       */
      ErrorCode setConfig(AccelFullScale accelScale, GyroFullScale gyroScale);

      /**
       * Function to configure gyroscope of IMU
       * @param scale - gyro full scale
       * @return error code
       */
      ErrorCode configureGyroscope(GyroFullScale scale);

      /**
       * Function to configure accelerometer of IMU
       * @param scale - accel full scale
       * @return error code
       */
      ErrorCode configureAccelerometer(AccelFullScale scale);

      /**
       * Function to configure gyroscope sample rate by choosing sample rate divider
       * This functions is only effective when gyro DLPF is enabled
       * @return error code
       */
      ErrorCode configureGyroSampleRate();

      /**
       * Function to configure accelerometer sample rate by choosing sample rate divider
       * This functions is only effective when accelerometer DLPF is enabled
       * @return error code
       */
      ErrorCode configureAccelSampleRate();

      /**
       * ICM-20948 FIFO buffer configuration
       * @param enableInterrupt - enable interrupt for FIFO overflow to propagate to interrupt pin 1.
       * @return error code
       */
      ErrorCode configureFIFO(bool enableInterrupt);

      /**
       * Configure data ready interrupt
       * @param activate - enable or disable data ready interrupt
       * @return error code
       */
      ErrorCode configureDataReadyInterrupt(bool activate);

      /**
       * Configure interrupt pin
       * @param logicLevel - false - logic level is active high, true - logic level is active low
       * @param open       - false - push-pull, true open drain
       * @param latchEn    - false - pin indicateds interrupt pulse is width 50us, true - pin level held until interrupt status is cleared
       * @return error code
       */
      ErrorCode configureInterruptPin(bool logicLevel, bool open, bool latchEn);

      /**
       * Clear interrupt by reading interrupt registers
       */
      ErrorCode clearInterruptStatus();

      /**
       * Wakeup chip from sleep mode;
       * @return error code
       */
      ErrorCode wakeup();
      
      /**
       * Activate sleep mode of the chip;
       * @return error code
       */
      ErrorCode sleep();

      /**
       * Enable low power mode
       * @return error code
       */
      ErrorCode lowPowerEnable();

      /**
       * Disable low power mode
       * @return error code
       */
      ErrorCode lowPowerDisable();

      /**
       * Reset FIFO
       * @return error code
       */
      ErrorCode resetFIFO();

      /**
       * Check FIFO, retrieve data if available and clear FIFO during data polling
       * @return
       */
      bool checkAndUpdateFIFO();

      /**
       * Handling of acceleration data retrieved from FIFO
       */
      void accelDataHandling();

      /**
       * Handling of gyroscope data retrieved from FIFO
       */
      void gyroDataHandling();

      /**
       * Handling of temperature data retrieved from FIFO
       */
      void temperatureDataHandling();

      /**
       * Handling of magnetic data retrieved from FIFO
       */
      void magneticDataHandling();

      uint8_t devAddr{0x68};
      RegisterBankSel activatedCurrentBank = RegisterBankSel::Common;
      
      CalibrationParametersFreeIMU calibrationParametersFreeIMU;
      CalibrationParameters calibrationParameters;

      static constexpr uint8_t numOfAddresses{43};
      etl::map<AddressID, BankWithAddress, numOfAddresses> mapOfAddresses = {
        {            AddressID::WHO_AM_I,             {RegisterBankSel::UserBank0, ADDR_WHO_AM_I}},
        {           AddressID::USER_CTRL,            {RegisterBankSel::UserBank0, ADDR_USER_CTRL}},
        {           AddressID::LP_CONFIG,            {RegisterBankSel::UserBank0, ADDR_LP_CONFIG}},
        {          AddressID::PWR_MGMT_1,           {RegisterBankSel::UserBank0, ADDR_PWR_MGMT_1}},
        {          AddressID::PWR_MGMT_2,           {RegisterBankSel::UserBank0, ADDR_PWR_MGMT_2}},
        {         AddressID::INT_PIN_CFG,          {RegisterBankSel::UserBank0, ADDR_INT_PIN_CFG}},
        {        AddressID::INT_ENABLE_1,         {RegisterBankSel::UserBank0, ADDR_INT_ENABLE_1}},
        {        AddressID::INT_ENABLE_2,         {RegisterBankSel::UserBank0, ADDR_INT_ENABLE_2}},
        {        AddressID::INT_STATUS,           {RegisterBankSel::UserBank0, ADDR_INT_STATUS}},
        {        AddressID::INT_STATUS_1,         {RegisterBankSel::UserBank0, ADDR_INT_STATUS_1}},
        {        AddressID::INT_STATUS_2,         {RegisterBankSel::UserBank0, ADDR_INT_STATUS_2}},
        {        AddressID::INT_STATUS_3,         {RegisterBankSel::UserBank0, ADDR_INT_STATUS_3}},
        {AddressID::SINGLE_FIFO_PRIORITY_SEL, {RegisterBankSel::UserBank0, ADDR_SINGLE_FIFO_PRIORITY_SEL}},
        {        AddressID::ACCEL_XOUT_H,         {RegisterBankSel::UserBank0, ADDR_ACCEL_XOUT_H}},
        {         AddressID::GYRO_XOUT_H,          {RegisterBankSel::UserBank0, ADDR_GYRO_XOUT_H}},
        {AddressID::EXT_SLV_SENS_DATA_00, {RegisterBankSel::UserBank0, ADDR_EXT_SLV_SENS_DATA_00}},
        {           AddressID::FIFO_EN_1,            {RegisterBankSel::UserBank0, ADDR_FIFO_EN_1}},
        {           AddressID::FIFO_EN_2,            {RegisterBankSel::UserBank0, ADDR_FIFO_EN_2}},
        {            AddressID::FIFO_RST,             {RegisterBankSel::UserBank0, ADDR_FIFO_RST}},
        {           AddressID::FIFO_MODE,            {RegisterBankSel::UserBank0, ADDR_FIFO_MODE}},
        {         AddressID::FIFO_COUNTH,          {RegisterBankSel::UserBank0, ADDR_FIFO_COUNTH}},
        {            AddressID::FIFO_R_W,             {RegisterBankSel::UserBank0, ADDR_FIFO_R_W}},
        {      AddressID::HW_FIX_DISABLE,       {RegisterBankSel::UserBank0, ADDR_HW_FIX_DISABLE}},
        {      AddressID::MEM_START_ADDR,       {RegisterBankSel::UserBank0, ADDR_MEM_START_ADDR}},
        {             AddressID::MEM_R_W,              {RegisterBankSel::UserBank0, ADDR_MEM_R_W}},
        {        AddressID::MEM_BANK_SEL,         {RegisterBankSel::UserBank0, ADDR_MEM_BANK_SEL}},

        {AddressID::TIMEBASE_CORRECTION_PLL, {RegisterBankSel::UserBank1, ADDR_TIMEBASE_CORRECTION_PLL}},

        {     AddressID::GYRO_SMPLRT_DIV,      {RegisterBankSel::UserBank2, ADDR_GYRO_SMPLRT_DIV}},
        {       AddressID::GYRO_CONFIG_1,        {RegisterBankSel::UserBank2, ADDR_GYRO_CONFIG_1}},
        {  AddressID::ACCEL_SMPLRT_DIV_2,   {RegisterBankSel::UserBank2, ADDR_ACCEL_SMPLRT_DIV_2}},
        {        AddressID::ACCEL_CONFIG,         {RegisterBankSel::UserBank2, ADDR_ACCEL_CONFIG}},
        {    AddressID::PRGM_START_ADDRH,     {RegisterBankSel::UserBank2, ADDR_PRGM_START_ADDRH}},
        {    AddressID::PRGM_START_ADDRL,     {RegisterBankSel::UserBank2, ADDR_PRGM_START_ADDRL}},

        {  AddressID::I2C_MST_ODR_CONFIG,   {RegisterBankSel::UserBank3, ADDR_I2C_MST_ODR_CONFIG}},
        {        AddressID::I2C_MST_CTRL,         {RegisterBankSel::UserBank3, ADDR_I2C_MST_CTRL}},
        {       AddressID::I2C_SLV0_ADDR,        {RegisterBankSel::UserBank3, ADDR_I2C_SLV0_ADDR}},
        {        AddressID::I2C_SLV0_REG,         {RegisterBankSel::UserBank3, ADDR_I2C_SLV0_REG}},
        {       AddressID::I2C_SLV0_CTRL,        {RegisterBankSel::UserBank3, ADDR_I2C_SLV0_CTRL}},
        {         AddressID::I2C_SLV0_DO,          {RegisterBankSel::UserBank3, ADDR_I2C_SLV0_DO}},
        {       AddressID::I2C_SLV1_ADDR,        {RegisterBankSel::UserBank3, ADDR_I2C_SLV1_ADDR}},
        {        AddressID::I2C_SLV1_REG,         {RegisterBankSel::UserBank3, ADDR_I2C_SLV1_REG}},
        {       AddressID::I2C_SLV1_CTRL,        {RegisterBankSel::UserBank3, ADDR_I2C_SLV1_CTRL}},
        {         AddressID::I2C_SLV1_DO,          {RegisterBankSel::UserBank3, ADDR_I2C_SLV1_DO}},
      };

      /**
       * Check validity of address map
       * @return true, if valid, otherwise - false
       */
      ErrorCode checkMapValidity();

      /**
      * Read from ICM-20948 register
      * @param reg - register to read from
      * @param value - value of register
      * @return true, if read successfully; otherwise - false
      */
      bool readReg(AddressID reg, uint8_t &value);

      /**
      * Burst read from ICM-20948 registers.
      * The amount of read registers is equal to the size of passed buffer
      * @param reg - start register
      * @param buff - buffer where read data will be stored
      * @return true, if read successfully; otherwise - false
      */
      bool burstReadRegs(AddressID reg, etl::span<uint8_t> buff);

      /**
      * Write to ICM-20948 register
      * @param reg - register to write to
      * @param value - value, that is written to register
      * @return true, if write successfully; otherwise - false
      */
      bool writeReg(AddressID reg, uint8_t value);

      /**
      * Burst write to ICM-20948 registers.
      * The amount of written registers is equal to the size of passed buffer
      * @param reg - start register
      * @param buff - buffer that contains data for transmission. Constraint: max size = 16
      * @return true, if write successfully; otherwise - false
      */
      bool burstWriteRegs(AddressID reg, etl::span<uint8_t> buff);

      /**
      * Select register bank of ICM-20948
      * @param regBank
      * @return true, if selected successfully; otherwise - false
      */
      bool selectRegisterBank(RegisterBankSel regBank);

      /**
       * Reset register bank, so it will be set for sure during next selectRegisterBank call
       */
      void resetRegisterBank();

      /**
      * Function to check that written value to the register is equal to expected value
      * @param addressID - register address
      * @param expectedValue - value that expected to be stored there
      * @return error code
      */
      ErrorCode checkRegister(AddressID addressID, uint8_t expectedValue);

      /**
      * Magnetometer initialization
      * @return error code
      */
      ErrorCode initMagnetometer();

      /**
       * Get magnetometer sensitivity
       * @return magnetometer sensitivity
       */
      float getSensitivity();

      /**
       * Set sensor overflow status
       * @param status
       */
      void setSensorOverflowStatus(uint8_t status);

      /**
       * Get sensor overflow status
       * @return
       */
      uint8_t getSensorOverflowStatus() const;

      static constexpr uint8_t AK09916Address{0xC};
      static constexpr uint8_t AK09916CompanyID{0x48};
      static constexpr uint8_t AK09916DeviceID{0x9};
      static constexpr float AK09916Sensitivity{0.15F};
      uint8_t AK09916Status2SensorOverflow{0};

      /**
      * Magnetometer addresses
      */
      static constexpr uint8_t MAGN_ADDR_WIA1{0x00};
      static constexpr uint8_t MAGN_ADDR_WIA2{0x01};
      static constexpr uint8_t MAGN_ADDR_RSV2{0x3};
      static constexpr uint8_t MAGN_ADDR_ST1{0x10};
      static constexpr uint8_t MAGN_ADDR_HXL{0x11};
      static constexpr uint8_t MAGN_ADDR_ST2{0x18};
      static constexpr uint8_t MAGN_ADDR_CNTL2{0x31};
      static constexpr uint8_t MAGN_ADDR_CNTL3{0x32};

      /**
      * Function that allow to go into ‘bypass mode’ (I2C master interface is disabled)
      * @param enableBypass - enable or disable ‘bypass mode’
      * @return error code
      */
      ErrorCode i2cMasterBypass(bool enableBypass);

      /**
      * Configure I2C master module
      * @param enableMaster - turn on or turn off I2C master
      * @return error code
      */
      ErrorCode i2cMasterConfigure(bool enableMaster);

      /**
      * Perform soft reset of the magnetometer AK09916
      * @return error code
      */
      ErrorCode resetMagnetometer();

      /**
      * Perform Who am I check of the magnetometer AK09916
      * @return error code
      */
      ErrorCode magnetometerWhoAmI();

      /**
      * Configure operation mode magnetometer AK09916
      * @return error code
      */
      ErrorCode configureMagnetometer();

      /**
      * While reading data from magnetometer AK09916 it is important to be sure to read ST2 register at the end.
      * As a result the following AK09916 registers are read: HXL, HXH, HYL, HYH, HZL, HZH, TMPS(dummy register), ST2
      * @return error code
      */
      ErrorCode startMagnetometer();

      /**
      * Function to write value to the magnetometer AK09916 register
      * @param reg - magnetometer register
      * @param value - value to be written
      * @return error code
      */
      ErrorCode magnetometerWriteReg(MagnetWrite write);

      /**
      * Function to read from magnetometer AK09916 registers
      * After call of this function data of read registers is stored in EXT_SLV_SENS_DATA_00 -
      * EXT_SLV_SENS_DATA_23 registers of ICM-20948
      * @param reg - start register
      * @param len - number of registers to read
      * @return error code
      */
      ErrorCode magnetometerReadReg(MagnetRead read);

      /**
       * Set DMP start address
       * @param address - DMP start address
       * @return error code
       */
      ErrorCode setDMPStartAddress(uint16_t address = dmpStartAddress);

      /**
       * Load firmware into DMP
       * @return error code
       */
      ErrorCode loadDMPFirmware();
      
      /**
       * Write to DMP memory
       * @param reg - value that contains selected register bank and selected start address
       * @param length - length of data to be written
       * @param data - pointer on start of data to be written
       * @return error code
       */
      ErrorCode writeDMPMemory(uint16_t reg, etl::span<uint8_t> buff);

      /**
       * Read from DMP memory
       * @param reg - value that contains selected register bank and selected start address
       * @param length - length of data to be read
       * @param data - pointer on start of data to be read to
       * @return error code
       */
      ErrorCode readDMPMemory(uint16_t reg, etl::span<uint8_t> buff);
      
      /**
       * Configuring gyro gain
       * @param div - divider
       * @param gyroLevel - gyro level
       * @return error code
       */
      ErrorCode setGyroSF(uint8_t div, int32_t gyroLevel);

      /**
      * Function to configure to do the ten byte reading from AK09916
      * @return error code
      */
      ErrorCode configureMagnReadingForDMP();

      /**
      * Function to configure single measurement triggering for AK09916
      * @return error code
      */
      ErrorCode configureMagnMeasurementForDMP();

      /*!
       * Calibrate gyroscope offsets to adjust for inherent biases.
       * This function determines the necessary offsets for each axis of the gyroscope
       * to ensure accurate angular velocity measurements.
       * @return ErrorCode indicating the success or failure of the calibration process.
       */
      ErrorCode calibrateGyroOffsets();

      /*!
       * Apply freeIMU calibration parameters to the given x, y, and z values.
       * The input values are modified in-place by applying the scaling and offsets defined
       * in the passed CalibrationParametersFreeIMU structure.
       * @param x Reference to the x-axis measurement to be adjusted.
       * @param y Reference to the y-axis measurement to be adjusted.
       * @param z Reference to the z-axis measurement to be adjusted.
       * @param calibrationParametersFreeIMU The calibration parameters containing the
       * scale and offset values for x, y, and z.
       */
      void applyCalibrationParametersFreeIMU(float &x, float &y, float &z, const CalibrationParametersFreeIMU &calibrationParametersFreeIMU);

      /*!
       * Apply calibration parameters to the specified x, y, and z axis values.
       * This function modifies the input values in-place using the calibration parameters
       * provided, which include a transformation matrix and offsets for each axis.
       *
       * @param x Reference to the x-axis value to be calibrated.
       * @param y Reference to the y-axis value to be calibrated.
       * @param z Reference to the z-axis value to be calibrated.
       * @param calibrationParameters Calibration parameters containing the transformation matrix
       * and offsets for x, y, and z axes.
       */
      void applyCalibrationParameters(float &x, float &y, float &z, const CalibrationParameters &calibrationParameters);

    };
}

#endif
