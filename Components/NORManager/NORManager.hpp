// ======================================================================
// \title  NORManager.hpp
// \author user
// \brief  hpp file for NORManager component implementation class
// ======================================================================

#ifndef Components_NORManager_HPP
#define Components_NORManager_HPP

#include "Components/NORManager/NORManagerComponentAc.hpp"

namespace Components {

  class NORManager :
    public NORManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct NORManager object
      NORManager(
          const char* const compName //!< The component name
      );

      //! Destroy NORManager object
      ~NORManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getOutMemorySize
      U32 getOutMemorySize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for getOutSectorSize
      U32 getOutSectorSize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for requestErase
      void requestErase_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        U32 length
      ) override;

      //! Handler implementation for requestRead
      void requestRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        Fw::Buffer& readBuff
      ) override;

      //! Handler implementation for requestWrite
      void requestWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        Fw::Buffer& writeBuff
      ) override;
  };

}

#endif
