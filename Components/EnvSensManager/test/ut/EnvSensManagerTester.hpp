// ======================================================================
// \title  EnvSensManagerTester.hpp
// \author user
// \brief  hpp file for EnvSensManager component test harness implementation class
// ======================================================================

#ifndef Components_EnvSensManagerTester_HPP
#define Components_EnvSensManagerTester_HPP

#include "Components/EnvSensManager/EnvSensManagerGTestBase.hpp"
#include "Components/EnvSensManager/EnvSensManager.hpp"

namespace Components
{

  class EnvSensManagerTester : public EnvSensManagerGTestBase
  {

  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

    // Queue depth supplied to the component instance under test
    static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object EnvSensManagerTester
    EnvSensManagerTester();

    //! Destroy object EnvSensManagerTester
    ~EnvSensManagerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    void testEnvSensManagerAveraging();
    void testEnvSensManagerInvalidData();

  private:
    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getPressuretDPS310
    void from_PingOut_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 key                  //!< Value to return to pinger
    );

    //! Handler implementation for getPressuretDPS310
    Drv::Sensor::Status from_getPressuretDPS310_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        F32 &pressure);

    //! Handler implementation for getTemperatureDPS310
    Drv::Sensor::Status from_getTemperatureDPS310_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        F32 &temperature);

    //! Handler implementation for getTemperaturePolynomNTC
    Drv::Sensor::Status from_getTemperaturePolynomNTC_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        F32 &temperature);

    //! Handler implementation for getTemperatureSTS21
    Drv::Sensor::Status from_getTemperatureSTS21_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        F32 &temperature);

    //! Handler implementation for getTemperatureTableNTC
    Drv::Sensor::Status from_getTemperatureTableNTC_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        F32 &temperature);

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

    void setMockPressuretDPS310(F32 value);
    void setMockTemperatureDPS310(F32 value);
    void setMockTemperatureNTC(F32 value);
    void setMockTemperatureSTS21(F32 value);

    void setMockPressuretDPS310Status(Drv::Sensor::Status status);
    void setMockTemperatureDPS310Status(Drv::Sensor::Status status);
    void setMockTemperatureNTCStatus(Drv::Sensor::Status status);
    void setMockTemperatureSTS21Status(Drv::Sensor::Status status);

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    EnvSensManager component;
    F32 mockedPressuretDPS310;
    F32 mockedTemperatureDPS310;
    F32 mockedTemperatureNTC;
    F32 mockedTemperatureSTS21;

    Drv::Sensor::Status mockedPressuretDPS310Status;
    Drv::Sensor::Status mockedTemperatureDPS310Status;
    Drv::Sensor::Status mockedTemperatureNTCStatus;
    Drv::Sensor::Status mockedTemperatureSTS21Status;
  };

}

#endif
