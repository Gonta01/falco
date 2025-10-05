// ======================================================================
// \title  TimeTester.cpp
// \author user
// \brief  cpp file for TimeTester component implementation class
// ======================================================================

#include "Components/TimeTester/TimeTester.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TimeTester ::
    TimeTester(const char* const compName) :
      TimeTesterComponentBase(compName)
  {

  }

  TimeTester ::
    ~TimeTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void TimeTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if(isStartTimeSet == false) {
      startTime = this->getTime();
      isStartTimeSet = true;
    }

    Fw::Time now = this->getTime();
    Fw::Time target = Fw::Time::sub(now, startTime);
    Fw::Time timeToWait(timeToWaitS, 0);
    
    if ((target >= timeToWait) && (isTimeSetOnce == false)) {
      Drv::Calendar::Time time;
      time.setTm_sec(45);
      time.setTm_min(30);
      time.setTm_hour(12);
      time.setTm_mday(25);
      time.setTm_mon(2);
      time.setTm_year(125);

      this->setTime_out(0, time);
      isTimeSetOnce = true;
    }
  }

}
