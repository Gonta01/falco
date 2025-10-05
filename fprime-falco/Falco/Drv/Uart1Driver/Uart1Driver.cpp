#include <Falco/Drv/Uart1Driver/Uart1Driver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "peripheral/uart/plib_uart1.h"
#include "xdmac/plib_xdmac_common.h"
#include "xdmac/plib_xdmac.h"
#include "etl/array.h"
#include "etl/algorithm.h"
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

const FwSizeType UART1_BUFFER_SIZE = 1024;
// Buffer in non cached memory space is used to prevent cache coherence issues
etl::array<U8, UART1_BUFFER_SIZE> UART1Buffer __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

Uart1Driver ::Uart1Driver(const char* compName)
    : Uart1DriverComponentBase(compName),
    overrunError(0),
    parityError(0),
    framingError(0),
    receiveBufferFull(0),
    transmitBufferOversize(0),
    transmitFailed(0) {
        eventGroupHandleUART1 = xEventGroupCreate();
    }

Uart1Driver ::~Uart1Driver(void) {}

void Uart1Driver::configure() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_4, transmitCallback, reinterpret_cast<uintptr_t>(this));
    UART1_ReadCallbackRegister(receiveCallback, reinterpret_cast<uintptr_t>(this));
}

Drv::UART::WriteStatus Uart1Driver::transmit_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    if (buffer.getSize() > UART1_BUFFER_SIZE) {
        transmitBufferOversize++;
        return Drv::UART::WriteStatus::FAILED;
    }
    etl::copy_n_s(buffer.getData(), buffer.getSize(), UART1Buffer.begin(), UART1Buffer.end());
    Fw::Buffer writeBuffer(UART1Buffer.data(), buffer.getSize());
    
    if (write_data(writeBuffer, timeoutMs) == false) {
        transmitFailed++;
        return Drv::UART::WriteStatus::FAILED;
    }
    return Drv::UART::WriteStatus::SUCCESS;
}

U32 Uart1Driver::receive_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    U32 numOfReadBytes = UART1_Read(buffer.getData(), buffer.getSize());
    buffer.setSize(numOfReadBytes); 
    return numOfReadBytes;
}

Drv::UART::Diagnostics Uart1Driver::report_handler(NATIVE_INT_TYPE portNum) {
    diagnostics.setOverrun(overrunError);
    diagnostics.setParity(parityError);
    diagnostics.setFraming(framingError);
    diagnostics.setReceiveBufferFull(receiveBufferFull);
    diagnostics.setTransmitBufferOversize(transmitBufferOversize);
    diagnostics.setTransmitFailed(transmitFailed);

    return diagnostics;
}

bool Uart1Driver ::write_data(Fw::Buffer& buffer, U32 timeoutMs) {
    bool writeStatus = false;

    if (startTransmission(buffer)) {
        if (waitTransmissionEnd(timeoutMs)) {
            writeStatus = true;
        }
    }

    return writeStatus;
}

bool Uart1Driver::startTransmission(Fw::Buffer& buffer) {
    return XDMAC_ChannelTransfer(XDMAC_CHANNEL_4, buffer.getData(), (const void *) (&UART1_REGS->UART_THR), buffer.getSize());
}
 
bool Uart1Driver::waitTransmissionEnd(U32 timeoutMs) {
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transmissionResult = xEventGroupWaitBits(eventGroupHandleUART1,
                        evtTransmitComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);
    
    constexpr auto maxMumOfTries = 10;
    auto numOfTries = 0;
    bool transmitComplete = true;
    while(!UART1_TransmitComplete()) {
        if (numOfTries >= maxMumOfTries) {
            transmitComplete = false;
            break;
        }
        Os::Task::delay(transmitCompleteWaitMs);
        numOfTries++;
    }
    
    if (transmitComplete == false) {
        return false;
    }

    return (transmissionResult == evtTransmitComplete);
}

void Uart1Driver::transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart1Driver *>(thisObjects)->transmitCallbackHandler(event);
}

void Uart1Driver::transmitCallbackHandler(XDMAC_TRANSFER_EVENT event) {
    EventBits_t events = evtError;
    if (event == XDMAC_TRANSFER_COMPLETE) {
        events = evtTransmitComplete;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(eventGroupHandleUART1, events, &xHigherPriorityTaskWoken);
}

void Uart1Driver::receiveCallback(UART_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart1Driver *>(thisObjects)->receiveCallbackHandler(event);
}

void Uart1Driver::receiveCallbackHandler(UART_EVENT event) {
    if (event == UART_EVENT_READ_ERROR) {
        UART_ERROR error = UART1_ErrorGet();
        overrunError = ((error & UART_SR_OVRE_Msk) == UART_ERROR_OVERRUN) ? (overrunError + 1) : overrunError;
        parityError = ((error & UART_SR_PARE_Msk) == UART_ERROR_PARITY) ? (parityError + 1) : parityError;
        framingError = ((error & UART_SR_FRAME_Msk) == UART_ERROR_FRAMING) ? (framingError + 1) : framingError;
    }

    if (event == UART_EVENT_READ_BUFFER_FULL) {
        receiveBufferFull++;
    }
}

}  // namespace Falco
