// ======================================================================
// \title  IOExpanderTester.hpp
// \author user
// \brief  hpp file for IOExpanderTester component implementation class
// ======================================================================

#ifndef Components_IOExpanderTester_HPP
#define Components_IOExpanderTester_HPP

#include "Components/IOExpanderTester/IOExpanderTesterComponentAc.hpp"

namespace Components {

  class IOExpanderTester :
    public IOExpanderTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IOExpanderTester object
      IOExpanderTester(
          const char* const compName //!< The component name
      );

      //! Destroy IOExpanderTester object
      ~IOExpanderTester();

    PRIVATE:
    
    enum class IO{
      Expander1 = 1,
      Expander2 = 2
    };

    struct Pattern {
      Fw::Logic Ch1;
      Fw::Logic Ch2;
      Fw::Logic Ch3;
      Fw::Logic Ch4;

      Fw::Logic Ch5;
      Fw::Logic Ch6;
      Fw::Logic Ch7;
      Fw::Logic Ch8;
    };

    // Note: ensure pin configurations is responsibility of the user
    enum class Tester {
      Mode0, // IOExpander1 pins configured as digitalInputs, IOExpander2 pins configured as digitalOutputs
      Mode1, // IOExpander2 pins configured as digitalInputs, IOExpander1 pins configured as digitalOutputs
      Mode2, // IOExpander1 pins configured as analogInputs,  IOExpander2 pins configured as digitalOutputs
      Mode3, // IOExpander2 pins configured as analogInputs,  IOExpander1 pins configured as digitalOutputs
    };

    /**
     * Update Digital Input Telemetry
     * @param channel - digital input channel
     * @param state - new channel state
     */
    void updateDigitalInputTelemetry(IO expander, const Drv::IOExpander::Channel& channel, Fw::Logic& state);

    /**
     * Update Analog Input Telemetry
     * @param channel - analog input channel
     * @param value - new channel value
     */  
    void updateAnalogInputTelemetry(IO expander, const Drv::IOExpander::Channel& channel, U16 value, F32 voltage);
    
    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id);

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Port for receiving calls from the rate group
    void run_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
    ) override;

    bool digitalOutputSetPattern(Pattern &pattern);
    bool digitalInputReceivePattern(IO expander, Pattern &pattern);
    bool analogInputUpdateValues(IO expander);

    void Mode0();
    void ModeMultiple0();
    void ModeChessMultiple0();


    void Mode1();
    void ModeMultiple1();

    void Mode2();
    
    void Mode3();

    Tester testerMode = Tester::Mode3;
    bool switchPattern = {false};  
    bool thresholdsAreSet{false};
  };

}

#endif
