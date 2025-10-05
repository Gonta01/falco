// ======================================================================
// \title  IMU2Manager.hpp
// \author user
// \brief  hpp file for IMU2Manager component implementation class
// ======================================================================

#ifndef Components_IMU2Manager_HPP
#define Components_IMU2Manager_HPP

#include "Components/IMU2Manager/IMU2ManagerComponentAc.hpp"

namespace Components {

  class IMU2Manager :
    public IMU2ManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IMU2Manager object
      IMU2Manager(
          const char* const compName //!< The component name
      );

      //! Destroy IMU2Manager object
      ~IMU2Manager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getData
      void getData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::Data& data
      ) override;

      //! Handler implementation for getQuaternion
      void getQuaternion_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::Quaternion& quat
      ) override;
      
      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      void handleData();

      Drv::IMU::Data bno055Data;
      Drv::IMU::Quaternion bno055Quaternion;

  };

}

#endif
