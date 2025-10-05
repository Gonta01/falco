// ======================================================================
// \title  NTC.hpp
// \author user
// \brief  hpp file for NTC component implementation class
// ======================================================================

#ifndef Components_NTC_HPP
#define Components_NTC_HPP

#include "Components/NTC/NTCComponentAc.hpp"

#include <cstdint>
#include <etl/array.h>
#include <etl/expected.h>
#include <etl/utility.h>

namespace Components {

  class NTC :
    public NTCComponentBase
  {

    public:
      struct NTCTableValue {
        I16 temperature;
        U32 resistance;
      };

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct NTC object
      NTC(
          const char* const compName //!< The component name
      );

      //! Destroy NTC object
      ~NTC();

      /**
       * Configuration of NTC
       * @param k - calibration coefficient (calibration by two points)
       * @param b - calibration coefficient (calibration by two points)
       */
      void configure(F32 k = 1, F32 b = 0);

    PRIVATE:
      F32 calibrCoeffK = 1;
      F32 calibrCoeffB = 0;

      float coeffX3 = -0.126217731;
      float coeffX2 = 5.397950462;
      float coeffX  = -92.49957942;
      float coeffB  = 517.6198318;

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getPolynomTemperature
      Drv::Sensor::Status getPolynomTemperature_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& temperature
      ) override;

      //! Handler implementation for getTableTemperature
      Drv::Sensor::Status getTableTemperature_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& temperature
      ) override;

      enum class ErrorCode : U8 {
        NoError = 0,
        OutOfRangeError,
        VoltageMeasurementError,
      };

      using closestValues = etl::pair<I16, I16>;
      etl::expected<F32, ErrorCode> voltageMeasurementResult;
      static constexpr U32 upperArmResistance{10'000};
      
      /**
       * @brief Function to get temperature
       * @return temperature in °C
       */
      etl::expected<F32, ErrorCode> getTemperature();

      /**
       * @brief Calculate NTC resistance based on received voltage from ADC
       * @param ntcVoltage - voltage value from ADC in volts
       * @return NTC resistance in Ohms
       */
      F32 calculateNTCResistance(F32 ntcVoltage);

      F32 calculateNTCResistanceBasedOnADC(U32 adcCode);

      /**
       * @brief Function to check resistance validity
       * @param ntcResistance - NTC resistance in Ohms
       * @return true, if resistance is in range, otherwise - false
       */
      [[nodiscard]] bool isNTCResistanceInRange(U32 ntcResistance) const;

      /**
       * @brief Function to find closest values to calculated NTC resistance
       * @param ntcResistance - NTC resistance in Ohms
       * @return pair of closest resistance to current NTC resistance
       */
      [[nodiscard]] closestValues searchClosestResistancesInTable(U32 ntcResistance) const;

      /**
       * @brief Function to get interpolated value of temperature based on closest values to NTC resistance
       * @param pair - closest values to NTC resistance
       * @param ntcResistance - NTC resistance in Ohms
       * @return interpolated temperature
       */
      F32 getInterpolatedTemperature(closestValues &pair, U32 ntcResistance) const;

      /**
       * @brief Function to get interpolated value of temperature based on closest values to NTC resistance
       * @return interpolated temperature
       */
      F32 getPolynomialTemperature(F32 ntcResistance) const;
  };

}

#endif
