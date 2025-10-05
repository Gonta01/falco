#ifndef CAN1FDDriver_HPP
#define CAN1FDDriver_HPP

#include "Falco/Drv/CAN1FDDriver/CAN1FDDriverComponentAc.hpp"
#include "Falco/Drv/CanCommon/CAN.hpp"

namespace Falco {

class CAN1FDDriver : public CAN1FDDriverComponentBase, public Base::CAN1Base {
  public:
    //! Construct object CAN1FDDriver
    //!
    CAN1FDDriver(const char* compName /*!< The component name*/
    );

    //! Destroy object CAN1FDDriver
    //!
    ~CAN1FDDriver(void);

    //! Configure this port
    //!
    void configure(Drv::CAN::NominalBitRate nominalBitRate, Drv::CAN::DataBitRate dataBitRate);
    
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
          Drv::CAN::FDFlags& flags,
          U32 timeoutMs
      ) override;

      //! Handler implementation for transmit
      //!
      //! Take data to write out via CAN
      Drv::CAN::Status transmit_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Fw::Buffer& buffer,
          const Drv::CAN::FDFlags& flags,
          U32 timeoutMs
      ) override;
};

}  // end namespace Falco

#endif
