// ======================================================================
// \title  CANFDTester.hpp
// \author user
// \brief  hpp file for CANFDTester component implementation class
// ======================================================================

#ifndef Components_CANFDTester_HPP
#define Components_CANFDTester_HPP

#include "Components/CANFDTester/CANFDTesterComponentAc.hpp"

namespace Components {

  class CANFDTester :
    public CANFDTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct CANFDTester object
      CANFDTester(
          const char* const compName //!< The component name
      );

      //! Destroy CANFDTester object
      ~CANFDTester();

    PRIVATE:
      uint32_t CANFDMessagesCounter;
      static constexpr U32 writeTimeout = 1;
      static constexpr U32 readTimeout = 0x0;
      
      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;
  };

}

#endif
