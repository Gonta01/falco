// ======================================================================
// \title  EnvSensManagerTester.cpp
// \author user
// \brief  cpp file for EnvSensManager component test harness implementation class
// ======================================================================

#include "EnvSensManagerTester.hpp"
#include "STest/Pick/Pick.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  EnvSensManagerTester ::
      EnvSensManagerTester() : EnvSensManagerGTestBase("EnvSensManagerTester", EnvSensManagerTester::MAX_HISTORY_SIZE),
                               component("EnvSensManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  EnvSensManagerTester ::
      ~EnvSensManagerTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void EnvSensManagerTester ::
      testEnvSensManagerAveraging()
  {
    setMockTemperatureDPS310(21);
    setMockTemperatureNTC(22);
    setMockTemperatureSTS21(25);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::SensorManager::SensorData sensorData;
    this->invoke_to_getSensorData(0, sensorData);

    constexpr F32 expectedResult = 22.7;
    constexpr F32 expectedTempDPS310 = 21.0;
    constexpr F32 expectedTempNTC = 22.0;
    constexpr F32 expectedTempSTS21 = 25.0;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(sensorData.getTemperatureAverage(), expectedResult, absError);
    ASSERT_NEAR(sensorData.getTemperatureDPS310(), expectedTempDPS310, absError);
    ASSERT_NEAR(sensorData.getTemperaturePolyNTC(), expectedTempNTC, absError);
    ASSERT_NEAR(sensorData.getTemperatureTableNTC(), expectedTempNTC, absError);
    ASSERT_NEAR(sensorData.getTemperatureSTS21(), expectedTempSTS21, absError);
  }

  void EnvSensManagerTester ::
      testEnvSensManagerInvalidData()
  {
    setMockTemperatureDPS310(219);
    setMockTemperatureNTC(22);
    setMockTemperatureSTS21(25);
    setMockTemperatureDPS310Status(Drv::Sensor::Status::FAILED);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::SensorManager::SensorData sensorData;
    this->invoke_to_getSensorData(0, sensorData);

    constexpr F32 expectedResult = 22.7;
    constexpr F32 expectedTempDPS310 = 219.0;
    constexpr F32 expectedTempNTC = 22.0;
    constexpr F32 expectedTempSTS21 = 25.0;
    constexpr F32 absError = 0.1;
    ASSERT_NE(sensorData.getTemperatureAverage(), expectedResult);
    ASSERT_NE(sensorData.getTemperatureDPS310(), expectedTempDPS310);
    ASSERT_NEAR(sensorData.getTemperaturePolyNTC(), expectedTempNTC, absError);
    ASSERT_NEAR(sensorData.getTemperatureTableNTC(), expectedTempNTC, absError);
    ASSERT_NEAR(sensorData.getTemperatureSTS21(), expectedTempSTS21, absError);

    constexpr bool expectedResultValid = false;
    constexpr bool expectedTempDPS310Valid = false;
    constexpr bool expectedTempNTCValid = true;
    constexpr bool expectedTempSTS21Valid = true;

    ASSERT_EQ(sensorData.getTemperatureAverageValid(), expectedResultValid);
    ASSERT_EQ(sensorData.getTemperatureDPS310Valid(), expectedTempDPS310Valid);
    ASSERT_EQ(sensorData.getTemperatureNTCValid(), expectedTempNTCValid);
    ASSERT_EQ(sensorData.getTemperatureNTCValid(), expectedTempNTCValid);
    ASSERT_EQ(sensorData.getTemperatureSTS21Valid(), expectedTempSTS21Valid);
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::Sensor::Status EnvSensManagerTester ::
      from_getPressuretDPS310_handler(
          NATIVE_INT_TYPE portNum,
          F32 &pressure)
  {
    pressure = mockedPressuretDPS310;
    return mockedPressuretDPS310Status;
  }

  Drv::Sensor::Status EnvSensManagerTester ::
      from_getTemperatureDPS310_handler(
          NATIVE_INT_TYPE portNum,
          F32 &temperature)
  {
    temperature = mockedTemperatureDPS310;
    return mockedTemperatureDPS310Status;
  }

  Drv::Sensor::Status EnvSensManagerTester ::
      from_getTemperatureSTS21_handler(
          NATIVE_INT_TYPE portNum,
          F32 &temperature)
  {
    temperature = mockedTemperatureSTS21;
    return mockedTemperatureSTS21Status;
  }

  Drv::Sensor::Status EnvSensManagerTester ::
      from_getTemperatureTableNTC_handler(
          NATIVE_INT_TYPE portNum,
          F32 &temperature)
  {
    temperature = mockedTemperatureNTC;
    return mockedTemperatureNTCStatus;
  }

  Drv::Sensor::Status EnvSensManagerTester ::
      from_getTemperaturePolynomNTC_handler(
          NATIVE_INT_TYPE portNum,
          F32 &temperature)
  {
    temperature = mockedTemperatureNTC;
    return mockedTemperatureNTCStatus;
  }

  void EnvSensManagerTester ::
      from_PingOut_handler(
          NATIVE_INT_TYPE portNum,
          U32 key)
  {
    // TODO
  }

  void EnvSensManagerTester::setMockPressuretDPS310(F32 value)
  {
    mockedPressuretDPS310 = value;
    mockedPressuretDPS310Status = Drv::Sensor::Status::SUCCESS;
  }

  void EnvSensManagerTester::setMockTemperatureDPS310(F32 value)
  {
    mockedTemperatureDPS310 = value;
    mockedTemperatureDPS310Status = Drv::Sensor::Status::SUCCESS;
  }

  void EnvSensManagerTester::setMockTemperatureNTC(F32 value)
  {
    mockedTemperatureNTC = value;
    mockedTemperatureNTCStatus = Drv::Sensor::Status::SUCCESS;
  }

  void EnvSensManagerTester::setMockTemperatureSTS21(F32 value)
  {
    mockedTemperatureSTS21 = value;
    mockedTemperatureSTS21Status = Drv::Sensor::Status::SUCCESS;
  }

  void EnvSensManagerTester::setMockPressuretDPS310Status(Drv::Sensor::Status status)
  {
    mockedPressuretDPS310Status = status;
  }
  void EnvSensManagerTester::setMockTemperatureDPS310Status(Drv::Sensor::Status status)
  {
    mockedTemperatureDPS310Status = status;
  }
  void EnvSensManagerTester::setMockTemperatureNTCStatus(Drv::Sensor::Status status)
  {
    mockedTemperatureNTCStatus = status;
  }
  void EnvSensManagerTester::setMockTemperatureSTS21Status(Drv::Sensor::Status status)
  {
    mockedTemperatureSTS21Status = status;
  }

}
