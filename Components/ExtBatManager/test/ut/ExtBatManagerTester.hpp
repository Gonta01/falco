// ======================================================================
// \title  ExtBatManagerTester.hpp
// \author user
// \brief  hpp file for ExtBatManager component test harness implementation class
// ======================================================================

#ifndef Components_ExtBatManagerTester_HPP
#define Components_ExtBatManagerTester_HPP

#include "Components/ExtBatManager/ExtBatManagerGTestBase.hpp"
#include "Components/ExtBatManager/ExtBatManager.hpp"

namespace Components {

  class ExtBatManagerTester :
    public ExtBatManagerGTestBase
  {

    public:

      // ----------------------------------------------------------------------
      // Constants
      // ----------------------------------------------------------------------

      // Maximum size of histories storing events, telemetry, and port outputs
      static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 12;

      // Instance ID supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

      // Queue depth supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

    public:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object ExtBatManagerTester
      ExtBatManagerTester();

      //! Destroy object ExtBatManagerTester
      ~ExtBatManagerTester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------
      void ExtBatManagerUT1();
      void ExtBatManagerUT2();
      void ExtBatManagerUT3();
      void ExtBatManagerUT4_1();
      void ExtBatManagerUT4_2();
      void ExtBatManagerUT5_1();
      void ExtBatManagerUT5_2();
      void ExtBatManagerUT5_3();
      void ExtBatManagerUT6_1();
      void ExtBatManagerUT6_2();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler implementation for enablePin
      void from_enablePin_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Fw::Logic& state
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

      void setMockAdcVoltage(F32 value);

      void setMockAdcStatus(Drv::ADC::Status status);

      private:

      // ----------------------------------------------------------------------
      // Member variables
      // ----------------------------------------------------------------------

      //! The component under test
      ExtBatManager component;
      F32 adcVoltage;
      Drv::ADC::Status adcStatus;

      static constexpr F32 maxInputVoltage{2.333};
      static constexpr F32 maxBatteryVoltage{16.8};
  };

}

#endif
