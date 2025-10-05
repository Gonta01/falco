// ======================================================================
// \title  GpioDriver.hpp
// \author lestarch
// \brief  hpp file for GpioDriver component implementation class
// ======================================================================

#ifndef SAMV71Q21B_DRV_GpioDriver_HPP
#define SAMV71Q21B_DRV_GpioDriver_HPP

#include "Falco/Drv/GpioDriver/GpioDriverComponentAc.hpp"
#include "peripheral/pio/plib_pio.h"

namespace Falco {

class GpioDriver : public GpioDriverComponentBase {
  public:
    //! configure GPIO
    enum GpioDirection {
        IN,   //!< input
        OUT,  //!< output
    };
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object GpioDriver
    //!
    GpioDriver(const char* const compName /*!< The component name*/
    );

    //! Initialize object GpioDriver
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    // Configure GPIO Driver
    void configure(PIO_PIN pin);

    //! Destroy object GpioDriver
    //!
    ~GpioDriver();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------
    PIO_PIN m_pin;
    bool isConfigured;
    
    //! Handler implementation for gpioRead
    //!
    void gpioRead_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                          Fw::Logic& state);

    //! Handler implementation for gpioWrite
    //!
    void gpioWrite_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                           const Fw::Logic& state);

};

}  // end namespace Drv

#endif
