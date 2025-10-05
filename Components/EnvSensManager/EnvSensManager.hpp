// ======================================================================
// \title  EnvSensManager.hpp
// \author user
// \brief  hpp file for EnvSensManager component implementation class
// ======================================================================

#ifndef Components_EnvSensManager_HPP
#define Components_EnvSensManager_HPP

#include "Components/EnvSensManager/EnvSensManagerComponentAc.hpp"

namespace Components {

  class EnvSensManager :
    public EnvSensManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct EnvSensManager object
      EnvSensManager(
          const char* const compName //!< The component name
      );

      //! Destroy EnvSensManager object
      ~EnvSensManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for getSensorData
      //!
      //! Port to get sensor data
      void getSensorData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::SensorManager::SensorData& sensorData
      ) override;

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      //! Handler implementation for PingIn
      //!
      //! Input ping port
      void PingIn_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      ) override;
      
      F32 temperatureDPS310;
      F32 pressureDPS310;
      bool temperatureDPS310Valid;
      bool pressureDPS310Valid;

      F32  temperatureSTS21;
      bool temperatureSTS21Valid;

      F32  temperatureTableNTC;
      bool temperatureTableNTCValid;

      F32  temperaturePolyNTC;
      bool temperaturePolyNTCValid;

      F32  temperatureAverage;
      static constexpr F32 numOfTemperatures = 3.0;

      static constexpr F32 currentConsumptionDivider = 10.0;
      static constexpr F32 supplyCurrentDivider      = 10.0;
      static constexpr F32 supplyVoltageDivider      = 10.0;

      Drv::SensorManager::SensorData sharedSensorData;
  };

}

#endif
