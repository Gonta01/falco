// ======================================================================
// \title  RS485Tester.cpp
// \author BDL
// \brief  cpp file for RS485Tester component implementation class
// ======================================================================

#include "Components/RS485Tester/RS485Tester.hpp"
#include "FpConfig.hpp"
#include "etl/string.h"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  RS485Tester ::
    RS485Tester(const char* const compName) :
      RS485TesterComponentBase(compName)
  {

  }

  RS485Tester ::
    ~RS485Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void RS485Tester :: run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    etl::array<U8, 128> readBuffer;
    Fw::Buffer rBuff(readBuffer.data(), readBuffer.size());

    Drv::PollStatus pollingResult = this->receive_out(0, rBuff);
    if (pollingResult == Drv::PollStatus::POLL_OK) {
      while (pollingResult == Drv::PollStatus::POLL_OK) {
        Os::Task::delay(2);
        pollingResult = this->receive_out(0, rBuff);
      }

      readyToTransmit = true;
    }
    
    if (readyToTransmit) {
      const size_t size = 1024;
      static etl::string<size> writeBuff = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 \n\r";
  
      Fw::Buffer writeBuffer(reinterpret_cast<U8*>(writeBuff.data()), writeBuff.size());
  
      Drv::SendStatus sendStatus = this->transmit_out(0, writeBuffer);
      readyToTransmit = false;
    }
  }
}
