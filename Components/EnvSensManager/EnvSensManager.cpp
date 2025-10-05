// ======================================================================
// \title  EnvSensManager.cpp
// \author user
// \brief  cpp file for EnvSensManager component implementation class
// ======================================================================

#include "Components/EnvSensManager/EnvSensManager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  EnvSensManager ::
    EnvSensManager(const char* const compName) :
      EnvSensManagerComponentBase(compName)
  {

  }

  EnvSensManager ::
    ~EnvSensManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void EnvSensManager::getSensorData_handler(NATIVE_INT_TYPE portNum, Drv::SensorManager::SensorData& sensorData) {
    sensorData = sharedSensorData;
  }

  void EnvSensManager ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    auto sensorStatus = this->getTemperatureDPS310_out(0, temperatureDPS310);
    bool temp1Valid = (sensorStatus == Drv::Sensor::Status::SUCCESS);
   
    sensorStatus = this->getPressuretDPS310_out(0, pressureDPS310);
    bool pressValid = (sensorStatus == Drv::Sensor::Status::SUCCESS);

    sensorStatus = this->getTemperatureSTS21_out(0, temperatureSTS21);
    bool temp2Valid = (sensorStatus == Drv::Sensor::Status::SUCCESS);

    sensorStatus = this->getTemperatureTableNTC_out(0, temperatureTableNTC);
    bool temp3Valid = (sensorStatus == Drv::Sensor::Status::SUCCESS);

    sensorStatus = this->getTemperaturePolynomNTC_out(0, temperaturePolyNTC);
    bool temp4Valid = (sensorStatus == Drv::Sensor::Status::SUCCESS);
    
    bool tempAverageValid = temp1Valid && temp2Valid && temp3Valid;
    if (tempAverageValid) {
      temperatureAverage = (temperatureDPS310 + temperatureSTS21 + temperaturePolyNTC) / numOfTemperatures;
    }

    this->lock();
    sharedSensorData.setTemperatureDPS310Valid(temp1Valid);
    if (temp1Valid) {
      sharedSensorData.setTemperatureDPS310(temperatureDPS310);
    }

    sharedSensorData.setPressureDPS310Valid(pressValid);
    if (pressValid) {
      sharedSensorData.setPressureDPS310(pressureDPS310);
    }

    sharedSensorData.setTemperatureSTS21Valid(temp2Valid);
    if (temp2Valid) {
      sharedSensorData.setTemperatureSTS21(temperatureSTS21);
    }

    sharedSensorData.setTemperatureNTCValid(temp3Valid);
    if (temp3Valid) {
      sharedSensorData.setTemperatureTableNTC(temperatureTableNTC);
    }
    if (temp4Valid) {
      sharedSensorData.setTemperaturePolyNTC(temperaturePolyNTC);
    }

    sharedSensorData.setTemperatureAverageValid(tempAverageValid);
    if (temperatureAverage) {
      sharedSensorData.setTemperatureAverage(temperatureAverage);
    }
    this->unLock();
  }

  void EnvSensManager::
    PingIn_handler(
        NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    this->PingOut_out(0,key);
  }
}
