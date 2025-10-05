#include <Falco/Drv/Uart0Driver/Uart0Driver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "uart/plib_uart0.h"
#include "xdmac/plib_xdmac_common.h"
#include "xdmac/plib_xdmac.h"
#include "etl/array.h"
#include "etl/algorithm.h"
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

const FwSizeType UART0_BUFFER_SIZE = 1024;
// Buffer in non cached memory space is used to prevent cache coherence issues
etl::array<U8, UART0_BUFFER_SIZE> UART0Buffer __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

Uart0Driver ::Uart0Driver(const char* compName)
    : Uart0DriverComponentBase(compName),
    overrunError(0),
    parityError(0),
    framingError(0),
    receiveBufferFull(0),
    transmitBufferOversize(0),
    transmitFailed(0)
{
    eventGroupHandleUART0 = xEventGroupCreate();
}

Uart0Driver ::~Uart0Driver(void) {}

void Uart0Driver::configure() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_3, transmitCallback, reinterpret_cast<uintptr_t>(this));
    UART0_ReadCallbackRegister(receiveCallback, reinterpret_cast<uintptr_t>(this));
}

Drv::UART::WriteStatus Uart0Driver::transmit_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    if (buffer.getSize() > UART0_BUFFER_SIZE) {
        transmitBufferOversize++;
        return Drv::UART::WriteStatus::FAILED;
    }
    etl::copy_n_s(buffer.getData(), buffer.getSize(), UART0Buffer.begin(), UART0Buffer.end());
    Fw::Buffer writeBuffer(UART0Buffer.data(), buffer.getSize());
    
    if (writeData(writeBuffer, timeoutMs) == false) {
        transmitFailed++;
        return Drv::UART::WriteStatus::FAILED;
    }
    return Drv::UART::WriteStatus::SUCCESS;
}

U32 Uart0Driver::receive_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, U32 timeoutMs) {
    U32 numOfReadBytes = UART0_Read(buffer.getData(), buffer.getSize());
    buffer.setSize(numOfReadBytes); 
    return numOfReadBytes;
}

Drv::UART::Diagnostics Uart0Driver::report_handler(NATIVE_INT_TYPE portNum) {
    diagnostics.setOverrun(overrunError);
    diagnostics.setParity(parityError);
    diagnostics.setFraming(framingError);
    diagnostics.setReceiveBufferFull(receiveBufferFull);
    diagnostics.setTransmitBufferOversize(transmitBufferOversize);
    diagnostics.setTransmitFailed(transmitFailed);

    return diagnostics;
}

bool Uart0Driver ::writeData(Fw::Buffer& fwBuffer, U32 timeoutMs) {
    bool writeStatus = false;

    receiverDisable();
    emptyRxBuffer();
    if (startTransmission(fwBuffer)) {
        if (waitTransmissionEnd(timeoutMs)) {
            writeStatus = true;
        }
    }
    receiverEnable();

    return writeStatus;
}

bool Uart0Driver::startTransmission(Fw::Buffer& fwBuffer) {
    return XDMAC_ChannelTransfer(XDMAC_CHANNEL_3, fwBuffer.getData(), (const void *) (&UART0_REGS->UART_THR), fwBuffer.getSize());
}
 
bool Uart0Driver::waitTransmissionEnd(U32 timeoutMs) {
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transmissionResult = xEventGroupWaitBits(eventGroupHandleUART0,
                        evtTransmitComplete | evtError,
                        pdTRUE,
                        pdFALSE,
                        ticks);
    
    constexpr auto maxMumOfTries = 10;
    auto numOfTries = 0;
    bool transmitComplete = true;
    while(!UART0_TransmitComplete()) {
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

void Uart0Driver::transmitCallback(XDMAC_TRANSFER_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart0Driver *>(thisObjects)->transmitCallbackHandler(event);
}

void Uart0Driver::transmitCallbackHandler(XDMAC_TRANSFER_EVENT event) {
    EventBits_t events = evtError;
    if (event == XDMAC_TRANSFER_COMPLETE) {
        events = evtTransmitComplete;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(eventGroupHandleUART0, events, &xHigherPriorityTaskWoken);
}

void Uart0Driver::receiverDisable() {
    UART0_REGS->UART_CR = UART_CR_RSTRX_Msk;
    UART0_REGS->UART_CR = UART_CR_RXDIS_Msk;
}

void Uart0Driver::receiverEnable() {
    UART0_REGS->UART_CR = UART_CR_RXEN_Msk;
}

void Uart0Driver::emptyRxBuffer() {
    U8 recvByte = 0;
    while (UART0_Read(&recvByte, sizeof(recvByte)) != 0);
}

void Uart0Driver::receiveCallback(UART_EVENT event, uintptr_t thisObjects) {
    reinterpret_cast<Uart0Driver *>(thisObjects)->receiveCallbackHandler(event);
}

void Uart0Driver::receiveCallbackHandler(UART_EVENT event) {
    if (event == UART_EVENT_READ_ERROR) {
        UART_ERROR error = UART0_ErrorGet();
        overrunError = ((error & UART_SR_OVRE_Msk) == UART_ERROR_OVERRUN) ? (overrunError + 1) : overrunError;
        parityError = ((error & UART_SR_PARE_Msk) == UART_ERROR_PARITY) ? (parityError + 1) : parityError;
        framingError = ((error & UART_SR_FRAME_Msk) == UART_ERROR_FRAMING) ? (framingError + 1) : framingError;
    }

    if (event == UART_EVENT_READ_BUFFER_FULL) {
        receiveBufferFull++;
    }
}

}  // namespace Falco
