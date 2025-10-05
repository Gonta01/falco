// ======================================================================
// \title  NORTester.cpp
// \author user
// \brief  cpp file for NORTester component implementation class
// ======================================================================

#include "Components/NORTester/NORTester.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  NORTester ::
    NORTester(const char* const compName) :
      NORTesterComponentBase(compName)
  {

  }

  NORTester ::
    ~NORTester()
  {

  }

  void NORTester :: configure() {
    FLASHSize  = this->getMemorySize_out(0);
    sectorSize = this->getSectorSize_out(0);

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void NORTester::getEraseStatus_handler(NATIVE_INT_TYPE portNum, const Drv::NORManager::Status& status) {
    eraseStatus = status;
  }

  void NORTester::getReadStatus_handler(NATIVE_INT_TYPE portNum, const Drv::NORManager::Status& status) {
    readStatus = status;
  }

  void NORTester::getWriteStatus_handler(NATIVE_INT_TYPE portNum, const Drv::NORManager::Status& status) {
    writeStatus = status;
  }

  void NORTester::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    switch (sm) {
      case StateMachine::INIT: {
        // report test started
        this->log_ACTIVITY_HI_TestStarted(); 

        sm = StateMachine::ERASE_REQUEST;
        break;
      }

      case StateMachine::ERASE_REQUEST: {
        auto eraseSize = sectorsToErase*sectorSize;
        this->requestErase_out(0, blcAddr, eraseSize);
        eraseStatus = Drv::NORManager::Status::IN_PROCESS;
        sm = StateMachine::ERASE_IN_PROCESS;
        break;
      }

      case StateMachine::ERASE_IN_PROCESS: {
        if (eraseStatus == Drv::NORManager::Status::SUCCESS) {
          sm = StateMachine::WRITE_REQUEST;
        }
        else if (eraseStatus == Drv::NORManager::Status::FAILED) {
          sm = StateMachine::FINISH;
          // report erase error
          this->log_WARNING_HI_EraseError(); 
        }

        break;
      }

      case StateMachine::WRITE_REQUEST: {
        for (uint32_t iter = 0U; iter < writeArr.size(); iter++) {
          writeArr[iter] = memAddr + 0x80000000;
          memAddr += 1;
        }
        Fw::Buffer writeBuffer(writeArr.data(), writeArr.size());
        this->requestWrite_out(0, blcAddr, writeBuffer);
        
        writeStatus = Drv::NORManager::Status::IN_PROCESS;
        sm = StateMachine::WRITE_IN_PROCESS;
        break;
      }


      case StateMachine::WRITE_IN_PROCESS: {
        if (writeStatus == Drv::NORManager::Status::SUCCESS) {
          sm = StateMachine::READ_REQUEST;
        }
        else if (writeStatus == Drv::NORManager::Status::FAILED) {
          sm = StateMachine::FINISH;
          // report write error
          this->log_WARNING_HI_WriteError(); 
        }

        break;
      }

      case StateMachine::READ_REQUEST: {
        Fw::Buffer readBuffer(readArr.data(), readArr.size());
        this->requestRead_out(0, blcAddr, readBuffer);
        readStatus = Drv::NORManager::Status::IN_PROCESS;
        sm = StateMachine::READ_IN_PROCESS;
        break;
      }

      case StateMachine::READ_IN_PROCESS: {
        if (readStatus == Drv::NORManager::Status::SUCCESS) {
          sm = StateMachine::CHECK_BUFFER_EQUALITY;
        }
        else if (readStatus == Drv::NORManager::Status::FAILED) {
          sm = StateMachine::FINISH;
          // report read error
          this->log_WARNING_HI_ReadError();
        }

        break;
      }
      
      case StateMachine::CHECK_BUFFER_EQUALITY: {
        if (etl::equal(writeArr.begin(), writeArr.end(), readArr.begin())) {
          blcAddr = memAddr;
          if (blcAddr < startAddress + FLASHSize) { 
            sm = StateMachine::ERASE_REQUEST;
          }
          else {
            sm = StateMachine::FINISH;
          }
        } else {
          // report test failure, buffer not equal
          this->log_WARNING_HI_BuffersNotEqual();
          sm = StateMachine::FINISH;
        }
        break;
      }

      case StateMachine::FINISH: {
        blcAddr = 0;
        memAddr = 0;
        // report test finished event
        this->log_ACTIVITY_HI_TestFinished(); 

        sm = StateMachine::INIT;
      }

      default:
      break;

    }
  }
}
