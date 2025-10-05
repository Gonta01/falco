// ======================================================================
// \title  NTCTester.hpp
// \author user
// \brief  hpp file for NTC component test harness implementation class
// ======================================================================

#ifndef Components_NTCTester_HPP
#define Components_NTCTester_HPP

#include "Components/NTC/NTCGTestBase.hpp"
#include "Components/NTC/NTC.hpp"

namespace Components {

  class NTCTester :
    public NTCGTestBase
  {

    public:

      // ----------------------------------------------------------------------
      // Constants
      // ----------------------------------------------------------------------

      // Maximum size of histories storing events, telemetry, and port outputs
      static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

      // Instance ID supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

    public:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object NTCTester
      NTCTester();

      //! Destroy object NTCTester
      ~NTCTester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      void testNTCGetTemperature();
      void testNTCColdTemperature();
      void testNTCPolynomTemperature();
      void testNTCPolynomColdTemperature();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------
      Drv::ADC::Status from_getADCCode_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32& adc
      );

      //! Handler implementation for getADCMax
      U32 from_getADCMax_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      );

      //! Handler implementation for getVoltageReference
      F32 from_getVoltageReference_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      );

      //! Handler implementation for getVoltageValue
      Drv::ADC::Status from_getVoltageValue_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& voltage
      );

    private:

      // ----------------------------------------------------------------------
      // Helper functions
      // ----------------------------------------------------------------------

      //! Connect ports
      void connectPorts();

      //! Initialize components
      void initComponents();

      void setMockVoltageReference(F32 value);
      void setMockVoltageValue(F32 value);
      void setMockADCCode(U32 value);
      void setMockADCMax(U32 value);

    private:

      // ----------------------------------------------------------------------
      // Member variables
      // ----------------------------------------------------------------------

      //! The component under test
      NTC component;
      F32 mockedVoltageReference;
      F32 mockedVoltageValue;
      U32 mockedADCMax;
      U32 mockedADCCode;

  };

}

#endif
