#include <Falco/Drv/HardwareRateDriver/HardwareRateDriver.hpp>
#include <FpConfig.hpp>
#include "peripheral/tc/plib_tc_common.h"
#include "peripheral/tc/plib_tc0.h"

namespace Falco {

HardwareRateDriver* HardwareRateDriver::s_driver = NULL;

HardwareRateDriver::HardwareRateDriver(const char* compName)
    : HardwareRateDriverComponentBase(compName) {
    s_driver = this;
}

HardwareRateDriver::~HardwareRateDriver(void) {}

void HardwareRateDriver::startTimer() {
    TC0_CH0_TimerCallbackRegister(timer0Callback, (uintptr_t) this);
    TC0_CH0_TimerStart();
}

void HardwareRateDriver::stop() {
    TC0_CH0_TimerStop();
}

void HardwareRateDriver::timer0Callback(TC_TIMER_STATUS status, uintptr_t context) {
    reinterpret_cast<HardwareRateDriver *>(context)->s_timerISR(status); 
}

void HardwareRateDriver::s_timerISR(TC_TIMER_STATUS status) {
    if (status == TC_TIMER_PERIOD_MATCH) {
        s_timer(s_driver);
    }
}

void HardwareRateDriver::s_timer(void* comp) {
    Svc::TimerVal now;
    now.take();
    HardwareRateDriver* driver = reinterpret_cast<HardwareRateDriver*>(comp);

    if(driver->isConnected_CycleBackward_OutputPort(0))
    {
        driver->CycleBackward_out(0, now);
    }
    driver->m_last = now;
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------
void HardwareRateDriver::CycleIn_handler(NATIVE_INT_TYPE portNum, Svc::TimerVal& cycleStart) {
  if(this->isConnected_CycleOut_OutputPort(0))
  {
    this->CycleOut_out(0, cycleStart);
  }
}

}  // namespace Falco
