// ======================================================================
// \title  Spi0Driver.hpp
// \author user
// \brief  hpp file for Spi0Driver component implementation class
// ======================================================================

#ifndef Drv_Spi0Driver_HPP
#define Drv_Spi0Driver_HPP

#include "Falco/Drv/Spi0Driver/Spi0DriverComponentAc.hpp"

#include "etl/span.h"
#include "xdmac/plib_xdmac_common.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

  class Spi0Driver :
    public Spi0DriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Spi0Driver object
      Spi0Driver(
          const char* const compName //!< The component name
      );

      //! Destroy Spi0Driver object
      ~Spi0Driver();

      //! Configure driver
      //!
      void configure();

      PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      
      //! Handler implementation for reinit
      //!
      //! Reinitialize SPI peripheral
      void reinit_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for read
      //!
      //! Receive data through SPI
      Drv::SPI::Status read_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Fw::Buffer& buffer,
          U32 timeoutMs
      ) override;

      //! Handler implementation for write
      //!
      //! Send data through SPI
      Drv::SPI::Status write_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Fw::Buffer& buffer,
          U32 timeoutMs
      ) override;

      //! Helper functions

      static constexpr uint8_t evtReceiveComplete = 1;
      static constexpr uint8_t evtTransmitComplete = 2;
      static constexpr uint8_t evtError = 3;

      EventGroupHandle_t eventGroupHandleSPI0;

      /**
       * @brief Peipheral initialization for driver functioning
       */
      void initialize();

      /**
       * @brief Flush out any unread data in SPI read buffer from the previous transfer
       */
      void flush();

      /**
       * @brief Function start data reception.
       *        For data reception two DMA channels are using. The first for SCK generating, the second for
       *        actual data receiving
       * @param rxBuff buffer where data will be stored when reception complete
       * @return true - reception started successfully, false - reception not started
       */
      bool startReception(size_t numOfBytes);

      /**
       * @brief Function wait till reception complete
       * @return true - reception complete successfully, false - error happens during the receive procedure execution
       */
      bool waitReceptionEnd(U32 timeoutMs);

      /**
       * @brief Data receiving complete callback function
       * @param event data transmission status
       * @param thisObjects reference of current object
       */
      static void receiveCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects);

      /**
       * @brief Handler of receive complete function
       * @param event status of end write data
       */
      void receiveCallbackHandler(XDMAC_TRANSFER_EVENT event);

      /**
       * @brief Function start data transmission.
       * @param txBuff Buffer to be send
       * @return true - transmission started successfully, false - transmission not started
       */
      bool startTransmission(size_t numOfBytes);

      /**
       * @brief Function wait till transmission complete
       * @return true - transmission complete successfully, false - error happens during the transmit procedure execution
       */
      bool waitTransmissionEnd(U32 timeoutMs);

      /**
       * @brief Data transmission complete callback function
       * @param event data transmission status
       * @param thisObjects reference of current object
       */
      static void transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects);

      /**
       * @brief Handler of transmit complete function
       * @param event status of end write data
       */
      void transmitCallbackHandler(XDMAC_TRANSFER_EVENT event);
  };

}

#endif
