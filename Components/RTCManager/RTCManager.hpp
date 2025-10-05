// ======================================================================
// \title  RTCManager.hpp
// \author user
// \brief  hpp file for RTCManager component implementation class
// ======================================================================

#ifndef Components_RTCManager_HPP
#define Components_RTCManager_HPP

#include "Components/RTCManager/RTCManagerComponentAc.hpp"

namespace Components {

  class RTCManager :
    public RTCManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct RTCManager object
      RTCManager(
          const char* const compName //!< The component name
      );

      //! Destroy RTCManager object
      ~RTCManager();

    PRIVATE:
      Drv::Calendar::Time currentTime;
      Drv::Calendar::Time receivedTime;

      bool isTimeSetToday{false};
      U32 timeBetweenCurrentTimeSettingS = 86400; // 1 day

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      //! Handler implementation for PingIn
      //!
      //! Input ping port
      void PingIn_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      ) override;
      
    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command SET_TIME
      //!
      //! Command to set time
      void SET_TIME_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U64 timeValue
      ) override;

      void setCurrentTime(U64 timeValue);
      U64 getCurrentTimeInS();

      Fw::Time lastTimeOfAutoTimeSetting;
  };

}

#endif