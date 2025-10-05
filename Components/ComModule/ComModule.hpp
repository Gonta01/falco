#ifndef ComModule_HPP
#define ComModule_HPP

#include "Components/ComModule/ComModuleComponentAc.hpp"
#include "Os/Task.hpp"

namespace Components {

class ComModule : public ComModuleComponentBase {
  public:
    const static FwSizeType SERIAL_BUFFER_SIZE = 64;  // Maximum buffer size

    //! Construct object ComModule
    //!
    ComModule(const char* compName /*!< The component name*/
    );

    //! Destroy object ComModule
    //!
    ~ComModule(void);

    //! Configure this port
    //!
    void configure();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    static constexpr U32 writeTimeout = 0xFFFF'FFFF;
    static constexpr U32 readTimeout = 0xFFFF'FFFF;
    
    //! Handler implementation for send
    //!
    Drv::SendStatus send_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                 Fw::Buffer& fwBuffer) override;

    //! Handler implementation for schedIn
    //!
    void schedIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                         NATIVE_UINT_TYPE context       /*!< The call order*/
    );

};

}  // end namespace Components

#endif
