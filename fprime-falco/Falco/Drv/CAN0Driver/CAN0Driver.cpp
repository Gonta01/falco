#include <Falco/Drv/CAN0Driver/CAN0Driver.hpp>
#include "Falco/Drv/CanCommon/CANStandardMessage.hpp"
#include "mcan/plib_mcan0.h"
#include "mcan/plib_mcan_common.h"
#include "etl/algorithm.h"

namespace Falco {

CAN0Driver ::CAN0Driver(const char* compName)
    : CAN0DriverComponentBase(compName)
{

}

CAN0Driver ::~CAN0Driver(void) {}

void CAN0Driver::configure(Drv::CAN::NominalBitRate nominalBitRate) {
    MCAN_BIT_TIMING_SETUP mcanBitTimingSetup;
    mcanBitTimingSetup.nominalBitTimingSet = true;
    mcanBitTimingSetup.dataBitTimingSet = false;    
    configureNominalBitTimings(mcanBitTimingSetup, nominalBitRate); 
    MCAN_BIT_TIMING mcanBitTiming;
    MCAN0_BitTimingCalculationGet(&mcanBitTimingSetup, &mcanBitTiming);
    MCAN0_BitTimingSet(&mcanBitTiming); 
    configMCAN();
}

Drv::CAN::Status CAN0Driver::receive_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, Drv::CAN::StandardFlags& flags, U32 timeoutMs) {
  Interfaces::ICAN::CANMessage receivedCanMessage;
  Drv::CAN::Status readResult = read(receivedCanMessage, timeoutMs);

  if (readResult == Drv::CAN::Status::NoDataToRead) {
    return Drv::CAN::Status::NoDataToRead;
  }
  
  if (readResult == Drv::CAN::Status::NoError) {
    U8* buffData = buffer.getData();
    U8* messageData = receivedCanMessage.data.data();

    U32 numOfReceivedBytes = receivedCanMessage.getDataLengthCodeInBytes();
    U32 bufferSize = buffer.getSize();
    U32 messageSize = receivedCanMessage.data.size();

    auto bytesToStore = etl::min(numOfReceivedBytes, bufferSize);
    bytesToStore = etl::min(bytesToStore, messageSize);

    for (I8 i = 0; i < bytesToStore; i++) {
      *(buffData + i) = *(messageData + i); 
    }

    buffer.setSize(bytesToStore);

    flags.setID(receivedCanMessage.id);
    flags.setExtendedFrameFlag((receivedCanMessage.xtd == 1) ? Drv::CAN::FrameFormat::ExtendedFrame : Drv::CAN::FrameFormat::StandardFrame);
    flags.setRemoterFrameFlag((receivedCanMessage.rtr == 1) ? Drv::CAN::RemoteFrame::Enable : Drv::CAN::RemoteFrame::Disable);
  }

  return Drv::CAN::Status::NoError;
}

Drv::CAN::Status CAN0Driver::transmit_handler(NATIVE_INT_TYPE portNum, Fw::Buffer& buffer, const Drv::CAN::StandardFlags& flags, U32 timeoutMs) {
  etl::span<U8> spanBuff(buffer.getData(), buffer.getSize());

  Drv::CANStandardMessage message(spanBuff, flags.getID(), flags.getExtendedFrameFlag(), flags.getRemoterFrameFlag());
  Interfaces::ICAN::CANMessage sendCanMessage;
  sendCanMessage.id = message.getID();
  sendCanMessage.xtd = message.getExtendedFrameFlag();
  sendCanMessage.dlc = message.getDataLengthCode();
  sendCanMessage.rtr = message.getRemoteFrameFlag();

  message.getData(sendCanMessage.data);

  Drv::CAN::Status writeResult = write(sendCanMessage, timeoutMs);
  if (writeResult == Drv::CAN::Status::WriteFailed) {
    return Drv::CAN::Status::WriteFailed;
  }

  return Drv::CAN::Status::NoError;
}

}  // namespace Falco
