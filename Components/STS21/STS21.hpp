// ======================================================================
// \title  STS21.hpp
// \author user
// \brief  hpp file for STS21 component implementation class
// ======================================================================

#ifndef Components_STS21_HPP
#define Components_STS21_HPP

#include "Components/STS21/STS21ComponentAc.hpp"
#include "etl/span.h"
#include "etl/expected.h"

namespace Components {

  class STS21 :
    public STS21ComponentBase
  {

    public:
      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct STS21 object
      STS21(
          const char* const compName //!< The component name
      );

      //! Destroy STS21 object
      ~STS21();
      
      /**
      * Configuration of STS21 temperature sensor
      * @param k - calibration coefficient (calibration by two points)
      * @param b - calibration coefficient (calibration by two points)
      */
      void configure(F32 k = 1, F32 b = 0);
      
    PRIVATE:
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getTemperature
      //!
      //! Sensor port to retrieve last measured temperature
      Drv::Sensor::Status getTemperature_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& temperature
      ) override;
      
      
      /**
      * The maximum duration for measurements depends on the resolution chosen.
      * @note  14 bit:  T typ = 66, T max = 85 ms
      * @note  13 bit:  T typ = 33, T max = 43 ms
      * @note  12 bit:  T typ = 17, T max = 22 ms
      * @note  11 bit:  T typ = 9,  T max = 11 ms
      */
      enum class MeasurementResolution : uint8_t {
          Resolution_14bit = 0,
          Resolution_13bit,
          Resolution_12bit,
          Resolution_11bit,
      };

      /**
      * NoError if operation completed successfully
      */
      enum class ErrorCode : uint8_t {
          NoError = 0,
          ICCommError,
          WrongResolutionConfiguration,
          NotInited
      };

      /**
       * STS21 commands
       */
      enum class STS21_COMMAND : uint8_t {
          TriggerMeasurement = 0xF3,
          SwReset = 0xFE,
          ReadUserRegister = 0xE7,
          WriteUserReg = 0xE6
      };

      /**
       * STS21 User Register Bits
       */
      enum STS21_USER_REGISTER : uint8_t {
          MeasurementResolutionOneBit = 0U,
          DisableOTPReloadBit = 1U,
          EnableOnChipHeater = 2U,
          EndOfBatteryBit = 6U,
          MeasurementResolutionTwoBit = 7U
      };

      /**
       * STS21 driver state
       */
      enum class DriverState : uint8_t {
            STS21_INIT,
            STS21_RUN
        };

      F32 calibrCoeffK = 1;
      F32 calibrCoeffB = 0;

      static constexpr U32 writeTimeout = 0xFFFF'FFFF;
      static constexpr U32 readTimeout = 0xFFFF'FFFF;

      static constexpr uint8_t userRegisterLen = 8U;
      static constexpr uint16_t polynomial = 0x131;
      static constexpr uint8_t STS21Addr = 0x4A;
      bool inited;
      DriverState driverState;

      // According to Sensirion_Temperature_Sensors_STS21_Datasheet page 7, table 7 for resolution 14 bit
      static constexpr U32 measurementMaxTime = 90;

      /**
       * Functions to handle driver error and create event reports
       */
      void errorReport(STS21::ErrorCode errorCode);

      /**
      * Get the temperature value.
      * Trigger measurement, wait until the measured value is available, read the value.
      * @return temperature value or ErrorCode
      */
      etl::expected<float, ErrorCode> getTemperatureBlocking();

      /**
      * Get the last measured temperature value.
      * Triggering the measurement before reading this value is required.
      * @code
      * triggerMeasurement();
      * // ... delay ~66ms - typical measurement times
      * temperature = getMeasurement();
      * @endcode
      *
      * The function does not provide the measured temperature value if
      * the triggered measurement is not completed when this function is called
      * @return temperature value or ErrorCode
      */
      etl::expected<float, ErrorCode> getMeasurement();
      
      /**
      * Trigger measurement
      * @return ErrorCode
      */
      etl::expected<void, ErrorCode> triggerMeasurement();

      /**
       * Write user register value via i2c
       * @param regVal Value to be written
       * @return true if write succeeds, false otherwise
       */
      bool writeUserReg(uint8_t regVal);

      /**
       * Write one sts21 command via i2c
       * @param cmd Command to be written
       * @return true if write succeeds, false otherwise
       */
      bool writeCmd(STS21_COMMAND cmd);

      /**
       * Read output data via i2c
       * @param data  Reference to read output data
       * @return true if write succeeds, false otherwise
       */
      bool read(uint16_t *data);

      /**
       * Check integrity of data buffer using CRC8 algorithm
       * @param dataBuff Buffer with data and CRC
       * @return true if buffer is valid, false otherwise
       */
      static bool isDataBuffValid(const etl::span<uint8_t> &dataBuff);
  };

}

#endif
