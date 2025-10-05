#ifndef RS485Driver_HPP
#define RS485Driver_HPP

#include "Falco/Drv/RS485Driver/RS485DriverComponentAc.hpp"

namespace Falco {

class RS485Driver : public RS485DriverComponentBase {
  public:

    //! Construct object RS485Driver
    //!
    RS485Driver(const char* compName /*!< The component name*/
    );

    //! Destroy object RS485Driver
    //!
    ~RS485Driver(void);

    //! Configure this port
    //!
    void configure();

  private:
    static constexpr U32 writeTimeout = 0xFFFF'FFFF;
    static constexpr U32 readTimeout = 0xFFFF'FFFF;

    //! Handler implementation for recv
    //!
    //! Receive data through RS485
    Drv::PollStatus recv_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& pollBuffer
    ) override;

    //! Handler implementation for send
    //!
    //! Send data through RS485
    Drv::SendStatus send_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& sendBuffer //!< Data to send
    ) override;

};

}  // end namespace Falco

#endif
