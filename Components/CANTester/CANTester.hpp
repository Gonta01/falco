// ======================================================================
// \title  CANTester.hpp
// \author bdl
// \brief  hpp file for CANTester component implementation class
// ======================================================================

#ifndef Components_CANTester_HPP
#define Components_CANTester_HPP

#include "Components/CANTester/CANTesterComponentAc.hpp"

namespace Components {

  class CANTester :
    public CANTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct CANTester object
      CANTester(
          const char* const compName //!< The component name
      );

      //! Destroy CANTester object
      ~CANTester();

    PRIVATE:
      uint32_t CANMessagesCounter;
      
      static constexpr U32 writeTimeout = 1;
      static constexpr U32 readTimeout = 0x0;

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      
  };

}

#endif
