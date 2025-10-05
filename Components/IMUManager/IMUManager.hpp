// ======================================================================
// \title  IMUManager.hpp
// \author user
// \brief  hpp file for IMUManager component implementation class
// ======================================================================

#ifndef Components_IMUManager_HPP
#define Components_IMUManager_HPP

#include "Components/IMUManager/IMUManagerComponentAc.hpp"

namespace Components {

  class IMUManager :
    public IMUManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IMUManager object
      IMUManager(
          const char* const compName //!< The component name
      );

      //! Destroy IMUManager object
      ~IMUManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getBNO055Data
      void getBNO055Data_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::IMU::Data& data
      ) override;

      //! Handler implementation for getBNO055Quaternion
      void getBNO055Quaternion_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::Quaternion& quat
      ) override;

      //! Handler implementation for getICM20948Data
      void getICM20948Data_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Drv::IMU::Data& data
      ) override;

      //! Handler implementation for getICM20948Quaternion
      void getICM20948Quaternion_handler(
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
      
      void handleICM20948();
      void handleBNO055();

      Drv::IMU::Data icm20948Data;
      Drv::IMU::Data bno055Data;

      Drv::IMU::Quaternion icm20948Quaternion;
      Drv::IMU::Quaternion bno055Quaternion;
  };

}

#endif
