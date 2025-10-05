// ======================================================================
// \title  RTCManager.cpp
// \author user
// \brief  cpp file for RTCManager component implementation class
// ======================================================================

#include "Components/RTCManager/RTCManager.hpp"
#include "FpConfig.hpp"
#include <ctime>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  RTCManager ::
    RTCManager(const char* const compName) :
      RTCManagerComponentBase(compName)
  {
    lastTimeOfAutoTimeSetting.set(0,0);
  }

  RTCManager ::
    ~RTCManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void RTCManager::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    if (isTimeSetToday == false) {
      /**
       * Get time from external source and set with the usage of setCurrentTime
       */
    }

    Fw::Time now = this->getTime();
    Fw::Time target;
    target.sub(now, lastTimeOfAutoTimeSetting);
    Fw::Time timeToSetRTC(timeBetweenCurrentTimeSettingS, 0);
    if (target >= timeToSetRTC) {
      isTimeSetToday = false;
    }
  }
  
  void RTCManager ::
    PingIn_handler(
        NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    this->PingOut_out(0,key);
  }
  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void RTCManager ::
    SET_TIME_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U64 timeValue
    )
  {
    U64 timeFromRTC = getCurrentTimeInS();
    if (timeValue > timeFromRTC) {
      setCurrentTime(timeValue);
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }
    else if (timeValue == timeFromRTC) {
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }
    else {
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
    }
  }

  void RTCManager::setCurrentTime(U64 timeValue) {
    time_t time = static_cast<time_t>(timeValue);
    struct tm *tmTime = gmtime(&time);
    currentTime.setTm_sec(tmTime->tm_sec);
    currentTime.setTm_min(tmTime->tm_min);
    currentTime.setTm_hour(tmTime->tm_hour);
    currentTime.setTm_mday(tmTime->tm_mday);
    currentTime.setTm_mon(tmTime->tm_mon);
    currentTime.setTm_year(tmTime->tm_year);
    currentTime.setTm_wday(tmTime->tm_wday);
    currentTime.setTm_yday(tmTime->tm_yday);
    currentTime.setTm_isdst(tmTime->tm_isdst);
    this->setTime_out(0, currentTime);
  }

  U64 RTCManager::getCurrentTimeInS() {
    this->getTime_out(0, receivedTime);
    struct tm sysTime;
    sysTime.tm_sec         = receivedTime.getTm_sec();
    sysTime.tm_min         = receivedTime.getTm_min();
    sysTime.tm_hour        = receivedTime.getTm_hour();
    sysTime.tm_mday        = receivedTime.getTm_mday();
    sysTime.tm_mon         = receivedTime.getTm_mon();
    sysTime.tm_year        = receivedTime.getTm_year();
    sysTime.tm_wday        = receivedTime.getTm_wday();
    sysTime.tm_yday        = receivedTime.getTm_yday();
    sysTime.tm_isdst       = receivedTime.getTm_isdst();
    time_t sec = mktime(&sysTime);

    return static_cast<U64>(sec);
  }
}
