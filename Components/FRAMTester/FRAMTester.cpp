// ======================================================================
// \title  FRAMTester.cpp
// \author user
// \brief  cpp file for FRAMTester component implementation class
// ======================================================================

#include "Components/FRAMTester/FRAMTester.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FRAMTester ::
    FRAMTester(const char* const compName) :
      FRAMTesterComponentBase(compName),
      startTesting(false),
      timeToWakeup(false),
      resetTesting(false),
      endTesting(false)
  {
  }

  FRAMTester ::
    ~FRAMTester()
  {

  }

  void FRAMTester::errorReport(ErrorCode error) {
    switch (error) {
        case ErrorCode::BuffersNotEqual:
            this->log_WARNING_HI_BuffersNotEqual();
        break;
        default:
        break;
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------
  void FRAMTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    switch (stateMachine) {
      case FRAMTesterSM::INIT_TRANSITION:
      {
        blcAddr = 0;
        memAddr = 0;
        initStateStartTime = this->getTime();
        stateMachine = FRAMTesterSM::INIT_PROCESS;
        status =  Drv::FRAM::ExtStatus::UNDEFINED;
        timeToWakeup = false;
        this->log_ACTIVITY_HI_InitSM();
        break;
      }

      case FRAMTesterSM::INIT_PROCESS:
      {
        Fw::Time now = this->getTime();
        Fw::Time target = Fw::Time::sub(now, initStateStartTime);
        Fw::Time timeInInitState(timeInInitStateS, 0);
        
        if (target >= timeInInitState) {
          startTesting = true;
        }

        if (startTesting) {
          stateMachine = FRAMTesterSM::WRITE_READ_TESTING_PROCESS;
          startTesting = false;
        }
        break;
      }

      case FRAMTesterSM::WRITE_READ_TESTING_PROCESS:
      {
        // fill writeArr
        for (uint32_t iter = 0U; iter < writeArr.size() / 4; iter++) {
          memAddr += 4;
          writeArr[iter * 4] = memAddr >> fourthByteOffset;
          writeArr[iter * 4 + 1] = memAddr >> thirdByteOffset;
          writeArr[iter * 4 + 2] = memAddr >> secondByteOffset;
          writeArr[iter * 4 + 3] = memAddr & firstByteMask;
        }
        // for (uint32_t iter = 0U; iter < writeArr.size(); iter++) {
        //   memAddr += 1;
        //   writeArr[iter] = memAddr & firstByteMask;
        // }
        
        if (blcAddr >= FRAMSize) {
          endTesting = true;
        }
        if (endTesting) {
          endTesting = false;
          stateMachine = FRAMTesterSM::TEST_SLEEP_TRANSITION;
          this->log_ACTIVITY_HI_FRAMTestFinished();
        }
        else {
          stateMachine = FRAMTesterSM::TEST_WRITE_TRANSITION;
        }
        break;
      }
      
      case FRAMTesterSM::TEST_WRITE_TRANSITION: {
        this->requestWrite_out(0, blcAddr, writeBuff);
        stateMachine = FRAMTesterSM::TEST_WRITE_PROCESS;
        break;
      }
      
      case FRAMTesterSM::TEST_WRITE_PROCESS:
      {
        if (status == Drv::FRAM::ExtStatus::WRITE_SUCCESS) {
          stateMachine = FRAMTesterSM::TEST_READ_TRANSITION;
        }
        if (status == Drv::FRAM::ExtStatus::WRITE_FAILED) {
          stateMachine = FRAMTesterSM::SAFE_TRANSITION;
        }
        break;
      }

      case FRAMTesterSM::TEST_READ_TRANSITION: {
        this->requestRead_out(0, blcAddr, readBuff);
        stateMachine = FRAMTesterSM::TEST_READ_PROCESS;
        break;
      }

      case FRAMTesterSM::TEST_READ_PROCESS: 
      {
        if (status == Drv::FRAM::ExtStatus::READ_SUCCESS) {
          stateMachine = FRAMTesterSM::CHECK_BUFFS_PROCESS;
        }
        if (status == Drv::FRAM::ExtStatus::READ_FAILED) {
          stateMachine = FRAMTesterSM::SAFE_TRANSITION;
        }
        break;  
      }
      case FRAMTesterSM::CHECK_BUFFS_PROCESS:
      {
        // set address where to write data
        if (etl::equal(writeArr.begin(), writeArr.end(), readArr.begin())) {
          // this->log_ACTIVITY_LO_TestedChunk(blcAddr, writeArr.size());
          blcAddr = memAddr;
          stateMachine = FRAMTesterSM::WRITE_READ_TESTING_PROCESS;
        }
        else {
          errorReport(ErrorCode::BuffersNotEqual);
          stateMachine = FRAMTesterSM::SAFE_TRANSITION;
        }
        break;
      }

      case FRAMTesterSM::TEST_SLEEP_TRANSITION:
      {
        this->requestSleep_out(0);
        stateMachine = FRAMTesterSM::TEST_SLEEP_PROCESS;
        break;
      }

      case FRAMTesterSM::TEST_SLEEP_PROCESS:
      {
        if (status == Drv::FRAM::ExtStatus::SLEEP_SUCCESS) {
          stateMachine = FRAMTesterSM::SLEEP_TRANSITION;
        }
        if (status == Drv::FRAM::ExtStatus::SLEEP_FAILED) {
          stateMachine = FRAMTesterSM::SAFE_TRANSITION;
        }
        break;
      }

      case FRAMTesterSM::SLEEP_TRANSITION:
      {
        sleepStateStartTime = this->getTime();
        this->log_ACTIVITY_HI_SleepSM();
        stateMachine = FRAMTesterSM::SLEEP_PROCESS;
        break; 
      }

      case FRAMTesterSM::SLEEP_PROCESS:
      {
        if (timeToWakeup == false) {
          Fw::Time now = this->getTime();
          Fw::Time target = Fw::Time::sub(now, sleepStateStartTime);
          Fw::Time timeInSleepState(timeInSleepStateS, 0);

          if (target >= timeInSleepState) {
            timeToWakeup = true;
            this->requestWakeup_out(0);
          }
        }

        if (status == Drv::FRAM::ExtStatus::WAKEUP_SUCCESS) {
          stateMachine = FRAMTesterSM::INIT_TRANSITION;
        }
        if (status == Drv::FRAM::ExtStatus::WAKEUP_FAILED) {
          stateMachine = FRAMTesterSM::SAFE_TRANSITION;
        }
        break; 
      }

      case FRAMTesterSM::SAFE_TRANSITION:
      {
        safeStateStartTime = this->getTime();
        stateMachine = FRAMTesterSM::SAFE_PROCESS;
        this->log_ACTIVITY_HI_SafeSM();
        break;
      }

      case FRAMTesterSM::SAFE_PROCESS: 
      {
        Fw::Time now = this->getTime();
        Fw::Time target = Fw::Time::sub(now, safeStateStartTime);
        Fw::Time safeInInitState(timeInSafeStateS, 0);
        
        if (target >= safeInInitState) {
          resetTesting = true;
        }

        if (resetTesting) {
          stateMachine = FRAMTesterSM::INIT_TRANSITION;
          resetTesting = false;
        }
        
        break;
      }

      default:
      {
        stateMachine = FRAMTesterSM::INIT_TRANSITION;
        break;
      }
        
    }
  }

  void FRAMTester ::
    status_handler(
        NATIVE_INT_TYPE portNum,
        const Drv::FRAM::ExtStatus& status
    )
  {
    this->status = status;
  }
}
