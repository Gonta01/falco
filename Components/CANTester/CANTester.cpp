// ======================================================================
// \title  CANTester.cpp
// \author bdl
// \brief  cpp file for CANTester component implementation class
// ======================================================================

#include "Components/CANTester/CANTester.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"
#include "Falco/Drv/CanCommon/CANStandardMessage.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CANTester ::
    CANTester(const char* const compName) :
      CANTesterComponentBase(compName),
      CANMessagesCounter(0)
  {

  }

  CANTester ::
    ~CANTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void CANTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    constexpr uint32_t arraySize = 8;
    bool readSuccess = false;

    CANMessagesCounter++;
    uint32_t messageID = 0xcafe;
    etl::array<uint8_t, arraySize> writeArr{0};
    writeArr.at(0) = (CANMessagesCounter >> 24) & 0xFF;
    writeArr.at(1) = (CANMessagesCounter >> 16) & 0xFF;
    writeArr.at(2) = (CANMessagesCounter >> 8) & 0xFF;
    writeArr.at(3) = (CANMessagesCounter >> 0) & 0xFF;
    writeArr.at(4) = 0xDE;
    writeArr.at(5) = 0xAD;
    writeArr.at(6) = 0xBE;
    writeArr.at(7) = 0xEF;

    Drv::CAN::StandardFlags writeFlags;
    writeFlags.setID(messageID);
    writeFlags.setExtendedFrameFlag(Drv::CAN::FrameFormat::ExtendedFrame);
    Fw::Buffer writeBuff(writeArr.data(), writeArr.size());

    Drv::CAN::StandardFlags readFlags;
    etl::array<uint8_t, arraySize> readArr{};
    Fw::Buffer readBuff(readArr.data(), readArr.size());
    if (read_out(0, readBuff, readFlags, readTimeout) == Drv::CAN::Status::NoError) {
      if (write_out(0, writeBuff, writeFlags, writeTimeout) == Drv::CAN::Status::WriteFailed) {
        this->log_WARNING_HI_WriteError();
      }
    }
  }

}
