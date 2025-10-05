#include <Components/ComModule/ComModule.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Components {

ComModule ::ComModule(const char* compName)
    : ComModuleComponentBase(compName) {}

ComModule ::~ComModule(void) {}

void ComModule::configure() {
    if (this->isConnected_ready_OutputPort(0)) {
        this->ready_out(0);
    }
}

Drv::SendStatus ComModule ::send_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer) {
    Drv::UART::WriteStatus writeStatus = this->write_out(0, fwBuffer, writeTimeout);
    deallocate_out(0, fwBuffer);
    return ((writeStatus == Drv::UART::WriteStatus::SUCCESS) ? Drv::SendStatus::SEND_OK : Drv::SendStatus::SEND_ERROR);
}

void ComModule ::schedIn_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Fw::Buffer recv_buffer = this->allocate_out(0, SERIAL_BUFFER_SIZE);
    U32 numOfReadBytes = this->read_out(0, recv_buffer, readTimeout);
    recv_out(0, recv_buffer, Drv::RecvStatus::RECV_OK);
}

}  // namespace Components
