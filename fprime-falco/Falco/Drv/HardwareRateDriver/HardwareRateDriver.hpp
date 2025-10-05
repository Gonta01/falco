#ifndef SAMV71Q21B_DRV_HARDWARERATEDRIVER_HPP
#define SAMV71Q21B_DRV_HARDWARERATEDRIVER_HPP
#include <Svc/Cycle/TimerVal.hpp>
#include <Falco/Drv/HardwareRateDriver/HardwareRateDriverComponentAc.hpp>
#include "peripheral/tc/plib_tc_common.h"

namespace Falco {
    /**
     * HardwareRateDriver:
     *
     * Used to drive the rate group driver via the CycleIn port. That port is
     * triggered via a timer interrupt.
     */
    class HardwareRateDriver : public HardwareRateDriverComponentBase  {
        public:
            /**
             * Construct the rate driver. Takes in a name (if configured) and a
             * rate at witch to drive.
             * \param const char* compName: name of the component (only supply if configured)
             */
            HardwareRateDriver(const char* compName);

            /**
             * Starts this driver, kicking off the hardware interrupt or the Linux
             * task that virtualizes this driver.
             */
            void startTimer();

            /**
             * Stops the hardware driver, enabling some clean-up.
             */
            void stop();

            //!< Driver deconstructor
            ~HardwareRateDriver(void);
            
        private:
            //!< Last time of run
            Svc::TimerVal m_last;
            //!< Pointer to the driver
            static HardwareRateDriver* s_driver;

            static void timer0Callback(TC_TIMER_STATUS status, uintptr_t context);
            
            //!< Static callback to the ISR triggered via a timer
            static void s_timerISR(TC_TIMER_STATUS status);

            static void s_timer(void * comp);

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for CycleIn
        //!
        //! The rate group scheduler input
        void CycleIn_handler(
            NATIVE_INT_TYPE portNum, //!< The port number
            Svc::TimerVal& cycleStart //!< Cycle start timer value
        ) override;

    };
}
#endif
