// ======================================================================
// \title  DPS310.hpp
// \author user
// \brief  hpp file for DPS310 component implementation class
// ======================================================================

#ifndef Components_DPS310_HPP
#define Components_DPS310_HPP

#include "Components/DPS310/DPS310ComponentAc.hpp"

#include <cstdint>
#include <etl/expected.h>
#include "etl/bitset.h"
#include "Os/Task.hpp"
namespace Components {

  class DPS310 :
    public DPS310ComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct DPS310 object
      DPS310(
          const char* const compName //!< The component name
      );

      //! Destroy DPS310 object
      ~DPS310();

      /**
      * Configuration of digital barometric pressure Sensor
      * @param kt - calibration coefficient for temperature measurements (calibration by two points)
      * @param bt - calibration coefficient for temperature measurements (calibration by two points)
      * @param kp - calibration coefficient for pressure measurements    (calibration by two points)
      * @param bp - calibration coefficient for pressure measurements    (calibration by two points)
      * 
      */
      void configure(F32 kt = 1, F32 bt = 0, F32 kp = 1, F32 bp = 0);

    PRIVATE:
      F32 calibrCoeffTempK = 1;
      F32 calibrCoeffTempB = 0;

      F32 calibrCoeffPressK = 1;
      F32 calibrCoeffPressB = 0;

      //! Handler implementation for getPressure
      //!
      //! Sensor port to retrieve last measured pressure
      Drv::Sensor::Status getPressure_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& pressure
      ) override;

      //! Handler implementation for getTemperature
      //!
      //! Sensor port to retrieve last measured temperature
      Drv::Sensor::Status getTemperature_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& temperature
      ) override;

      enum class ErrorCode : uint8_t {
            NoError = 0,
            NotInited,
            MeasurementTimeout,
            FailedWriteToDPS310Register,
            FailedReadDPS310Register
        };

        /** The measurement rate ranges */
        enum class MeasurementRate : uint8_t {
            DPS310_1HZ,
            DPS310_2HZ,
            DPS310_4HZ,
            DPS310_8HZ,
            DPS310_16HZ,
            DPS310_32HZ,
            DPS310_64HZ,
            DPS310_128HZ,
        };

        /** The  oversample rate ranges */
        enum class OversampleRate : uint8_t {
            DPS310_1SAMPLE,
            DPS310_2SAMPLES,
            DPS310_4SAMPLES,
            DPS310_8SAMPLES,
            DPS310_16SAMPLES,
            DPS310_32SAMPLES,
            DPS310_64SAMPLES,
            DPS310_128SAMPLES,
        };

        enum class DriverState : uint8_t {
            DPS310_INIT,
            DPS310_RUN
        };

        struct rawMeasurements {
            int32_t rawPressure;
            int32_t rawTemperature;
        };

        int32_t scaleFactorkT;
        int32_t scaleFactorkP;

        static constexpr U32 writeTimeout = 0xFFFF'FFFF;
        static constexpr U32 readTimeout = 0xFFFF'FFFF;

        static constexpr uint8_t DPS310_REG_PRSB0{0x02};
        static constexpr uint8_t DPS310_REG_TMPB0{0x05};
        static constexpr uint8_t DPS310_NUM_OF_RAW_BYTES{0x3};

        static constexpr uint8_t DPS310_REG_PRSCFG{0x06};
        static constexpr uint8_t DPS310_REG_TMPCFG{0x07};
        static constexpr uint8_t DPS310_REG_MEASCFG{0x08};
        static constexpr uint8_t DPS310_REG_CFGREG{0x09};
        static constexpr uint8_t DPS310_REG_RESET{0x0C};
        static constexpr uint8_t DPS310_REG_PRODREVID{0x0D};
        static constexpr uint8_t DPS310_REG_TMPCOEFSRCE{0x28};

        static constexpr uint8_t firstCalibrCoeffRegister{0x10};
        static constexpr uint8_t numOfCalibrCoeffRegisters{18};

        static constexpr uint8_t u4bitsOffset{4U};
        static constexpr uint8_t u8bitsOffset{8U};
        static constexpr uint8_t u12bitsOffset{12U};
        static constexpr uint8_t u16bitsOffset{16U};
        static constexpr uint8_t u4mask{0x0F};
        static constexpr uint8_t u8mask{0xFF};

        static constexpr uint8_t register8BitSize{8U};
        static constexpr uint8_t coeff12BitSize{12U};
        static constexpr uint8_t coeff16BitSize{16U};
        static constexpr uint8_t coeff20BitSize{20U};
        static constexpr uint8_t rawData24BitSize{24U};

        static constexpr uint8_t DPS310Addr = 0x76;
        uint8_t revisionID{0};
        uint8_t productID{0};

        bool needShiftForPressure{false};
        bool needShiftForTemperature{false};
        bool blockMeasurements{false};
        bool prodAndRevIdReady{false};

        etl::bitset<coeff12BitSize, uint16_t> bits_c0, bits_c1;
        etl::bitset<coeff16BitSize, uint16_t> bits_c01, bits_c11, bits_c20, bits_c21, bits_c30;
        int16_t c0, c1, c01, c11, c20, c21, c30;
        etl::bitset<coeff20BitSize, uint32_t> bits_c00, bits_c10;
        int32_t c00, c10;

        static constexpr uint32_t measureTimeFor1Sample = 4;
        static constexpr uint32_t measureTimeFor2Samples = 6;
        static constexpr uint32_t measureTimeFor4Samples = 9;
        static constexpr uint32_t measureTimeFor8Samples = 15;
        static constexpr uint32_t measureTimeFor16Samples = 28;
        static constexpr uint32_t measureTimeFor32Samples = 54;
        static constexpr uint32_t measureTimeFor64Samples = 105;
        static constexpr uint32_t measureTimeFor128Samples = 210;

        static constexpr int32_t scaleFactorFor1Sample    = 524288;
        static constexpr int32_t scaleFactorFor2Samples   = 1572864;
        static constexpr int32_t scaleFactorFor4Samples   = 3670016;
        static constexpr int32_t scaleFactorFor8Samples   = 7864320;
        static constexpr int32_t scaleFactorFor16Samples  = 253952;
        static constexpr int32_t scaleFactorFor32Samples  = 516096;
        static constexpr int32_t scaleFactorFor64Samples  = 1040384;
        static constexpr int32_t scaleFactorFor128Samples = 2088960;
        
        uint32_t timeDelayTempRdyMs = 0;
        uint32_t timeDelayPrsRdyMs = 0;

        DriverState driverState;

        /**
         * Functions to handle driver error and create event reports
         */
        void errorReport(DPS310::ErrorCode errorCode);

        /**
         * Start digital barometric pressure sensor measurements
         * @return error code
         */
        ErrorCode startMeasurements();

        /**
         * Stop digital barometric pressure sensor measurements
         * @return error code
         */
        ErrorCode stopMeasurements();

        /**
         * Get temperature measured by sensor
         * @return temperature in °C or error code if failed to receive measurements
         */
        etl::expected<float, ErrorCode> getTemperature();

        /**
         * Get pressure measured by sensor
         * @return pressure in Pa or error code if failed to receive measurements
         */
        etl::expected<float, ErrorCode> getPressure();

        /**
         * Get product ID of DSP310
         * @return product ID; error code in unexpected case scenario
         */
        [[nodiscard]] etl::expected<uint8_t, ErrorCode>  getProductID() const;

        /**
         * Get revision ID of DSP310
         * @return revision ID; error code in unexpected case scenario
         */
        [[nodiscard]] etl::expected<uint8_t, ErrorCode>  getRevisionID() const;

        /**
         * Receive values of Product and Revision ID from corresponding register
         * @return error code
         */
        ErrorCode readProdAndRevID();

        /**
         * Initiate soft reset of the sensor
         * @return error code
         */
        ErrorCode reset();

        /**
         * Read from DPS310 register
         * @param reg - register to read from
         * @param value - value of register
         * @return true, if read successfully; otherwise - false
         */
        bool readReg(uint8_t reg, uint8_t &value);

        /**
         * Write to DPS310 register
         * @param reg - register to write to
         * @param value - value, that is written to register
         * @return true, if write successfully; otherwise - false
         */
        bool writeReg(uint8_t reg, uint8_t value);

        /**
         * Generate the decimal number out of the 2's complement value
         * This function is needed to transform calibration coefficients and raw values received from registers
         * @param val - 2's complement number
         * @param bits - number of bits that represents 2's complement number
         * @return decimal number
         */
        int32_t transformTwosComplement(int32_t val, uint8_t bits);

        /**
         * The COEF_RDY bit in the Measurement Configuration register will be set when the
         * coefficients can be read out.
         * According to datasheet time to coefficients are available is 40 ms.
         * @return true when COEF_RDY == 1, otherwise false; error code when unexpected case scenario
         */
        etl::expected<bool, ErrorCode> waitUntilCoefficientsAvailable();

        /**
         * The SENSOR_RDY bit in the Measurement Configuration register will be set when the
         * coefficients can be read out.
         * According to datasheet time to sensor ready is 12 ms.
         * @return true when SENSOR_RDY == 1, otherwise false; error code when unexpected case scenario
         */
        etl::expected<bool, ErrorCode> waitUntilSensorReady();

        /**
         * Receive values from Calibration Coefficients register that  contains the 2´s complement coefficients
         * that are used to calculate the compensated pressure and temperature values.
         * @return error code
         */
        ErrorCode readCalibrationCoefficients();

        /**
         * Configuration of pressure measurement rate (PM_RATE) and resolution (PM_PRC)
         * @param measurementRate
         * @param oversampleRate
         * @return error code
         */
        ErrorCode configurePressureMeasurement(MeasurementRate measurementRate, OversampleRate oversampleRate);

        /**
         * Configuration of temperature measurement rate (TMP_RATE) and resolution (TMP_PRC).
         * @param measurementRate
         * @param oversampleRate
         * @return error code
         */
        ErrorCode configureTemperatureMeasurement(MeasurementRate measurementRate, OversampleRate oversampleRate);

        /**
         * Configure constants that depend on oversample rate
         * @param delayTimeToConfigure - variable that will containt delay time
         * @param oversampleRate - value based on which delay time is chosen
         */
        void configureMeasurementConstants(uint32_t &delayTimeToConfigure, int32_t &scaleFactor, OversampleRate oversampleRate);

        /**
         * Configuration of measurement data shift
         * @return error code
         */
        ErrorCode generalConfiguration();

        /**
         * The function reads values of Temperature Data registers (3 bytes) and transform from 2's complement
         * representation to decimal
         * @return decimal raw temperature value; error code if failed to receive value
         */
        etl::expected<int32_t, ErrorCode> getRawTemperature();

        /**
         * The function reads values of Pressure Data registers (3 bytes) and transform from 2's complement
         * representation to decimal
         * @return decimal raw pressure value; error code if failed to receive value
         */
        etl::expected<int32_t, ErrorCode> getRawPressure();

        /**
         * Calculate compensated temperature value
         * @param rawTemperature - decimal raw temperature value
         * @return compensated temperature value
         */
        [[nodiscard]] float calculateTemperature(int32_t rawTemperature) const;

        /**
         * Calculate compensated pressure value
         * @param rawPressure  - decimal raw pressure value
         * @param rawTemperature  - decimal raw temperature value
         * @return compensated pressure value
         */
        [[nodiscard]] float calculatePressure(rawMeasurements measurements) const;

        /**
         * During start of measurements it is need to wait one time before measurements are ready
         * @return error code
         */
        ErrorCode waitUntilMeasurementReady();

        /**
         * Start continuous measurements
         * @return error code
         */
        ErrorCode startContinuousMeasurements();
  };

}

#endif
