// ======================================================================
// \title  IMUTester.hpp
// \author user
// \brief  hpp file for IMUTester component implementation class
// ======================================================================

#ifndef Components_IMUTester_HPP
#define Components_IMUTester_HPP

#include "Components/IMUTester/IMUTesterComponentAc.hpp"

namespace Components {

  class IMUTester :
    public IMUTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IMUTester object
      IMUTester(
          const char* const compName //!< The component name
      );

      //! Destroy IMUTester object
      ~IMUTester();

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

    Drv::IMU::Data icm20948Data;
    Drv::IMU::Data bno055Data;

    Drv::IMU::Quaternion icm20948Quaternion;
    Drv::IMU::Quaternion bno055Quaternion;
  };

}

#endif
