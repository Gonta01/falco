// ======================================================================
// \title  RS485Tester.hpp
// \author BDL
// \brief  hpp file for RS485Tester component implementation class
// ======================================================================

#ifndef Components_RS485Tester_HPP
#define Components_RS485Tester_HPP

#include "Components/RS485Tester/RS485TesterComponentAc.hpp"

namespace Components {

  class RS485Tester :
    public RS485TesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct RS485Tester object
      RS485Tester(
          const char* const compName //!< The component name
      );

      //! Destroy RS485Tester object
      ~RS485Tester();

    PRIVATE:
      bool readyToTransmit = false;
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

  };

}

#endif
