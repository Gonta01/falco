// ======================================================================
// \title  RTCDriver.hpp
// \author user
// \brief  hpp file for RTCDriver component implementation class
// ======================================================================

#ifndef Drv_RTCDriver_HPP
#define Drv_RTCDriver_HPP

#include "Falco/Drv/RTCDriver/RTCDriverComponentAc.hpp"

namespace Falco {

  class RTCDriver :
    public RTCDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct RTCDriver object
      RTCDriver(
          const char* const compName //!< The component name
      );

      //! Destroy RTCDriver object
      ~RTCDriver();

      void configure(const Drv::Calendar::Time& time);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for getTime
      //!
      //! Port to set time
      void getTime_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::Calendar::Time& time
      ) override;

      //! Handler implementation for setTime
      //!
      //! Port to set time
      void setTime_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Drv::Calendar::Time& time
      ) override;

  };

}

#endif
