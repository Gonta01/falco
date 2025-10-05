#include <Os/IntervalTimer.hpp>
#include <Fw/Types/Assert.hpp>
#include <FreeRTOS.h>
#include <task.h>

//** TODO: test IntervalTimer implementation */
namespace Os {
    U32 IntervalTimer::getDiffUsec(const RawTime& t1In, const RawTime& t2In) {
        U64 t1 = ((U64)t1In.upper << 32) | t1In.lower;
        U64 t2 = ((U64)t2In.upper << 32) | t2In.lower;
        return static_cast<U32>(t1 - t2);
    }

    void IntervalTimer::getRawTime(RawTime& time) {
        TickType_t t = xTaskGetTickCount() * portTICK_PERIOD_MS;
        uint64_t timeUs = t * 1000;

        time.upper = (timeUs >> 32);
        time.lower = (timeUs & 0xFFFFFFFF);
    }
}