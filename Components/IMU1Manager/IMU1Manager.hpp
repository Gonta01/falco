// ======================================================================
// \title  IMU1Manager.hpp
// \author user
// \brief  hpp file for IMU1Manager component implementation class
// ======================================================================

#ifndef Components_IMU1Manager_HPP
#define Components_IMU1Manager_HPP

#include "Components/IMU1Manager/IMU1ManagerComponentAc.hpp"

namespace Components {

  class IMU1Manager :
    public IMU1ManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IMU1Manager object
      IMU1Manager(
          const char* const compName //!< The component name
      );

      //! Destroy IMU1Manager object
      ~IMU1Manager();
      
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

      Drv::IMU::Data icm20948Data;
      Drv::IMU::Quaternion icm20948Quaternion;
  };

}

#endif
