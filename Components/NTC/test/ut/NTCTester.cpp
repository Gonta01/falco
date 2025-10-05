// ======================================================================
// \title  NTCTester.cpp
// \author user
// \brief  cpp file for NTC component test harness implementation class
// ======================================================================

#include "NTCTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  NTCTester ::
      NTCTester() : NTCGTestBase("NTCTester", NTCTester::MAX_HISTORY_SIZE),
                    component("NTC")
  {
    this->initComponents();
    this->connectPorts();
  }

  NTCTester ::
      ~NTCTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------
  void NTCTester ::testNTCGetTemperature()
  {
    this->setMockVoltageValue(1.2);
    this->setMockVoltageReference(2.5);
    F32 temperature = 0;
    this->invoke_to_getTableTemperature(0, temperature);
    constexpr F32 expectedTemperature = 27.0;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(temperature, expectedTemperature, absError);
  }

  void NTCTester::testNTCColdTemperature()
  {
    // I
    this->setMockVoltageValue(1.9);
    this->setMockVoltageReference(2.5);
    F32 temperature = 0;
    this->invoke_to_getTableTemperature(0, temperature);
    constexpr F32 expectedTemperature = -1.7;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(temperature, expectedTemperature, absError);
  }

  void NTCTester::testNTCPolynomTemperature()
  {
    this->component.configure();
    this->setMockADCMax(255);
    this->setMockADCCode(121);
    F32 actualTemperature = 0;
    this->invoke_to_getPolynomTemperature(0, actualTemperature);
    constexpr F32 expectedTemperature = 27.5;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }

  void NTCTester::testNTCPolynomColdTemperature()
  {
    this->component.configure();
    this->setMockADCMax(255);
    this->setMockADCCode(195);
    F32 actualTemperature = 0;
    this->invoke_to_getPolynomTemperature(0, actualTemperature);
    constexpr F32 expectedTemperature = -2.2;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(actualTemperature, expectedTemperature, absError);
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------
  Drv::ADC::Status NTCTester ::
      from_getADCCode_handler(
          NATIVE_INT_TYPE portNum,
          U32 &adc)
  {
    adc = mockedADCCode;
    return Drv::ADC::Status::SUCCESS;
  }

  U32 NTCTester ::
      from_getADCMax_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedADCMax;
  }

  F32 NTCTester ::
      from_getVoltageReference_handler(NATIVE_INT_TYPE portNum)
  {
    return mockedVoltageReference;
  }

  Drv::ADC::Status NTCTester ::
      from_getVoltageValue_handler(
          NATIVE_INT_TYPE portNum,
          F32 &voltage)
  {
    voltage = mockedVoltageValue;
    return Drv::ADC::Status::SUCCESS;
  }

  void NTCTester::setMockVoltageReference(F32 value)
  {
    mockedVoltageReference = value;
  }

  void NTCTester::setMockVoltageValue(F32 value)
  {
    mockedVoltageValue = value;
  }

  void NTCTester::setMockADCMax(U32 value)
  {
    mockedADCMax = value;
  }

  void NTCTester::setMockADCCode(U32 value)
  {
    mockedADCCode = value;
  }

}
