// ======================================================================
// \title  FileTester.hpp
// \author user
// \brief  hpp file for FileTester component implementation class
// ======================================================================

#ifndef Components_FileTester_HPP
#define Components_FileTester_HPP

#include "Components/FileTester/FileTesterComponentAc.hpp"
#include <Os/File.hpp>
#include <Os/Directory.hpp>
#include <Os/FileSystem.hpp>
#include <etl/string.h>

namespace Components {

  class FileTester :
    public FileTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FileTester object
      FileTester(
          const char* const compName //!< The component name
      );

      //! Destroy FileTester object
      ~FileTester();

      /**
       * Initial testing configurations
       */
      void configure();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      bool  isFailHappened{false};
      static constexpr U32 maxStringSize = 64;
      etl::string<maxStringSize> string;

      static constexpr U8 firstTesting  = 1; 
      static constexpr U8 secondTesting = 2; 
      static constexpr U8 thirdTesting  = 3; 
      U8 testingMode = thirdTesting; 

      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      bool conductTesting(Os::File &file, const char* fileName);
      bool conductTesting2(Os::File &file1, const char* fileName1, Os::File &file2, const char* fileName2);
      bool conductTesting3(Os::File &file1, const char* fileName1, Os::File &file2, const char* fileName2, Os::File &file3, const char* fileName3);
      bool testSeek(Os::File &file, const char* fileName);
      bool testDirectory();
      bool readThroughDirectory(Os::Directory &dir, U32 numOfReads);
      bool testAppendMode();
      bool testGetFileCount();
      bool testRemoveDirectory();
      bool testRemoveFile();
      bool testMoveFile();
      bool testCopyFile();

  };

}

#endif
