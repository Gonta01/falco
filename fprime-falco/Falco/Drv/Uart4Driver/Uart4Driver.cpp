#include <Falco/Drv/Uart4Driver/Uart4Driver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "peripheral/uart/plib_uart4.h"
#include "xdmac/plib_xdmac_common.h"
#include "xdmac/plib_xdmac.h"
#include "etl/array.h"
#include "etl/algorithm.h"
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

const FwSizeType UART4_BUFFER_SIZE = 1024;
// Buffer in non cached memory space is used to prevent cache coherence issues
etl::array<U8, UART4_BUFFER_SIZE> UART4Buffer __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

Uart4Driver ::Uart4Driver(const char* compName)
    : Uart4DriverComponentBase(compName),
    overrunError(0),
    parityError(0),
    framingError(0),
    receiveBufferFull(0),
    transmitBufferOversize(0),
    transmitFailed(0) {
        eventGroupHandleUART4 = xEventGroupCreate();
    }

Uart4Driver ::~Uart4Driver(void) {}

void Uart4Driver::configure() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_5, transmitCallback, reinterpret_cast<uintptr_t>(this));
    UART4_ReadCallbackRegister(receiveCallback, reinterpret_cast<uintptr_t>(this));
}

Drv::UART::WriteStatus Uart4Driver::transmit_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    if (buffer.getSize() > UART4_BUFFER_SIZE) {
        transmitBufferOversize++;
        return Drv::UART::WriteStatus::FAILED;
    }
    etl::copy_n_s(buffer.getData(), buffer.getSize(), UART4Buffer.begin(), UART4Buffer.end());
    Fw::Buffer writeBuffer(UART4Buffer.data(), buffer.getSize());
    
    if (write_data(writeBuffer, timeoutMs) == false) {
        transmitFailed++;
        return Drv::UART::WriteStatus::FAILED;
    }
    return Drv::UART::WriteStatus::SUCCESS;
}

U32 Uart4Driver::receive_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    U32 numOfReadBytes = UART4_Read(buffer.getData(), buffer.getSize());
    buffer.setSize(numOfReadBytes); 
    return numOfReadBytes;
}

U32 Uart4Driver::getNumOfBytes_handler(const NATIVE_INT_TYPE portNum) {
    return UART4_ReadCountGet();
}

Drv::UART::Diagnostics Uart4Driver::report_handler(NATIVE_INT_TYPE portNum) {
    diagnostics.setOverrun(overrunError);
    diagnostics.setParity(parityError);
    diagnostics.setFraming(framingError);
    diagnostics.setReceiveBufferFull(receiveBufferFull);
    diagnostics.setTransmitBufferOversize(transmitBufferOversize);
    diagnostics.setTransmitFailed(transmitFailed);

    return diagnostics;
}

bool Uart4Driver ::write_data(Fw::Buffer& buffer, U32 timeoutMs) {
    bool writeStatus = false;

    if (startTransmission(buffer)) {
        if (waitTransmissionEnd(timeoutMs)) {
            writeStatus = true;
        }
    }

    return writeStatus;
}

bool Uart4Driver::startTransmission(Fw::Buffer& buffer) {
    return XDMAC_ChannelTransfer(XDMAC_CHANNEL_5, buffer.getData(), (const void *) (&UART4_REGS->UART_THR), buffer.getSize());
}
 
bool Uart4Driver::waitTransmissionEnd(U32 timeoutMs) {
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transmissionResult = xEventGroupWaitBits(eventGroupHandleUART4,
                        evtTransmitComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);
    
    constexpr auto maxMumOfTries = 10;
    auto numOfTries = 0;
    bool transmitComplete = true;
    while(!UART4_TransmitComplete()) {
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

void Uart4Driver::transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart4Driver *>(thisObjects)->transmitCallbackHandler(event);
}

void Uart4Driver::transmitCallbackHandler(XDMAC_TRANSFER_EVENT event) {
    EventBits_t events = evtError;
    if (event == XDMAC_TRANSFER_COMPLETE) {
        events = evtTransmitComplete;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(eventGroupHandleUART4, events, &xHigherPriorityTaskWoken);
}

void Uart4Driver::receiveCallback(UART_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart4Driver *>(thisObjects)->receiveCallbackHandler(event);
}

void Uart4Driver::receiveCallbackHandler(UART_EVENT event) {
    if (event == UART_EVENT_READ_ERROR) {
        UART_ERROR error = UART4_ErrorGet();
        overrunError = ((error & UART_SR_OVRE_Msk) == UART_ERROR_OVERRUN) ? (overrunError + 1) : overrunError;
        parityError = ((error & UART_SR_PARE_Msk) == UART_ERROR_PARITY) ? (parityError + 1) : parityError;
        framingError = ((error & UART_SR_FRAME_Msk) == UART_ERROR_FRAMING) ? (framingError + 1) : framingError;
    }

    if (event == UART_EVENT_READ_BUFFER_FULL) {
        receiveBufferFull++;
    }
}

}  // namespace Falco
