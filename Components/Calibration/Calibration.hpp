// ======================================================================
// \title  Calibration.hpp
// \author user
// \brief  hpp file for Calibration component implementation class
// ======================================================================

#ifndef Components_Calibration_HPP
#define Components_Calibration_HPP

#include "Components/Calibration/CalibrationComponentAc.hpp"
#include "etl/string.h"

namespace Components {

  class Calibration :
    public CalibrationComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Calibration object
      Calibration(
          const char* const compName //!< The component name
      );

      //! Destroy Calibration object
      ~Calibration();

    PRIVATE:
    enum class CalibrationMode{
      FREE_IMU,
      MAG_CAL
    };
      static constexpr CalibrationMode mode = CalibrationMode::MAG_CAL;

      Drv::IMU::CalibrationData calibrationData{0,0,0, 0,0,0, 0,0,0};
      Drv::IMU::Data imuData;
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

      // FreeIMU related things
      bool sendRawDataForCalibrationForFreeIMU(Drv::IMU::CalibrationData& rawData);
      bool sendVersionAnswerForFreeIMU();
      bool isSamplingStarted{false};
      U32 samplingCnt = 0;
      U32 maxCount = 0;
      
      static constexpr U32 sizeOfCalibrationMessage = 512;
      etl::string<sizeOfCalibrationMessage> calibrationMessage;

      void useFreeIMUCalibration();

      // MagCal realted things
      bool sendRawDataForCalibrationForMagCal(Drv::IMU::CalibrationData& rawData);

      void useMagCalCalibration();
      
  };

}

#endif
