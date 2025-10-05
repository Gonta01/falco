// ======================================================================
// \title  ExternalI2cDevice.hpp
// \author user
// \brief  hpp file for ExternalI2cDevice component implementation class
// ======================================================================

#ifndef Components_ExternalI2cDevice_HPP
#define Components_ExternalI2cDevice_HPP

#include "Components/ExternalI2cDevice/ExternalI2cDeviceComponentAc.hpp"

namespace Components {

  class ExternalI2cDevice :
    public ExternalI2cDeviceComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct ExternalI2cDevice object
      ExternalI2cDevice(
          const char* const compName //!< The component name
      );

      //! Destroy ExternalI2cDevice object
      ~ExternalI2cDevice();

      //! Emit parameter updated EVR
      //!
      void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

    PRIVATE:
      static constexpr U32 writeTimeout = 0xFFFF'FFFF;
      static constexpr U32 readTimeout  = 0xFFFF'FFFF;

      bool oneShotWriteEnable;
      bool oneShotReadEnable;
      U8 regValue;
      U8 writeData;
      U8 readData;

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command WRITE
      //!
      //! Command to write data through I2C port
      void WRITE_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U8 regValue, //!< Device register to write in
          U8 writeData //!< Value that should be written to register
      ) override;

      //! Handler implementation for command READ
      //!
      //! Command to read data through I2C port
      void READ_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U8 regValue //!< Device register to read from
      ) override;

  };

}

#endif
