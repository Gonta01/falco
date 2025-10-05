// ======================================================================
// \title  GpioDriverImpl.cpp
// \author lestarch
// \brief  cpp file for GpioDriver component implementation class
// ======================================================================

#include <Falco/Drv/GpioDriver/GpioDriver.hpp>
#include <FpConfig.hpp>
#include "peripheral/pio/plib_pio.h"

namespace Falco {

GpioDriver ::GpioDriver(const char* const compName)
: GpioDriverComponentBase(compName),
    isConfigured(false) 
{

}

GpioDriver ::~GpioDriver() {

}

void GpioDriver ::init(const NATIVE_INT_TYPE instance) {
    GpioDriverComponentBase::init(instance);
}

void GpioDriver ::configure(PIO_PIN pin) {
    m_pin = pin;
    isConfigured = true;
}

void GpioDriver ::gpioRead_handler(const NATIVE_INT_TYPE portNum, Fw::Logic& state) {
    if (isConfigured) {
        U8 value = PIO_PinRead(m_pin);
        state = (value == true) ? Fw::Logic::HIGH : Fw::Logic::LOW;
    }
}

void GpioDriver ::gpioWrite_handler(const NATIVE_INT_TYPE portNum, const Fw::Logic& state) {
    if (isConfigured) {
        PIO_PinWrite(m_pin, state == Fw::Logic::HIGH);
    }
}

}  // end namespace Drv
