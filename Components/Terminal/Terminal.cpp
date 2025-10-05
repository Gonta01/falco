// ======================================================================
// \title  Terminal.cpp
// \author user
// \brief  cpp file for Terminal component implementation class
// ======================================================================

#include "Components/Terminal/Terminal.hpp"
#include "FpConfig.hpp"
#include "etl/to_string.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Terminal ::
    Terminal(const char* const compName) :
      TerminalComponentBase(compName)
  {

  }

  Terminal ::
    ~Terminal()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void Terminal::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Drv::SensorManager::SensorData sensorData;
    this->getSensorData_out(0, sensorData);
    constexpr U32 myStringMaxSize = 256;
    etl::string<myStringMaxSize> my_string;

    my_string.clear();
    my_string.append("STS21: ");
    if (sensorData.getTemperatureSTS21Valid()) {
        etl::to_string(sensorData.getTemperatureSTS21(), my_string, etl::format_spec().precision(3), true);
    }
    else {
        my_string.append("N/A");
    }
    my_string.append("; DPS310: ");
    if (sensorData.getTemperatureDPS310Valid()) {
        etl::to_string(sensorData.getTemperatureDPS310(), my_string, etl::format_spec().precision(3), true);
    }
    else {
        my_string.append("N/A");
    }
    my_string.append("; NTC table: ");
    if (sensorData.getTemperatureNTCValid()) {
      etl::to_string(sensorData.getTemperatureTableNTC(), my_string, etl::format_spec().precision(3), true);
      my_string.append("; NTC poly: ");
      etl::to_string(sensorData.getTemperaturePolyNTC(), my_string, etl::format_spec().precision(3), true);
    }
    else {
        my_string.append("N/A");
    }
    my_string.append("; NTC raw ADC: ");
    U32 singleADCCode = 0;
    auto status = getSingleADCCode_out(0, singleADCCode);
    if (status == Drv::ADC::Status::SUCCESS) {
      etl::to_string(singleADCCode, my_string, etl::format_spec(), true);
    }
    else {
        my_string.append("N/A");
    }
    my_string.append("\n\r");
    
    Fw::Buffer fwBuffer(reinterpret_cast<U8*>(my_string.data()), my_string.size());
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, 2);

  }
}
