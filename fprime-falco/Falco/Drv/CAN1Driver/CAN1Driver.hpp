#ifndef CAN1Driver_HPP
#define CAN1Driver_HPP

#include "Falco/Drv/CAN1Driver/CAN1DriverComponentAc.hpp"
#include "Falco/Drv/CanCommon/CAN.hpp"

namespace Falco {

class CAN1Driver : public CAN1DriverComponentBase, public Base::CAN1Base {
  public:
    //! Construct object CAN1Driver
    //!
    CAN1Driver(const char* compName /*!< The component name*/
    );

    //! Destroy object CAN1Driver
    //!
    ~CAN1Driver(void);

    //! Configure this port
    //!
    void configure(Drv::CAN::NominalBitRate nominalBitRate);
    
  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for receive
    //!
    //! Read data via CAN
    Drv::CAN::Status receive_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& buffer,
        Drv::CAN::StandardFlags& flags,
        U32 timeoutMs
    ) override;

    //! Handler implementation for transmit
    //!
    //! Take data to write out via CAN
    Drv::CAN::Status transmit_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& buffer,
        const Drv::CAN::StandardFlags& flags,
        U32 timeoutMs
    ) override;
};

}  // end namespace Falco

#endif
