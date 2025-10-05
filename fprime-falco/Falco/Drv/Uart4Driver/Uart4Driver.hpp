#ifndef Uart4Driver_HPP
#define Uart4Driver_HPP

#include "Falco/Drv/Uart4Driver/Uart4DriverComponentAc.hpp"
#include "Os/Task.hpp"
#include "uart/plib_uart_common.h"
#include "xdmac/plib_xdmac_common.h"
#include "etl/array.h"
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

class Uart4Driver : public Uart4DriverComponentBase {
  public:
    const static FwSizeType SERIAL_BUFFER_SIZE = 64;  // Maximum buffer size

    //! Construct object Uart4Driver
    //!
    Uart4Driver(const char* compName /*!< The component name*/
    );

    //! Destroy object Uart4Driver
    //!
    ~Uart4Driver(void);

    //! Configure this port
    //!
    void configure();

  private:
    static constexpr NATIVE_UINT_TYPE transmitCompleteWaitMs = 1;
    
    static constexpr EventBits_t evtTransmitComplete = 1;
    static constexpr EventBits_t evtError = 2;

    EventGroupHandle_t eventGroupHandleUART4;

    Drv::UART::Diagnostics diagnostics;
    volatile U32 overrunError;
    volatile U32 parityError;
    volatile U32 framingError;
    volatile U32 receiveBufferFull;
    U32 transmitBufferOversize;
    U32 transmitFailed;

    //! Handler implementation for transmit
    //!
    //! Take data to write out via UART
    Drv::UART::WriteStatus transmit_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& buffer, //!< Buffer with data to write from
        U32 timeoutMs
    ) override;

    //! Handler implementation for receive
    //!
    //! Read data via UART
    U32 receive_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Buffer& buffer, //!< Buffer with data to read to
        U32 timeoutMs
    ) override;

    //! Handler implementation for getNumOfBytes
    //!
    //! Get number of available bytes for reading
    U32 getNumOfBytes_handler(
        NATIVE_INT_TYPE portNum //!< The port number
    ) override;

    //! Handler implementation for report
      //!
      //! Report UART diagnostics
      Drv::UART::Diagnostics report_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

    bool write_data(Fw::Buffer& fwBuffer, U32 timeoutMs);

    bool startTransmission(Fw::Buffer& fwBuffer);
    bool waitTransmissionEnd(U32 timeoutMs);

    static void transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects);
    void transmitCallbackHandler(XDMAC_TRANSFER_EVENT event);

    static void receiveCallback(UART_EVENT event, uintptr_t thisObjects);
    void receiveCallbackHandler(UART_EVENT event);

};

}  // end namespace Falco

#endif
