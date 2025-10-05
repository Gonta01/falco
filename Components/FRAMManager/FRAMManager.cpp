// ======================================================================
// \title  FRAMManager.cpp
// \author user
// \brief  cpp file for FRAMManager component implementation class
// ======================================================================

#include "Components/FRAMManager/FRAMManager.hpp"
#include "FpConfig.hpp"
#include <etl/span.h>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FRAMManager ::
    FRAMManager(const char* const compName) :
      FRAMManagerComponentBase(compName)
  {

  }

  FRAMManager ::
    ~FRAMManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  void FRAMManager ::
  requestWrite_handler(
      NATIVE_INT_TYPE portNum,
      U32 addr,
      Fw::Buffer& buffer
  )
  {
    // Analyze write buffer
    constexpr U32 FRAMLimitedSize = 256;
    U32 fullParts = buffer.getSize() / FRAMLimitedSize; // TODO: Instead of using of magic number make call to FRAMDriver to receive limitation value
    U32 remainingBytes = buffer.getSize() % FRAMLimitedSize; // TODO: The same as above
    
    // Write main chunks
    U32 inAddr = addr;
    for (U32 i = 0; i < fullParts; i++) {
      etl::span<U8> internalWriteBuffer(buffer.getData() + FRAMLimitedSize * i, FRAMLimitedSize);
      Fw::Buffer inBuffer(internalWriteBuffer.data(), internalWriteBuffer.size());

      if (this->write_out(0, inAddr, inBuffer) == Drv::FRAM::InStatus::FAILED) {
        errorReport(ErrorCode::WriteFailed);
        this->status_out(0, Drv::FRAM::ExtStatus::WRITE_FAILED);
        return;
      }
      inAddr += FRAMLimitedSize;
    }

    if (remainingBytes > 0) {
      // Write remaining chunk
      etl::span<U8> internalWriteBuffer(buffer.getData() + FRAMLimitedSize * fullParts, remainingBytes);
      Fw::Buffer inBuffer(internalWriteBuffer.data(), internalWriteBuffer.size());
      if (this->write_out(0, inAddr, inBuffer) == Drv::FRAM::InStatus::FAILED) {
        errorReport(ErrorCode::WriteFailed);
        this->status_out(0, Drv::FRAM::ExtStatus::WRITE_FAILED);
        return;
      }
    }

    // initiate status
    this->status_out(0, Drv::FRAM::ExtStatus::WRITE_SUCCESS);
  }

  void FRAMManager ::
    requestRead_handler(
        NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer& buffer
    )
  {
    // Analyze read buffer
    constexpr U32 FRAMLimitedSize = 256;
    U32 fullParts = buffer.getSize() / FRAMLimitedSize; // TODO: Instead of using of magic number make call to FRAMDriver to receive limitation value
    U32 remainingBytes = buffer.getSize() % FRAMLimitedSize; // TODO: The same as above

    // Read main chunks
    U32 inAddr = addr;
    for (U32 i = 0; i < fullParts; i++) {
      etl::span<U8> internalWriteBuffer(buffer.getData() + FRAMLimitedSize * i, FRAMLimitedSize);
      Fw::Buffer inBuffer(internalWriteBuffer.data(), internalWriteBuffer.size());

      if (this->read_out(0, inAddr, inBuffer) == Drv::FRAM::InStatus::FAILED) {
        errorReport(ErrorCode::ReadFailed);
        this->status_out(0, Drv::FRAM::ExtStatus::READ_FAILED);
        return;
      }
      inAddr += FRAMLimitedSize;
    }

    if (remainingBytes > 0) {
      // Read remaining chunk
      etl::span<U8> internalWriteBuffer(buffer.getData() + FRAMLimitedSize * fullParts, remainingBytes);
      Fw::Buffer inBuffer(internalWriteBuffer.data(), internalWriteBuffer.size());
      if (this->read_out(0, inAddr, inBuffer) == Drv::FRAM::InStatus::FAILED) {
        errorReport(ErrorCode::ReadFailed);
        this->status_out(0, Drv::FRAM::ExtStatus::READ_FAILED);
        return;
      }
    }
    this->status_out(0, Drv::FRAM::ExtStatus::READ_SUCCESS);
  }

  void FRAMManager ::
    requestSleep_handler(NATIVE_INT_TYPE portNum)
  {
    if (this->sleep_out(0) == Drv::FRAM::InStatus::FAILED) {
      errorReport(ErrorCode::SleepFailed);
      this->status_out(0, Drv::FRAM::ExtStatus::SLEEP_FAILED);
      return;
    }

    this->status_out(0, Drv::FRAM::ExtStatus::SLEEP_SUCCESS);
  }

  void FRAMManager ::
    requestWakeup_handler(NATIVE_INT_TYPE portNum)
  {
    if (this->wakeup_out(0) == Drv::FRAM::InStatus::FAILED) {
      errorReport(ErrorCode::WakeupFailed);
      this->status_out(0, Drv::FRAM::ExtStatus::WAKEUP_FAILED);
      return;
    }
    this->status_out(0, Drv::FRAM::ExtStatus::WAKEUP_SUCCESS);
  }

  void FRAMManager::errorReport(ErrorCode error) {
    switch (error) {
        case ErrorCode::WriteFailed:
            this->log_WARNING_HI_WriteFailed();
        break;
        case ErrorCode::ReadFailed:
            this->log_WARNING_HI_ReadFailed();
        break;
        case ErrorCode::WakeupFailed:
            this->log_WARNING_HI_WakeupFailed();
        break;
        case ErrorCode::SleepFailed:
            this->log_WARNING_HI_SleepFailed();
        break;
        default:
        break;
    }
  }


  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void FRAMManager ::
    REINIT_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    this->reinit_out(0);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }
}
