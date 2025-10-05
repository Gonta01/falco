// ======================================================================
// \title  BNO055.hpp
// \author user
// \brief  hpp file for BNO055 component implementation class
// ======================================================================

#ifndef Components_BNO055_HPP
#define Components_BNO055_HPP

#include "Components/BNO055/BNO055ComponentAc.hpp"
#include <etl/span.h>
#include <etl/expected.h>

namespace Components {

  class BNO055 :
    public BNO055ComponentBase
  {

    public:
      struct EulerAngles {
        float heading;
        float roll;
        float pitch;
      };

      struct QuaternionAngles {
        float w;
        float x;
        float y;
        float z;
      };
      
      struct AccelData {
        float x;
        float y;
        float z;
      };

      struct GyroData {
        float x;
        float y;
        float z;
      };

      struct MagnData {
        float x;
        float y;
        float z;
      };

      struct IMUData {
        AccelData accelData;
        GyroData gyroData;
        MagnData magnData;
      };

      struct CalibrationStatus {
        bool sysCalibStatus;
        bool gyrCalibStatus;
        bool accCalibStatus;
        bool magCalibStatus;
      };


      enum class CommStatus {
        NO_STATUS                 = 0x00,
        WRITE_SUCCESS             = 0x01,
        READ_FAIL                 = 0x02,
        WRITE_FAIL                = 0x03,
        REGMAP_INVALID_ADDRESS    = 0x04,
        REGMAP_WRITE_DISABLED     = 0x05,
        WRONG_START_BYTE          = 0x06,
        BUS_OVER_RUN_ERROR        = 0x07,
        MAX_LENGTH_ERROR          = 0x08,
        MIN_LENGTH_ERROR          = 0x09,
        RECEIVE_CHARACTER_TIMEOUT = 0x0A
      };

      enum class ErrorCode {
        OPERATION_SUCCESS         = 0x00,
        COMMUNICATION_ERROR       = 0x01,
        RESPONSE_HEADER_MISMATCH  = 0x02,
        RESPONSE_SIZE_MISMATCH    = 0x03,
        RECEIVE_FAIL              = 0x04,
        SEND_FAIL                 = 0x05,
        BUFFER_CLEAR_FAIL         = 0x06,
        RESET_FAIL                = 0x07,
        INIT_FAIL                 = 0x08,
        SELF_TEST_FAIL            = 0x09,
        CALIBRATION_UNFINISHED    = 0x0A
      };

      enum class AccelGRange : U8 {
        Range2G  = 0b00,
        Range4G  = 0b01,
        Range8G  = 0b10,
        Range16G = 0b11
      };

      enum class AccelBandwidth : U8 {
        Bandwidth7_81Hz  = 0b000'00,
        Bandwidth15_63Hz = 0b001'00,
        Bandwidth31_25Hz = 0b010'00,
        Bandwidth62_5Hz  = 0b011'00,
        Bandwidth125Hz   = 0b100'00,
        Bandwidth250Hz   = 0b101'00,
        Bandwidth500Hz   = 0b110'00,
        Bandwidth1000Hz  = 0b111'00
      };

      enum class AccelOperationMode : U8 {
        Normal      = 0b000'00000,
        Suspend     = 0b001'00000,
        LowPower1   = 0b010'00000,
        Standby     = 0b011'00000,
        LowPower2   = 0b100'00000,
        DeepSuspend = 0b101'00000
      };

      enum class GyroDpsRange : U8 {
        Range2000dps = 0b000,
        Range1000dps = 0b001,
        Range500dps  = 0b010,
        Range250dps  = 0b011,
        Range125dps  = 0b100
      };

      enum class GyroBandwidth : U8 {
        Bandwidth523Hz = 0b000'000,
        Bandwidth230Hz = 0b001'000,
        Bandwidth116Hz = 0b010'000,
        Bandwidth47Hz  = 0b011'000,
        Bandwidth23Hz  = 0b100'000,
        Bandwidth12Hz  = 0b101'000,
        Bandwidth64Hz  = 0b110'000,
        Bandwidth32Hz  = 0b111'000
      };

      enum class GyroPowerMode : U8 {
        Normal            = 0b000,
        FastPowerUp       = 0b001,
        DeepSuspend       = 0b010,
        Suspend           = 0b011,
        AdvancedPowersave = 0b100
      };

      enum class MagDataOutputRate : U8 {
        Rate2Hz  = 0b000,
        Rate6Hz  = 0b001,
        Rate8Hz  = 0b010,
        Rate10Hz = 0b011,
        Rate15Hz = 0b100,
        Rate20Hz = 0b101,
        Rate25Hz = 0b110,
        Rate30Hz = 0b111
      };

      enum class MagOperationMode : U8 {
        LowPower        = 0b00'000,
        Regular         = 0b01'000,
        EnhancedRegular = 0b10'000,
        HighAccuracy    = 0b11'000
      };

      enum class MagPowerMode : U8 {
        Normal    = 0b00'00'000,
        Sleep     = 0b01'00'000,
        Suspend   = 0b10'00'000,
        ForceMode = 0b11'00'000
      };

      enum class PowerMode : U8 {
        NormalMode   = 0b00,
        LowPowerMode = 0b01,
        SuspendMode  = 0b10
      };

      enum class OperatingMode : U8 {
        ConfigMode   = 0b0000,
        /* Non-Fusion Mode */
        AccOnly      = 0b0001,
        MagOnly      = 0b0010,
        GyrOnly      = 0b0011,
        AccMag       = 0b0100,
        AccGyro      = 0b0101,
        MagGyro      = 0b0110,
        AMG          = 0b0111,
        /* Fusion Mode */
        IMU          = 0b1000,
        COMPASS      = 0b1001,
        M4G          = 0b1010,
        NDOF_FMC_OFF = 0b1011,
        NDOF         = 0b1100
      };

      enum class AccelUnits : U8 {
        M_s2 = 0b0,
        Mg   = 0b1
      };

      enum class AngularRateUnits : U8 {
        Dps = 0b0'0,
        Rps = 0b1'0
      };

      enum class EulerAnglesUnits : U8 {
        Degrees = 0b0'00,
        Radians = 0b1'00
      };

      enum class TemperatureUnits : U8 {
        Celsius    = 0b0'0000,
        Fahrenheit = 0b1'0000
      };

      enum class OrientationModeUnits : U8 {
        Windows = 0b0'0000000,
        Android = 0b1'0000000
      };

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct BNO055 object
      BNO055(
          const char* const compName //!< The component name
      );

      //! Destroy BNO055 object
      ~BNO055();
      
      enum class Mode {
        NO_FUSION,
        FUSION
      };

      /**
      * Configuration of IMU
      * @param mode - IMU mode
      */
      void configure(Mode mode);

    PRIVATE:

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

    //! Handler implementation for getRawData
      Drv::IMU::Status getRawData_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::RawData& rawData
    ) override;

    //! Handler implementation for getQuatData
    Drv::IMU::Status getQuatData_handler(
      NATIVE_INT_TYPE portNum, //!< The port number
      Drv::IMU::QuatData& quatData
    ) override;

    /**
     * Function to conduct IMU initialization
     * @param mode - IMU mode
     * @return ErrorCode
     */
    ErrorCode initialize(Mode mode);
    
    /**
     * Function to set IMU power mode
     * @param powerMode - powerMode
     * @return ErrorCode
     */
    ErrorCode setPowerMode(PowerMode powerMode);

    /**
     * Function to set IMU operating mode
     * @param mode - operating mode
     * @return ErrorCode
     */
    ErrorCode setOperatingMode(OperatingMode mode);

    /**
     * Function to change measurement units
     * @param accelUnits - measurement units of acceleration (m/s^2 or mg)
     * @param angRateUnits - measurement unit of angle rate (from gyroscope) (deg/s or rad/s)
     * @param eulerAngleUnits - measurement unit of Euler angle (degrees or radians)
     * @param tempUnits - sensor operation mode (Celsius or Fahrenheit)
     * @param orientnation - sensor operation mode (Quaternion units)
     * @return ErrorCode
     */
    ErrorCode setUnits(AccelUnits accelUnits = AccelUnits::M_s2, AngularRateUnits angRateUnits = AngularRateUnits::Dps, EulerAnglesUnits eulerAngleUnits = EulerAnglesUnits::Degrees, TemperatureUnits tempUnits = TemperatureUnits::Celsius, OrientationModeUnits orientnation = OrientationModeUnits::Android);

    /**
     * Function to configure accelerometer
     * Note: These configurations are auto controlled in fusion mode
     * @param range - measurement range
     * @param bandwidth - sensor bandwidth
     * @param mode - sensor operation mode
     * @return ErrorCode
     */
    ErrorCode setAccelConfig(AccelGRange range, AccelBandwidth bandwidth, AccelOperationMode mode);

    /**
     * Function to configure gyroscope
     * Note: These configurations are auto controlled in fusion mode
     * @param range - measurement range
     * @param bandwidth - sensor bandwidth
     * @param mode - sensor operation mode
     * @return ErrorCode
     */
    ErrorCode setGyroConfig(GyroDpsRange range, GyroBandwidth bandwidth, GyroPowerMode mode);
    
    /**
     * Function to configure magnetometer 
     * Note: These configurations are auto controlled in fusion mode
     * @param rate - measurement rate
     * @param operationMode - sensor operation mode
     * @param powerMode - sensor power mode
     * @return ErrorCode
     */
    ErrorCode setMagConfig(MagDataOutputRate rate, MagOperationMode operationMode, MagPowerMode powerMode);

    /**
     * Function to configure IMU in No-Fusion mode 
     * @return ErrorCode
     */
    ErrorCode noFusionConfiguration();

    /**
     * Function to get orientation in Euler angles
     * @return euler angles or ErrorCode
     */
    etl::expected<EulerAngles, ErrorCode> getEulerAngles();

    /**
     * Function to get orientation in quaternions
     * @return quaternions or ErrorCode
     */
    etl::expected<QuaternionAngles, ErrorCode> getQuaternionAngles();
    
    /**
     * Function to get acceleration
     * @return  3-axis acceleration in m/s^2, otherwise - error code
     */
    etl::expected<AccelData, ErrorCode> getAccelData();
    
    /**
     * Function to get angular rate (gyroscope data)
     * @return 3-axis angular velocity in degrees per second, otherwise - error code
     */
    etl::expected<GyroData, ErrorCode> getGyroData();
    
    /**
     * Function to get magnetic field strength
     * @return  3-axis magnetic field in microTesla, otherwise - error code
     */
    etl::expected<MagnData, ErrorCode> getMagneticData();
    
    /**
     * Function to get IMU data (acceleration, angular rate, magnetic field strength)
     * @return IMU data or ErrorCode
     */
    etl::expected<IMUData, ErrorCode> getIMUData();
    
    /**
     * Function to check that calibration is finished
     * @return ErrorCode
     */
    ErrorCode isCalibrationFinished();
    
    /**
     * Function to perform calibration
     * @return ErrorCode
     */
    ErrorCode setCalibrationProfile(Fw::Buffer& buff);

     /**
     * Function to read current calibration configuration
     * @return ErrorCode
     */
    ErrorCode readCalibrationProfile();

    /**
     * Function to get calibration status
     * @return CalibrationStatus or ErrorCode
     */
    etl::expected<CalibrationStatus, ErrorCode> getCalibrationStatus();

    /**
     * Log reporting to GDS
     * Note: errors detected by driver are reported
     * @param errorCode - error code to report
     */
    void errorReport(ErrorCode errorCode);

    /**
     * Log reporting to GDS
     * Note: errors received from BNO055 are reported
     * @param commStatus - status to reports
     */
    void commStatusReport(CommStatus commStatus);
    /**
     * Get last communication status
     * Should be called when ErrorCode::COMMUNICATION_ERROR is detected
     * @return CommStatus
     */
    CommStatus getCommStatus();

    /**
     * @start Functions responsible for communication with BNO055
     */
    ErrorCode writeReg(U8 regAddr, U8 value);
    ErrorCode readReg(U8 regAddr, U8& value);

    ErrorCode writeRegs(U8 regAddr, Fw::Buffer& buff);
    ErrorCode writeRegsHandle(U8 regAddr, Fw::Buffer& buff);

    ErrorCode readRegs(U8 regAddr, size_t numOfRegs, Fw::Buffer& buff);
    ErrorCode readRegsHandle(U8 regAddr, size_t numOfRegs);

    ErrorCode sendWriteData(U8 regAddr, etl::span<U8> buffer);
    ErrorCode sendReadData(U8 regAddr, size_t numOfRegs);

    etl::expected<size_t, ErrorCode> receiveData(etl::span<U8> buffer);
    ErrorCode receiveBufferCleanup();

    ErrorCode handleWriteResponse(etl::span<U8> buffer);
    ErrorCode handleReadResponse(etl::span<U8> buffer);
    /**
     * @end Functions responsible for communication with BNO055
     */
    
    /**
     * Function to enter configuration mode
     * @return ErrorCode
     */
    ErrorCode setConfigMode();

    /**
     * Function to enter NDOF fusion mode
     * @return ErrorCode
     */
    ErrorCode setNDOFMode();

    /**
     * Function to enter AMG non-fusion mode
     * @return ErrorCode
     */
    ErrorCode setAMGMode();

    ErrorCode calibrateGyroOffsets();

    float GyroXOffset{0.0F};
    float GyroYOffset{0.0F};
    float GyroZOffset{0.0F};

    U32 busOverrunErrorCnt = 0;
    static constexpr U32 busOverrunErrorReportCnt = 100; 
    static constexpr U32 writeTimeout = 0xFFFF'FFFF;
    static constexpr U32 readTimeout = 0xFFFF'FFFF;

    static constexpr U32 minWaitTime = 2;
    
    CommStatus commStatus = CommStatus::NO_STATUS;

    bool initializationFinished = false;
    bool calibrationFinished = false;

    static constexpr U8 configMode = 0x0;
    static constexpr U8 amgMode = 0b0111;
    static constexpr U8 fusionModeNDOF = 0xc;
    static constexpr U8 modeSwitchTime = 22;
    static constexpr U32 powerOnResetTime = 650;
    static constexpr U8 maxNumOfTries = 20;
    static constexpr U8 eulerHrpDataSize = 6;
    static constexpr U8 rawDataSize = 6;
    static constexpr U8 fullRawDataSize = 18;
    static constexpr U8 quaternionsDataSize = 8;
    static constexpr U8 numOfReinitAttempts = 5;
    
    static constexpr float eulerDivDeg = 16.0;
    static constexpr float eulerDivRad = 900.0;

    static constexpr float quatDiv = 16384.0;

    static constexpr float accM_s2Div = 100.0;
    static constexpr float accMgDiv   = 1.0;

    static constexpr float magDiv     = 16.0;

    static constexpr float gyroDpsDiv = 16.0;
    static constexpr float gyroRpsDiv = 900.0;

    float accDiv   = accM_s2Div;
    float gyroDiv  = gyroDpsDiv ;
    float eulerDiv = eulerDivDeg;

    static constexpr U8 readSuccessResponseHeaderSize = 2;  
    static constexpr U8 readSuccessResponseHeader  = 0xBB;
    static constexpr U8 readFailureResponseHeader = 0xEE;
    static constexpr U8 ackResponseHeader = 0xEE;

    static constexpr U8 sendStartByte       = 0xAA;
    static constexpr U8 sendWriteMode       = 0x0;
    static constexpr U8 sendReadMode        = 0x1;
    static constexpr U8 sendHeaderSize      = 4;
    static constexpr U8 failureResponseSize = 2;
    static constexpr U8 ackResponseSize     = 2;

    static constexpr size_t maxPacketSize = 128;
    etl::array<U8, maxPacketSize> writePacket;
    etl::array<U8, maxPacketSize> readPacket;

    static constexpr U8 pageIDAddr                 = 0x07;

    static constexpr U8 accConfigAddrPage1         = 0x08;
    static constexpr U8 magConfigAddrPage1         = 0x09;
    static constexpr U8 gyrConfig0AddrPage1        = 0x0A;
    static constexpr U8 gyrConfig1AddrPage1        = 0x0B;

    static constexpr U8 chipIDAddrPage0            = 0x0;
    static constexpr U8 accDataXLsbAddrPage0       = 0x08; // start register of acceleration data
    static constexpr U8 magDataXLsbAddrPage0       = 0x0E; // start register of magnetic data
    static constexpr U8 gyrDataXLsbAddrPage0       = 0x14; // start register of gyro data
    static constexpr U8 eulDataXLsbAddrPage0       = 0x1A; // start register of Euler angles data
    static constexpr U8 quaDataWLsbAddrPage0       = 0x20; // start register of quaternions data
    static constexpr U8 calibStatAddrPage0         = 0x35;
    static constexpr U8 stResultAddrPage0          = 0x36;
    static constexpr U8 pwrModeAddrPage0           = 0x3E;
    static constexpr U8 unitSelPage0               = 0x3B;
    static constexpr U8 operationModeAddrPage0     = 0x3D;
    static constexpr U8 accOffsetXLsbAddrPage0     = 0x55; // start register of calibration data

    static constexpr U8 numOfCalibrationRegs = 22;
    etl::array<U8, numOfCalibrationRegs> defaultCalibArr{0x16, 0x0, 0x1e, 0x0, 0xd6, 0xff, 0x9d, 0xfe, 0x6, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x2, 0x0, 0xe8, 0x3, 0xba, 0x2};
    etl::array<U8, numOfCalibrationRegs> newCalibArr;

    static constexpr U8 expectedChipID          = 0xA0;
    static constexpr U8 expectedAccID           = 0xFB;
    static constexpr U8 expectedMagID           = 0x32;
    static constexpr U8 expectedGyrID           = 0x0F;
    static constexpr U8 expectedSwRevIdLsb      = 0x11;
    static constexpr U8 expectedSwRevIdMsb      = 0x03;
    static constexpr U8 expectedBlRevId         = 0x15;
    static constexpr U8 calibStatExpectedResult = 0xFF;
    static constexpr U8 expectedStResult        = 0xF;
  };

}

#endif
