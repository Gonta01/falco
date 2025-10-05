#include <Falco/Drv/RS485Driver/RS485Driver.hpp>
#include "pio/plib_pio.h"

// TODO: RS485 should be in other namespace. For example, "Board"
namespace Falco {

RS485Driver ::RS485Driver(const char* compName)
    : RS485DriverComponentBase(compName) {

}

RS485Driver ::~RS485Driver(void) {}

void RS485Driver::configure() {
    RS485_DriverEnable_Clear();
}

Drv::PollStatus RS485Driver::recv_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& recvBuffer) {
    U32 numOfReceivedBytes = this->read_out(0, recvBuffer, readTimeout);
    if (numOfReceivedBytes == 0) {
        return Drv::PollStatus::POLL_RETRY;
    }
    return Drv::PollStatus::POLL_OK;
}

Drv::SendStatus RS485Driver::send_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& sendBuffer) {
    RS485_DriverEnable_Set();
    Drv::UART::WriteStatus writeStatus = this->write_out(0, sendBuffer, writeTimeout);
    RS485_DriverEnable_Clear();
    
    return ((writeStatus == Drv::UART::WriteStatus::SUCCESS) ? Drv::SendStatus::SEND_OK : Drv::SendStatus::SEND_ERROR);
}

}  // namespace Falco
