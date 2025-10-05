// ======================================================================
// \title  CANFDTester.cpp
// \author user
// \brief  cpp file for CANFDTester component implementation class
// ======================================================================

#include "Components/CANFDTester/CANFDTester.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CANFDTester ::
    CANFDTester(const char* const compName) :
      CANFDTesterComponentBase(compName)
  {

  }

  CANFDTester ::
    ~CANFDTester()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void CANFDTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    constexpr uint32_t arraySize = 64;
    bool readSuccess = false;

    CANFDMessagesCounter++;
    uint32_t messageID = 0xfd00;
    etl::array<uint8_t, arraySize> writeArr{0};
    writeArr.at(0) = (CANFDMessagesCounter >> 24) & 0xFF;
    writeArr.at(1) = (CANFDMessagesCounter >> 16) & 0xFF;
    writeArr.at(2) = (CANFDMessagesCounter >> 8) & 0xFF;
    writeArr.at(3) = (CANFDMessagesCounter >> 0) & 0xFF;
    writeArr.at(4) = 0xDE;
    writeArr.at(5) = 0xAD;
    writeArr.at(6) = 0xBE;
    writeArr.at(7) = 0xEF;

    writeArr.at(60) = 0xCC;
    writeArr.at(61) = 0xDD;
    writeArr.at(62) = 0xEE;
    writeArr.at(63) = 0xFF;

    Drv::CAN::FDFlags writeFlags;
    writeFlags.setID(messageID);
    writeFlags.setExtendedFrameFlag(Drv::CAN::FrameFormat::ExtendedFrame);
    writeFlags.setFDFrameFlag(Drv::CAN::FrameFD::Enable);
    writeFlags.setBitRateSwitchFlag(Drv::CAN::BitRateSwitch::Enable);
    Fw::Buffer writeBuff(writeArr.data(), writeArr.size());

    Drv::CAN::FDFlags readFlags;
    etl::array<uint8_t, arraySize> readArr{};
    Fw::Buffer readBuff(readArr.data(), readArr.size());
    if (read_out(0, readBuff, readFlags, readTimeout) == Drv::CAN::Status::NoError) {
      if (write_out(0, writeBuff, writeFlags, writeTimeout) == Drv::CAN::Status::WriteFailed) {
        this->log_WARNING_HI_WriteError();
      }
    }
  }
}
