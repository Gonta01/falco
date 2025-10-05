// ======================================================================
// \title  ExtBatTester.cpp
// \author user
// \brief  cpp file for ExtBatTester component implementation class
// ======================================================================

#include "Components/ExtBatTester/ExtBatTester.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ExtBatTester ::
    ExtBatTester(const char* const compName) :
      ExtBatTesterComponentBase(compName)
  {

  }

  ExtBatTester ::
    ~ExtBatTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ExtBatTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    this->getVoltage_out(0, voltageData);
  }

}
