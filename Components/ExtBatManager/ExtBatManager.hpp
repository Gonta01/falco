// ======================================================================
// \title  ExtBatManager.hpp
// \author user
// \brief  hpp file for ExtBatManager component implementation class
// ======================================================================

#ifndef Components_ExtBatManager_HPP
#define Components_ExtBatManager_HPP

#include "Components/ExtBatManager/ExtBatManagerComponentAc.hpp"
#include <etl/expected.h>
#include <etl/circular_buffer.h>

namespace Components {

  class ExtBatManager :
    public ExtBatManagerComponentBase
  {

    public:
      struct Thresholds {
        U32 Min;
        U32 Max;
      };

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ExtBatManager object
      ExtBatManager(
          const char* const compName //!< The component name
      );

      //! Destroy ExtBatManager object
      ~ExtBatManager();

      // Configure external battery manager
      void configure(const Thresholds &thresholds);

    PRIVATE:
      enum class ErrorCode : uint8_t {
        NoError = 0,
        VoltageMeasurementError
      };

      static constexpr U8 delayForBatteryMeasurementMs{2};
      static constexpr F32 maxInputVoltage{2.333};
      static constexpr F32 maxBatteryVoltage{16.8};
      Drv::ExtBat::VoltageData extBatVoltageData;
      
      static constexpr U32 numOfMeasurementsForAveraging{10};
      etl::circular_buffer<F32, numOfMeasurementsForAveraging> bufferOfMeasurements;

      static constexpr U32 minPossibleThresholdMiliVolts{0};
      static constexpr U32 maxPossibleThresholdMiliVolts{16'800};

      static constexpr F32 scaleMiliVoltsToVolts{1000.0};
      
      Thresholds extBatThresholds;
      bool overvoltageReportedFlag{false};
      bool undervoltageReportedFlag{false};
      
      bool voltageValid{false};
      bool undervoltageAlarm{false};
      bool overvoltageAlarm{false};
      
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for getVoltage
      void getVoltage_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::ExtBat::VoltageData& voltageData
      ) override;

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      /**
      * Function allows to receive voltage of external battery
      * @return voltage in volts
      */
      etl::expected<float, ErrorCode> getVoltage();

      /**
      * Function calculates external battery voltage
      * @param inputVoltage - voltage received by analog input in volts
      * @return external battery voltage in volts
      */
      F32 calculateBatteryVoltage(F32 inputVoltage);

      /**
      * Function to receive average voltage
      * @param averagedVoltage
      * @return true, if successfully received voltage
      */ 
      bool receiveAveragedVoltage(F32& averagedVoltage);

      /**
      * Function to receive instantaneous voltage measurement
      * @param voltageValue
      * @return true, if successfully received voltage
      */ 
      bool receiveInstantaneousVoltageMeasurement(F32& voltageValue);

      /**
       * Function to check overvoltage
       * @param voltageValue - voltage to be checked
       * @return true, if overvoltage, otherwise - false
       */
      bool checkOvervoltage(const F32& voltageValue);

      /**
       * Function to check undervoltage
       * @param voltageValue - voltage to be checked
       * @return true, if undervoltage, otherwise - false
       */
      bool checkUndervoltage(const F32& voltageValue);
  };

}

#endif
