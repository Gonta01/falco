// ======================================================================
// \title  FSPerformanceTester.hpp
// \author user
// \brief  hpp file for FSPerformanceTester component implementation class
// ======================================================================

#ifndef Components_FSPerformanceTester_HPP
#define Components_FSPerformanceTester_HPP

#include "Components/FSPerformanceTester/FSPerformanceTesterComponentAc.hpp"
#include <Os/File.hpp>
#include <Os/Directory.hpp>
#include <Os/FileSystem.hpp>
#include "etl/string.h"

namespace Components {

  class FSPerformanceTester :
    public FSPerformanceTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FSPerformanceTester object
      FSPerformanceTester(
          const char* const compName //!< The component name
      );

      //! Destroy FSPerformanceTester object
      ~FSPerformanceTester();

      void configure();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      
      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      Os::File metrics;
      Os::File testFile;
      
      static constexpr U32 maxFileNameSize  = 256;
      etl::string<maxFileNameSize> fileName = "testFile.txt";
      static constexpr U32 sizeOfLogMessage = 256;
      etl::string<sizeOfLogMessage> logMessage;
      
      U32 numOfFiles = 0;
      static constexpr size_t textMaxSize = 60;

      void getCalendarTimeInS(Fw::Time& currentTimeS);
      
      void createFileName();
      U32 testOpen();
      U32 testWrite();
      U32 testRead();
      
  };

}

#endif
