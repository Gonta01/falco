#include <Falco/SystemTime/SystemTime.hpp>
#include <Fw/Time/Time.hpp>
#include "FreeRTOS.h"
#include "task.h"

namespace Falco {

    SystemTime::SystemTime(const char* name) : SystemTimeComponentBase(name) {} 
    SystemTime::~SystemTime() {}
    
    void SystemTime::init(NATIVE_INT_TYPE instance) {
        SystemTimeComponentBase::init(instance);
    }
    
    void SystemTime::timeGetPort_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::Time &time /*!< The time to set */
    ) {
        U32 currentTimeMs = pdTICKS_TO_MS(xTaskGetTickCount());
        U32 currentTimeS = currentTimeMs / 1000;
        U32 currentTimeUs = (currentTimeMs -  currentTimeS * 1000) * 1000;

        time.set(currentTimeS, currentTimeUs);
    }
}
