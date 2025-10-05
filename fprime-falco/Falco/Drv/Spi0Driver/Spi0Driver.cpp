// ======================================================================
// \title  Spi0Driver.cpp
// \author user
// \brief  cpp file for Spi0Driver component implementation class
// ======================================================================

#include "FpConfig.hpp"
#include "Falco/Drv/Spi0Driver/Spi0Driver.hpp"

#include "etl/span.h"
#include "etl/algorithm.h"
#include "samv71q21b.h"
#include "xdmac/plib_xdmac.h"
#include "xdmac/plib_xdmac_common.h"
#include "spi/spi_master/plib_spi0_master.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {
  const uint32_t NON_CACHED_SPI_WRITE_BUFFER_SIZE = 256;
  const uint32_t NON_CACHED_SPI_READ_BUFFER_SIZE = 256;
  // Buffer in non cached memory space is used to prevent cache coherence issues
  etl::array<uint8_t, NON_CACHED_SPI_WRITE_BUFFER_SIZE> SPIBufferWrite __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));
  etl::array<uint8_t, NON_CACHED_SPI_READ_BUFFER_SIZE> SPIBufferRead __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Spi0Driver ::
    Spi0Driver(const char* const compName) :
      Spi0DriverComponentBase(compName)
  {
    eventGroupHandleSPI0 = xEventGroupCreate();
  }

  Spi0Driver ::
    ~Spi0Driver()
  {

  }

  void Spi0Driver :: configure() {
    initialize();
  }

  void Spi0Driver::initialize() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_1, transmitCallback, reinterpret_cast<uintptr_t>(this));
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_0, receiveCallback, reinterpret_cast<uintptr_t>(this));
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void Spi0Driver::reinit_handler(NATIVE_INT_TYPE portNum) {
    SPI0_Initialize();
    initialize();
  }

  Drv::SPI::Status Spi0Driver :: read_handler(  NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    
    if (buffer.getSize() > SPIBufferRead.size()) {
      return Drv::SPI::Status::BufferOutOfRange;
    }

    Drv::SPI::Status status = Drv::SPI::Status::IOError;
    U32 numOfBytesToRead = buffer.getSize();

    etl::fill(SPIBufferWrite.begin(), SPIBufferWrite.end(), 0);

    if (startReception(numOfBytesToRead)) {
      if (waitReceptionEnd(timeoutMs)) {
          status = Drv::SPI::Status::NoError;
          etl::copy_n(SPIBufferRead.begin(), numOfBytesToRead, buffer.getData());
      }
    }

    return status;
  }

  Drv::SPI::Status Spi0Driver :: write_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    if (buffer.getSize() > SPIBufferWrite.size()) {
      return Drv::SPI::Status::BufferOutOfRange;
    }
    
    U32 numOfBytesToWrite = buffer.getSize();
    etl::copy_n_s(buffer.getData(), numOfBytesToWrite, SPIBufferWrite.begin(), SPIBufferWrite.end());

    Drv::SPI::Status status = Drv::SPI::Status::IOError;
    if (startTransmission(numOfBytesToWrite)) {
      if (waitTransmissionEnd(timeoutMs)) {
        status = Drv::SPI::Status::NoError;
      }
    }

    return status;
  }

  // Helper functions
  void Spi0Driver::receiveCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects) {
    //NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,performance-no-int-to-ptr)
    reinterpret_cast<Spi0Driver *>(thisObjects)->receiveCallbackHandler(event);
  }

  void Spi0Driver::receiveCallbackHandler(XDMAC_TRANSFER_EVENT event) {
    EventBits_t events = evtError;
    if (event == XDMAC_TRANSFER_COMPLETE) {
      events = evtReceiveComplete;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(eventGroupHandleSPI0, events, &xHigherPriorityTaskWoken);
  }

  void Spi0Driver::transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects) {
    //NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,performance-no-int-to-ptr)
    reinterpret_cast<Spi0Driver *>(thisObjects)->transmitCallbackHandler(event);
  }

  void Spi0Driver::transmitCallbackHandler(XDMAC_TRANSFER_EVENT event) {
    EventBits_t events = evtError;
    if (event == XDMAC_TRANSFER_COMPLETE) {
      events = evtTransmitComplete;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(eventGroupHandleSPI0, events, &xHigherPriorityTaskWoken);
  }

  bool Spi0Driver::startReception(size_t numOfBytes) {
    flush();
    if (numOfBytes > SPIBufferWrite.size()) {
        return false;
    }
    bool result = false;
    //NOLINTBEGIN(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
    if (XDMAC_ChannelTransfer(
                XDMAC_CHANNEL_0, (const void *) (&SPI0_REGS->SPI_RDR), SPIBufferRead.data(), numOfBytes)) {
        if (XDMAC_ChannelTransfer(
                    XDMAC_CHANNEL_1, SPIBufferWrite.data(), (const void *) (&SPI0_REGS->SPI_TDR), numOfBytes)) {
            result = true;
        }
    }
    //NOLINTEND(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
    return result;
  }

  bool Spi0Driver::waitReceptionEnd(U32 timeoutMs) {
    /* We need to wait for two events because during data receiving we are using two DMA channels
     *  and both of them will generate its own interrupt */
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transmissionResult0 = xEventGroupWaitBits(eventGroupHandleSPI0,
                        evtTransmitComplete | evtReceiveComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);

    auto transmissionResult1 = xEventGroupWaitBits(eventGroupHandleSPI0,
                        evtTransmitComplete | evtReceiveComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);

    return (transmissionResult0 != evtError && transmissionResult1 != evtError);
  }

  bool Spi0Driver::startTransmission(size_t numOfBytes) {
    if (numOfBytes > SPIBufferWrite.size()) {
      return false;
    }
    flush();
    //NOLINTBEGIN(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
    return XDMAC_ChannelTransfer(
            XDMAC_CHANNEL_1, SPIBufferWrite.data(), (const void *) (&SPI0_REGS->SPI_TDR), numOfBytes);
    //NOLINTEND(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
  }

  bool Spi0Driver::waitTransmissionEnd(U32 timeoutMs) {
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transmissionResult = xEventGroupWaitBits(eventGroupHandleSPI0,
                        evtTransmitComplete | evtReceiveComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);
    return (transmissionResult == evtTransmitComplete);
  }

  void Spi0Driver::flush() {
    volatile uint32_t const receivedData = (SPI0_REGS->SPI_RDR & SPI_RDR_RD_Msk) >> SPI_RDR_RD_Pos; // NOLINT
    (void) receivedData;                                                                            // NOLINT
  }

}
