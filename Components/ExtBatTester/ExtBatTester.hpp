// ======================================================================
// \title  ExtBatTester.hpp
// \author user
// \brief  hpp file for ExtBatTester component implementation class
// ======================================================================

#ifndef Components_ExtBatTester_HPP
#define Components_ExtBatTester_HPP

#include "Components/ExtBatTester/ExtBatTesterComponentAc.hpp"

namespace Components {

  class ExtBatTester :
    public ExtBatTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ExtBatTester object
      ExtBatTester(
          const char* const compName //!< The component name
      );

      //! Destroy ExtBatTester object
      ~ExtBatTester();

    PRIVATE:
        Drv::ExtBat::VoltageData voltageData;
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
