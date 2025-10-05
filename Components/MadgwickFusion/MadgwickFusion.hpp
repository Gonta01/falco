// ======================================================================
// \title  MadgwickFusion.hpp
// \author user
// \brief  hpp file for MadgwickFusion component implementation class
// ======================================================================

#ifndef Components_MadgwickFusion_HPP
#define Components_MadgwickFusion_HPP

#include "Components/MadgwickFusion/MadgwickFusionComponentAc.hpp"
#include "Fusion.h"
#include "etl/string.h"

namespace Components {

  class MadgwickFusion :
    public MadgwickFusionComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct MadgwickFusion object
      MadgwickFusion(
          const char* const compName //!< The component name
      );

      //! Destroy MadgwickFusion object
      ~MadgwickFusion();

      void configure();

    PRIVATE:

      // Define calibration (replace with actual calibration data if available)
      const FusionMatrix gyroscopeMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
      const FusionVector gyroscopeSensitivity = {1.0f, 1.0f, 1.0f};
      const FusionVector gyroscopeOffset = {0.0f, 0.0f, 0.0f};
      const FusionMatrix accelerometerMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
      const FusionVector accelerometerSensitivity = {1.0f, 1.0f, 1.0f};
      const FusionVector accelerometerOffset = {0.0f, 0.0f, 0.0f};
      const FusionMatrix softIronMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
      const FusionVector hardIronOffset = {0.0f, 0.0f, 0.0f};

      // Initialise algorithms
      FusionOffset offset;
      FusionAhrs ahrs;

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      Drv::IMU::Data imuData;
      F32 samplePeriod = 0.01;
      U32 SAMPLE_RATE = 100;
      F32 g = 9.80665;
      
      uint32_t counter = 0;

      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

      void simpleExampleInit();
      void simpleExampleRun();
      void advancedExampleInit();
      void advancedExampleRun();

      bool sendDataFor3DVisualization(const FusionQuaternion& quaternion);
      bool sendDataFor2DVisualization(const FusionEuler& euler);
      bool sendRawDataForCalibration(const FusionVector& accel, const FusionVector& gyro, const FusionVector& magn);
      bool sendDataForDebug(const FusionQuaternion& quaternion, const FusionAhrsInternalStates& ahrsInternalStates, const FusionAhrsFlags& ahrsFlags);
    etl::string<1024> my_string;

  };

}

#endif
