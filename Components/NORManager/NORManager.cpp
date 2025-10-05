// ======================================================================
// \title  NORManager.cpp
// \author user
// \brief  cpp file for NORManager component implementation class
// ======================================================================

#include "Components/NORManager/NORManager.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"
#include "etl/span.h"
#include "etl/algorithm.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  NORManager ::
    NORManager(const char* const compName) :
      NORManagerComponentBase(compName)
  {

  }

  NORManager ::
    ~NORManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  U32 NORManager::getOutMemorySize_handler(NATIVE_INT_TYPE portNum) {
    return this->getInMemorySize_out(portNum);
  }

  U32 NORManager::getOutSectorSize_handler(NATIVE_INT_TYPE portNum) {
    return this->getInSectorSize_out(portNum);
  }

  void NORManager::requestErase_handler(NATIVE_INT_TYPE portNum, U32 startAddress, U32 length) {
    Drv::NOR::ErrorCode errorCode = this->erase_out(0, startAddress, length);
    if (errorCode != Drv::NOR::ErrorCode::NoError) {
      this->eraseStatus_out(0, Drv::NORManager::Status::FAILED);
      return;
    }
    this->eraseStatus_out(0, Drv::NORManager::Status::SUCCESS);
  }
  
  void NORManager::requestRead_handler(NATIVE_INT_TYPE portNum, U32 startAddress, Fw::Buffer& readBuff) {
    Drv::NOR::ErrorCode errorCode = this->read_out(0, startAddress, readBuff);
    if (errorCode != Drv::NOR::ErrorCode::NoError) {
      this->readStatus_out(0, Drv::NORManager::Status::FAILED);
      return;
    }
    this->readStatus_out(0, Drv::NORManager::Status::SUCCESS);
  }

  void NORManager::requestWrite_handler(NATIVE_INT_TYPE portNum, U32 startAddress, Fw::Buffer& writeBuff) {
    Drv::NOR::ErrorCode errorCode = this->write_out(0, startAddress, writeBuff);
    if (errorCode != Drv::NOR::ErrorCode::NoError) {
      this->writeStatus_out(0, Drv::NORManager::Status::FAILED);
      return;
    }

    this->writeStatus_out(0, Drv::NORManager::Status::SUCCESS);
  }
}
