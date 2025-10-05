// ======================================================================
// \title  EnvSensTester.cpp
// \author user
// \brief  cpp file for EnvSensTester component implementation class
// ======================================================================

#include "Components/EnvSensTester/EnvSensTester.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  EnvSensTester ::
    EnvSensTester(const char* const compName) :
      EnvSensTesterComponentBase(compName)
  {

  }

  EnvSensTester ::
    ~EnvSensTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void EnvSensTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    this->getSensorData_out(0, sensorData);
     if (sensorData.getTemperatureDPS310Valid()) {
      this->tlmWrite_TemperatureDPS310(sensorData.getTemperatureDPS310());
     }
    
    if (sensorData.getPressureDPS310Valid()) {
      this->tlmWrite_PressureDPS310(sensorData.getPressureDPS310());
    }

    if (sensorData.getTemperatureSTS21Valid()) {
      this->tlmWrite_TemperatureSTS21(sensorData.getTemperatureSTS21());
    }

    if (sensorData.getTemperatureNTCValid()) {
      this->tlmWrite_TemperatureNTC(sensorData.getTemperaturePolyNTC());
    }

    if (sensorData.getTemperatureAverageValid()) {
      this->tlmWrite_TemperatureAverage(sensorData.getTemperatureAverage());
    }
  }

}
