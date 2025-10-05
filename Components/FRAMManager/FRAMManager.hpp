// ======================================================================
// \title  FRAMManager.hpp
// \author user
// \brief  hpp file for FRAMManager component implementation class
// ======================================================================

#ifndef Components_FRAMManager_HPP
#define Components_FRAMManager_HPP

#include "Components/FRAMManager/FRAMManagerComponentAc.hpp"

namespace Components {

  class FRAMManager :
    public FRAMManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FRAMManager object
      FRAMManager(
          const char* const compName //!< The component name
      );

      //! Destroy FRAMManager object
      ~FRAMManager();

    PRIVATE:
      enum class ErrorCode : uint8_t {
        NoError = 0,
        WriteFailed,
        ReadFailed,
        WakeupFailed,
        SleepFailed
      };
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      
      //! Handler implementation for requestWrite
      void requestWrite_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr,
          Fw::Buffer& buffer
      ) override;

      //! Handler implementation for requestRead
      void requestRead_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr,
          Fw::Buffer& buffer
      ) override;

      //! Handler implementation for requestSleep
      void requestSleep_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for requestWakeup
      void requestWakeup_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;
      
      /**
       * Function to handle driver error and create event reports
       */
      void errorReport(ErrorCode error);

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command REINIT
      //!
      //! Command to reinit FRAM driver and peripheral it relies on (SPI)
      void REINIT_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;
  };

}

#endif
