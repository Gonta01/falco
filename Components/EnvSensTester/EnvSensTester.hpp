// ======================================================================
// \title  EnvSensTester.hpp
// \author user
// \brief  hpp file for EnvSensTester component implementation class
// ======================================================================

#ifndef Components_EnvSensTester_HPP
#define Components_EnvSensTester_HPP

#include "Components/EnvSensTester/EnvSensTesterComponentAc.hpp"

namespace Components {

  class EnvSensTester :
    public EnvSensTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct EnvSensTester object
      EnvSensTester(
          const char* const compName //!< The component name
      );

      //! Destroy EnvSensTester object
      ~EnvSensTester();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Polling for receiving data
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      Drv::SensorManager::SensorData sensorData;
  };

}

#endif
