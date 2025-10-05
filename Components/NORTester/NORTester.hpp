// ======================================================================
// \title  NORTester.hpp
// \author user
// \brief  hpp file for NORTester component implementation class
// ======================================================================

#ifndef Components_NORTester_HPP
#define Components_NORTester_HPP

#include "Components/NORTester/NORTesterComponentAc.hpp"
#include "etl/array.h"

namespace Components {

  class NORTester :
    public NORTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct NORTester object
      NORTester(
          const char* const compName //!< The component name
      );

      //! Destroy NORTester object
      ~NORTester();

      void configure();

    PRIVATE:
      enum class StateMachine {
        INIT,
        ERASE_REQUEST,
        ERASE_IN_PROCESS,
        WRITE_REQUEST,
        WRITE_IN_PROCESS,
        READ_REQUEST,
        READ_IN_PROCESS,
        CHECK_BUFFER_EQUALITY,
        FINISH
      };

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      
      //! Handler implementation for getEraseStatus
      void getEraseStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status& status
      ) override;

      //! Handler implementation for getReadStatus
      void getReadStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status& status
      ) override;

      //! Handler implementation for getWriteStatus
      void getWriteStatus_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::NORManager::Status& status
      ) override;

      //! Handler implementation for run
      void run_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

      static constexpr uint32_t readArraySize  = 8192;
      static constexpr uint32_t writeArraySize = 8192;
      static constexpr uint32_t sectorsToErase = 2;   // sectorsToErase*sectorSize bytes will be erased
  
      etl::array<U8, readArraySize> readArr{0U};
      etl::array<U8, writeArraySize> writeArr{0U};
  
      uint32_t FLASHSize  = 0;
      uint32_t sectorSize = 0;
      static constexpr uint32_t startAddress = 0x0;
  
      uint32_t memAddr = startAddress;
      uint32_t blcAddr = startAddress;

      bool error = false;
      bool testFinished = false;
      
      StateMachine sm;

      Drv::NORManager::Status eraseStatus;
      Drv::NORManager::Status writeStatus;
      Drv::NORManager::Status readStatus;
  };

}

#endif
