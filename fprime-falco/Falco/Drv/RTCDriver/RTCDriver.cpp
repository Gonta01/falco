// ======================================================================
// \title  RTCDriver.cpp
// \author user
// \brief  cpp file for RTCDriver component implementation class
// ======================================================================

#include "Falco/Drv/RTCDriver/RTCDriver.hpp"
#include "FpConfig.hpp"
#include "peripheral/rtc/plib_rtc.h"
#include "Os/Mutex.hpp"
#include <ctime>
#include "ff.h"
#include "sys_fs.h"

static Os::Mutex systemTimeMutex;

namespace Falco {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  RTCDriver ::
    RTCDriver(const char* const compName) :
      RTCDriverComponentBase(compName)
  {

  }

  RTCDriver ::
    ~RTCDriver()
  {

  }

  void RTCDriver::configure(const Drv::Calendar::Time& time) {
    struct tm sysTime;
    sysTime.tm_sec         = time.getTm_sec();
    sysTime.tm_min         = time.getTm_min();
    sysTime.tm_hour        = time.getTm_hour();
    sysTime.tm_mday        = time.getTm_mday();
    sysTime.tm_mon         = time.getTm_mon();
    sysTime.tm_year        = time.getTm_year();
    sysTime.tm_wday        = time.getTm_wday();
    sysTime.tm_yday        = time.getTm_yday();
    sysTime.tm_isdst       = time.getTm_isdst();
    RTC_TimeSet(&sysTime);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void RTCDriver ::
    getTime_handler(
        NATIVE_INT_TYPE portNum,
        Drv::Calendar::Time& time
    )
  {
    systemTimeMutex.lock();
    struct tm now;
    RTC_TimeGet(&now);
    time.setTm_sec(now.tm_sec);
    time.setTm_min(now.tm_min);
    time.setTm_hour(now.tm_hour);
    time.setTm_mday(now.tm_mday);
    time.setTm_mon(now.tm_mon);
    time.setTm_year(now.tm_year);
    time.setTm_wday(now.tm_wday);
    time.setTm_yday(now.tm_yday);
    time.setTm_isdst(now.tm_isdst);
    systemTimeMutex.unLock();
  }

  void RTCDriver ::
    setTime_handler(
        NATIVE_INT_TYPE portNum,
        const Drv::Calendar::Time& time
    )
  {
    systemTimeMutex.lock();
    struct tm sysTime;
    sysTime.tm_sec         = time.getTm_sec();
    sysTime.tm_min         = time.getTm_min();
    sysTime.tm_hour        = time.getTm_hour();
    sysTime.tm_mday        = time.getTm_mday();
    sysTime.tm_mon         = time.getTm_mon();
    sysTime.tm_year        = time.getTm_year();
    sysTime.tm_wday        = time.getTm_wday();
    sysTime.tm_yday        = time.getTm_yday();
    sysTime.tm_isdst       = time.getTm_isdst();
    RTC_TimeSet(&sysTime);
    systemTimeMutex.unLock();
  }
}

DWORD get_fattime(void) {
    systemTimeMutex.lock();
    struct tm now;
    RTC_TimeGet(&now);

    SYS_FS_TIME time;
    time.packedTime = 0;

    // All FAT FS times are calculated based on 0 = 1980
    time.discreteTime.year = (now.tm_year  + 1900 - 1980);
    time.discreteTime.month = (now.tm_mon + 1);
    time.discreteTime.day = now.tm_mday ;     
    time.discreteTime.hour = now.tm_hour;  
    time.discreteTime.minute = now.tm_min; 
    time.discreteTime.second = now.tm_sec / 2; 

    systemTimeMutex.unLock();
    return (time.packedTime);
}