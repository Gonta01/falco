// ======================================================================
// \title  FRAMTester.hpp
// \author user
// \brief  hpp file for FRAMTester component implementation class
// ======================================================================

#ifndef Components_FRAMTester_HPP
#define Components_FRAMTester_HPP

#include "Components/FRAMTester/FRAMTesterComponentAc.hpp"
#include "etl/array.h"

namespace Components {

  class FRAMTester :
    public FRAMTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FRAMTester object
      FRAMTester(
          const char* const compName //!< The component name
      );

      //! Destroy FRAMTester object
      ~FRAMTester();

    PRIVATE:
      enum class FRAMTesterSM{
        INIT_TRANSITION,
        INIT_PROCESS,
        WRITE_READ_TESTING_PROCESS,
        TEST_WRITE_TRANSITION,
        TEST_WRITE_PROCESS,
        TEST_READ_TRANSITION,
        TEST_READ_PROCESS,
        CHECK_BUFFS_PROCESS,
        TEST_SLEEP_TRANSITION,
        TEST_SLEEP_PROCESS,
        SLEEP_TRANSITION,
        SLEEP_PROCESS,
        SAFE_TRANSITION,
        SAFE_PROCESS
      };

      enum class ErrorCode : uint8_t {
        NoError = 0,
        BuffersNotEqual,
      };

      FRAMTesterSM stateMachine = FRAMTesterSM::INIT_TRANSITION;
      Drv::FRAM::ExtStatus status;
      bool startTesting;
      bool timeToWakeup;
      bool resetTesting;
      bool endTesting;

      Fw::Time initStateStartTime;
      static constexpr U32 timeInInitStateS = 5;
      Fw::Time safeStateStartTime;
      static constexpr U32 timeInSafeStateS = 5;
      Fw::Time sleepStateStartTime;
      static constexpr U32 timeInSleepStateS = 5;

      static constexpr uint32_t arraySize = 1024;
      etl::array<uint8_t, arraySize> writeArr{0U};
      etl::array<uint8_t, arraySize> readArr{0U};
      Fw::Buffer writeBuff{writeArr.data(), writeArr.size()};
      Fw::Buffer readBuff{readArr.data(), readArr.size()};
      
      static constexpr uint32_t FRAMSize = 512U * 1024U;
      static constexpr uint8_t fourthByteOffset = 24U;
      static constexpr uint8_t thirdByteOffset = 16U;
      static constexpr uint8_t secondByteOffset = 8U;
      static constexpr uint8_t firstByteMask = 0xFFU;

      uint32_t memAddr = 0U;
      uint32_t blcAddr = 0U;
      /**
       * Function to handle driver error and create event reports
       */
      void errorReport(ErrorCode error);

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      //! Handler implementation for status
      void status_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Drv::FRAM::ExtStatus& status
      ) override;
  };

}

#endif
