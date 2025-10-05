// ======================================================================
// \title  Terminal.hpp
// \author user
// \brief  hpp file for Terminal component implementation class
// ======================================================================

#ifndef Components_Terminal_HPP
#define Components_Terminal_HPP

#include "Components/Terminal/TerminalComponentAc.hpp"

namespace Components {

  class Terminal :
    public TerminalComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Terminal object
      Terminal(
          const char* const compName //!< The component name
      );

      //! Destroy Terminal object
      ~Terminal();

    PRIVATE:

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
