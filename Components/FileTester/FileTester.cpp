// ======================================================================
// \title  FileTester.cpp
// \author user
// \brief  cpp file for FileTester component implementation class
// ======================================================================

#include "Components/FileTester/FileTester.hpp"
#include "FpConfig.hpp"
#include "Falco/Os/FileSystemAddon.hpp"
#include "Os/FileSystem.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FileTester ::
    FileTester(const char* const compName) :
      FileTesterComponentBase(compName)
  {

  }

  FileTester ::
    ~FileTester()
  {

  }

  void FileTester::configure() {
    auto initResult = Os::FileSystem::initFileSystem();
    if (initResult == Os::FileSystem::OP_OK) {
      this->log_ACTIVITY_HI_InitSuccess();
    } 
    else {
      this->log_WARNING_HI_TestFail();
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void FileTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    isFailHappened = false;
    if (testingMode == firstTesting) {
      Os::Directory dir;

      auto dirStatus = dir.open("folder1/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        auto fsStatus = Os::FileSystem::createDirectory("folder1/");
        if (fsStatus != Os::FileSystem::OP_OK) {
          isFailHappened = true;
        }
        dirStatus = dir.open("folder1/");
        if (dirStatus != Os::Directory::Status::OP_OK) {
          isFailHappened = true;
        }
      }

      Os::File fileTesting0;
      if (conductTesting(fileTesting0, "File00.txt") == false) {
        isFailHappened = true;
      } 
      if (conductTesting(fileTesting0, "File01.txt") == false) {
        isFailHappened = true;
      }

      FwSizeType fileSize;
      auto status = Os::FileSystem::getFileSize("File00.txt", fileSize);
      if (status != Os::FileSystem::OP_OK) {
        isFailHappened = true;
      }

      Os::File fileTesting1;
      if (conductTesting(fileTesting1, "File10.txt") == false) {
        isFailHappened = true;
      } 
      if (conductTesting(fileTesting1, "File11.txt") == false) {
        isFailHappened = true;
      }

      Os::File fileTesting2;
      if (conductTesting(fileTesting2, "File20.txt") == false) {
        isFailHappened = true;
      } 
      if (conductTesting(fileTesting2, "File21.txt") == false) {
        isFailHappened = true;
      }
    }
    else if (testingMode == secondTesting) {
      Os::File fileTesting1;
      Os::File fileTesting2;
      Os::File fileTesting3;
  
      if (conductTesting2(fileTesting1, "folder1/fileTesting21.txt", fileTesting2, "folder1/fileTesting22.txt") == false) {
        isFailHappened = true;
      } 
  
      if (conductTesting3(fileTesting1, "folder1/fileTesting31.txt", fileTesting2, "folder1/fileTesting32.txt", fileTesting3, "folder1/fileTesting33.txt") == false) {
        isFailHappened = true;
      } 
      
      FwSizeType totalBytes = 0;
      FwSizeType freeBytes  = 0;
  
      auto status = Os::FileSystem::getFreeSpace("", totalBytes, freeBytes);
      if (status != Os::FileSystem::OP_OK) {
        isFailHappened = true;
      }
      
      if (testSeek(fileTesting1, "FileSeek01.txt") == false) {
        isFailHappened = true;
      }
  
      if (testDirectory() == false) {
        isFailHappened = true;
      }
  
      if (testAppendMode() == false) {
        isFailHappened = true;
      }
  
      if (testGetFileCount() == false) {
        isFailHappened = true;
      }
      
      if (testRemoveDirectory() == false) {
        isFailHappened = true;
      }
      
      if (testRemoveFile() == false) {
        isFailHappened = true;
      }
    }
    else if (testingMode == thirdTesting) {
      if (testMoveFile() == false) {
        isFailHappened = true;
      }
      
      if (testCopyFile() == false) {
        isFailHappened = true;
      }
    }
    if (testingMode == thirdTesting) {
      testingMode = firstTesting;
    }
    else if (testingMode == secondTesting) {
      testingMode = thirdTesting;
    }
    else {
      testingMode = secondTesting;
    }

    if (isFailHappened) {
      this->log_WARNING_HI_TestFail();
    }

  }

  bool FileTester::conductTesting(Os::File &file, const char* fileName) {
    auto result = file.open(fileName, Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 60;
    etl::string<textMaxSize> testText = "Testing hello world! Testing hello world!";

    if (result == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      result = file.write(testText.data(), testTextSize, false);
      file.close();

      if (result == Os::File::Status::OP_OK) {
        result = file.open(fileName, Os::File::Mode::OPEN_READ);
        if (result == Os::File::Status::OP_OK) {
          etl::string<textMaxSize> readText;
          readText.initialize_free_space();
          NATIVE_INT_TYPE readTextCapacity = readText.capacity();
          result = file.read(readText.data(), readTextCapacity);
          file.close();
          readText.trim_to_terminator();
          if (result == Os::File::Status::OP_OK) {
            if (readText.compare(testText) == 0) {
              // test success
                return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool FileTester::conductTesting2(Os::File &file1, const char* fileName1, Os::File &file2, const char* fileName2) {
    auto result1 = file1.open(fileName1, Os::File::Mode::OPEN_WRITE);
    auto result2 = file2.open(fileName2, Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 30;
    etl::string<textMaxSize> testText1 = "Testing 1st file opened!";
    etl::string<textMaxSize> testText2 = "Testing 2nd file opened!";

    if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK)) {
      NATIVE_INT_TYPE testTextSize = testText1.size();
      result1 = file1.write(testText1.data(), testTextSize, false);
      result2 = file2.write(testText2.data(), testTextSize, false);
      file1.close();
      file2.close();
      
      if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK)) {
        result2 = file2.open(fileName2, Os::File::Mode::OPEN_READ);
        result1 = file1.open(fileName1, Os::File::Mode::OPEN_READ);

        if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK)) {
          etl::string<textMaxSize> readText1;
          etl::string<textMaxSize> readText2;
          readText1.initialize_free_space();
          readText2.initialize_free_space();
          NATIVE_INT_TYPE readTextCapacity1 = readText1.capacity();
          NATIVE_INT_TYPE readTextCapacity2 = readText2.capacity();
          result1 = file1.read(readText1.data(), readTextCapacity1);
          result2 = file2.read(readText2.data(), readTextCapacity2);
          file1.close();
          file2.close();
          readText1.trim_to_terminator();
          readText2.trim_to_terminator();
          if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK)) {
            if ((readText1.compare(testText1) == 0) && (readText2.compare(testText2) == 0)) {
              // test success
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool FileTester::conductTesting3(Os::File &file1, const char* fileName1, Os::File &file2, const char* fileName2, Os::File &file3, const char* fileName3) {
    auto result1 = file1.open(fileName1, Os::File::Mode::OPEN_WRITE);
    auto result2 = file2.open(fileName2, Os::File::Mode::OPEN_WRITE);
    auto result3 = file3.open(fileName3, Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 30;
    etl::string<textMaxSize> testText1 = "Testing 1st file opened!";
    etl::string<textMaxSize> testText2 = "Testing 2nd file opened!";
    etl::string<textMaxSize> testText3 = "Testing 3rd file opened!";

    if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK) && (result3 == Os::File::Status::OP_OK)) {
      NATIVE_INT_TYPE testTextSize = testText1.size();
      result1 = file1.write(testText1.data(), testTextSize, false);
      result2 = file2.write(testText2.data(), testTextSize, false);
      result3 = file3.write(testText3.data(), testTextSize, false);

      file1.close();
      file2.close();
      file3.close();
      
      if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK) && (result3 == Os::File::Status::OP_OK)) {
        result3 = file3.open(fileName3, Os::File::Mode::OPEN_READ);
        result2 = file2.open(fileName2, Os::File::Mode::OPEN_READ);
        result1 = file1.open(fileName1, Os::File::Mode::OPEN_READ);

        if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK) && (result3 == Os::File::Status::OP_OK)) {
          etl::string<textMaxSize> readText1;
          etl::string<textMaxSize> readText2;
          etl::string<textMaxSize> readText3;

          readText1.initialize_free_space();
          readText2.initialize_free_space();
          readText3.initialize_free_space();

          NATIVE_INT_TYPE readTextCapacity1 = readText1.capacity();
          NATIVE_INT_TYPE readTextCapacity2 = readText2.capacity();
          NATIVE_INT_TYPE readTextCapacity3 = readText3.capacity();

          result1 = file1.read(readText1.data(), readTextCapacity1);
          result2 = file2.read(readText2.data(), readTextCapacity2);
          result3 = file3.read(readText3.data(), readTextCapacity3);

          file1.close();
          file2.close();
          file3.close();

          readText1.trim_to_terminator();
          readText2.trim_to_terminator();
          readText3.trim_to_terminator();

          if ((result1 == Os::File::Status::OP_OK) && (result2 == Os::File::Status::OP_OK) && (result3 == Os::File::Status::OP_OK)) {
            if ((readText1.compare(testText1) == 0) && (readText2.compare(testText2) == 0) && (readText3.compare(testText3) == 0)) {
              // test success
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool FileTester::testSeek(Os::File &file, const char* fileName) {
    auto result = file.open(fileName, Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 40;
    etl::string<textMaxSize> testText = "Let's: test seek";
    etl::string<textMaxSize> insertText = "Here inserted texT! ";
    etl::string<textMaxSize> expectedText = "Let's: Here inserted texT! ";

    if (result == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      result = file.write(testText.data(), testTextSize, false);
      if (result == Os::File::Status::OP_OK) {
        result = file.seek(7, false);
        NATIVE_INT_TYPE insertTextSize = insertText.size();
        result = file.write(insertText.data(), insertTextSize, false);
      }
      file.close();

      if (result == Os::File::Status::OP_OK) {
        result = file.open(fileName, Os::File::Mode::OPEN_READ);
        if (result == Os::File::Status::OP_OK) {
          etl::string<textMaxSize> readText;
          readText.initialize_free_space();
          NATIVE_INT_TYPE readTextCapacity = readText.capacity();
          result = file.read(readText.data(), readTextCapacity);
          file.close();
          readText.trim_to_terminator();
          if (result == Os::File::Status::OP_OK) {
            if (readText.compare(expectedText) == 0) {
              // test success
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool FileTester::readThroughDirectory(Os::Directory &dir, U32 numOfReads) {
    for (U32 i = 0; i < numOfReads; i++) {
      auto dirStatus = dir.read(string.data(), string.max_size());
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir.close();
        return false;
      }
    }
    return true;
  }

  bool FileTester::testDirectory() {
    Os::Directory dir0;
    Os::Directory dir1;

    auto dirStatus = dir0.open("directory0/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("directory0/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir0.close();
        return false;
      }
      dirStatus = dir0.open("directory0/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir0.close();
        return false;
      }
    }
    dir0.close();

    dirStatus = dir0.open("directory1/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("directory1/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir0.close();
        return false;
      }
      dirStatus = dir0.open("directory1/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir0.close();
        return false;
      }
    }

    dirStatus = dir1.open("directory2/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("directory2/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir1.close();
        return false;
      }
      dirStatus = dir1.open("directory2/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir1.close();
        return false;
      }
    }
    dir1.close();
    dir0.close();

    Os::File file0;
    Os::File file1;
    Os::File file2;
    Os::File file3;
    Os::File file4;

    auto fileStatus = file0.open("directory0/filed00.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    
    fileStatus = file1.open("directory1/filed10.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }

    fileStatus = file2.open("directory2/filed20.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }

    fileStatus = file3.open("directory0/filed01.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }

    fileStatus = file4.open("directory0/filed02.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }

    file0.close();
    file1.close();
    file2.close();
    file3.close();
    file4.close();

    fileStatus = file3.open("directory1/filed11.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file3.close();

    fileStatus = file3.open("directory1/filed12.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file3.close();

    fileStatus = file3.open("directory1/filed13.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file3.close();

    fileStatus = file3.open("directory1/filed14.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file3.close();

    fileStatus = file4.open("directory2/filed21.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed22.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed23.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed24.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed25.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed26.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    fileStatus = file4.open("directory2/filed27.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file4.close();

    constexpr U32 numOfElementsInDir0 = 3;

    dirStatus = dir0.open("directory0/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      dir0.close();
      return false;
    }

    bool isReadSuccess = readThroughDirectory(dir0, numOfElementsInDir0);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir0.read(string.data(), string.max_size());
    if (dirStatus == Os::Directory::Status::NO_MORE_FILES) {
      dirStatus = dir0.rewind();
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir0.close();
        return false;
      }
    }

    isReadSuccess = readThroughDirectory(dir0, numOfElementsInDir0);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir0.read(string.data(), string.max_size());
    if (dirStatus != Os::Directory::Status::NO_MORE_FILES) {
      return false;
    }

    constexpr U32 numOfElementsInDir1 = 5;
    dirStatus = dir1.open("directory1/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      dir1.close();
      return false;
    }

    isReadSuccess = readThroughDirectory(dir1, numOfElementsInDir1);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir1.read(string.data(), string.max_size());
    if (dirStatus == Os::Directory::Status::NO_MORE_FILES) {
      dirStatus = dir1.rewind();
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir1.close();
        return false;
      }
    }

    isReadSuccess = readThroughDirectory(dir1, numOfElementsInDir1);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir1.read(string.data(), string.max_size());
    if (dirStatus != Os::Directory::Status::NO_MORE_FILES) {
      return false;
    }

    Os::Directory dir2;
    constexpr U32 numOfElementsInDir2 = 8;
    dirStatus = dir2.open("directory2/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      dir2.close();
      return false;
    }

    isReadSuccess = readThroughDirectory(dir2, numOfElementsInDir2);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir2.read(string.data(), string.max_size());
    if (dirStatus == Os::Directory::Status::NO_MORE_FILES) {
      dirStatus = dir2.rewind();
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir2.close();
        return false;
      }
    }

    isReadSuccess = readThroughDirectory(dir2, numOfElementsInDir2);
    if (isReadSuccess == false) {
      return false;
    }

    dirStatus = dir2.read(string.data(), string.max_size());
    if (dirStatus != Os::Directory::Status::NO_MORE_FILES) {
      return false;
    }

    return true;
  }

  bool FileTester::testAppendMode() {
    Os::File file;
    auto result = file.open("TestAppend.txt", Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 80;
    etl::string<textMaxSize> testText = "Some testing text. Some testing text! ";

    bool isWriteReadSuccess = false;
    if (result == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      result = file.write(testText.data(), testTextSize, false);
      file.close();

      if (result == Os::File::Status::OP_OK) {
        result = file.open("TestAppend.txt", Os::File::Mode::OPEN_READ);
        if (result == Os::File::Status::OP_OK) {
          etl::string<textMaxSize> readText;
          readText.initialize_free_space();
          NATIVE_INT_TYPE readTextCapacity = readText.capacity();
          result = file.read(readText.data(), readTextCapacity);
          file.close();
          readText.trim_to_terminator();
          if (result == Os::File::Status::OP_OK) {
            if (readText.compare(testText) == 0) {
              // test success
              isWriteReadSuccess = true;
            }
          }
        }
      }
    }
    if (isWriteReadSuccess == false) {
      return false;
    }

    result = file.open("TestAppend.txt", Os::File::Mode::OPEN_APPEND);
    if (result != Os::File::Status::OP_OK) {
      return false;
    }
    etl::string<textMaxSize> appendedText = "Appended text. Appended text! ";
    NATIVE_INT_TYPE appendedTextSize = appendedText.size();
    result = file.write(appendedText.data(), appendedTextSize, false);
    if (result != Os::File::Status::OP_OK) {
      return false;
    }
    
    file.close();
    etl::string<textMaxSize> expextedText = "Some testing text. Some testing text! Appended text. Appended text! ";
    if (result == Os::File::Status::OP_OK) {
      result = file.open("TestAppend.txt", Os::File::Mode::OPEN_READ);
      if (result == Os::File::Status::OP_OK) {
        etl::string<textMaxSize> readText;
        readText.initialize_free_space();
        NATIVE_INT_TYPE readTextCapacity = readText.capacity();
        result = file.read(readText.data(), readTextCapacity);
        file.close();
        readText.trim_to_terminator();
        if (result == Os::File::Status::OP_OK) {
          if (readText.compare(expextedText) == 0) {
            // test success
            isWriteReadSuccess = true;
          }
        }
      }
    }

    return true;
  }

  bool FileTester::testGetFileCount() {
    U32 fileCount = 0;
    
    Os::Directory dir;
    auto dirStatus = dir.open("testDir/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("testDir/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir.close();
        return false;
      }
      dirStatus = dir.open("testDir/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir.close();
        return false;
      }
    }

    Os::File file;
    auto fileStatus = file.open("testDir/file00.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file.close();

    fileStatus = file.open("testDir/file01.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file.close();

    fileStatus = file.open("testDir/file02.txt", Os::File::Mode::OPEN_CREATE);
    if (fileStatus != Os::File::OP_OK) {
      return false;
    }
    file.close();

    constexpr U32 numOfCreatedFiles = 3;

    auto status = Os::FileSystem::getFileCount("testDir/", fileCount);
    if (status != Os::FileSystem::Status::OP_OK) {
      return false;
    }

    if (fileCount != numOfCreatedFiles) {
      return false;
    }

    return true;
  }

  bool FileTester::testRemoveDirectory() {
    Os::Directory dir;
    auto dirStatus = dir.open("removeDir/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("removeDir/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir.close();
        return false;
      }
      dirStatus = dir.open("removeDir/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir.close();
        return false;
      }
    }
    auto fsStatus = Os::FileSystem::removeDirectory("removeDir/");
    if (fsStatus != Os::FileSystem::OP_OK) {
      return false;
    }
    return true;
  }

  bool FileTester::testRemoveFile() {
    Os::File file;

    auto result = file.open("RemoveFile.txt", Os::File::Mode::OPEN_WRITE);
    
    constexpr size_t textMaxSize = 60;
    etl::string<textMaxSize> testText = "To be removed";

    bool isWriteReadSuccess = false;
    if (result == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      result = file.write(testText.data(), testTextSize, false);
      file.close();

      if (result == Os::File::Status::OP_OK) {
        result = file.open("RemoveFile.txt", Os::File::Mode::OPEN_READ);
        if (result == Os::File::Status::OP_OK) {
          etl::string<textMaxSize> readText;
          readText.initialize_free_space();
          NATIVE_INT_TYPE readTextCapacity = readText.capacity();
          result = file.read(readText.data(), readTextCapacity);
          file.close();
          readText.trim_to_terminator();
          if (result == Os::File::Status::OP_OK) {
            if (readText.compare(testText) == 0) {
              // test success
              isWriteReadSuccess = true;
            }
          }
        }
      }
    }

    if (isWriteReadSuccess == false) {
      return false;
    }

    auto fsStatus = Os::FileSystem::removeFile("RemoveFile.txt");
    if (fsStatus != Os::FileSystem::Status::OP_OK) {
      return false;
    }

    return true;
  }

  bool FileTester::testMoveFile() {
    Os::Directory dir;

    Os::FileSystem::removeFile("moveDir/fileToMove.txt");
    Os::FileSystem::removeDirectory("moveDir/");

    auto dirStatus = dir.open("moveDir/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("moveDir/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir.close();
        return false;
      }
      dirStatus = dir.open("moveDir/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir.close();
        return false;
      }
    }

    Os::File file;
    auto fileStatus = file.open("fileToMove.txt", Os::File::Mode::OPEN_WRITE);

    constexpr size_t textMaxSize = 80;
    etl::string<textMaxSize> testText = "Text to be moved! ";

    bool isWriteReadSuccess = false;
    if (fileStatus == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      fileStatus = file.write(testText.data(), testTextSize, false);
      file.close();
    }

    if (fileStatus != Os::File::Status::OP_OK) {
      return false;
    }

    auto fsStatus = Os::FileSystem::moveFile("fileToMove.txt", "moveDir/fileToMove.txt");
    if (fsStatus != Os::FileSystem::Status::OP_OK) {
      return false;
    }

    return true;
  }

  bool FileTester::testCopyFile() {
    Os::Directory dir;

    Os::FileSystem::removeFile("copyDir/fileToCopy.txt");
    Os::FileSystem::removeDirectory("copyDir/");

    auto dirStatus = dir.open("copyDir/");
    if (dirStatus != Os::Directory::Status::OP_OK) {
      auto fsStatus = Os::FileSystem::createDirectory("copyDir/");
      if (fsStatus != Os::FileSystem::OP_OK) {
        dir.close();
        return false;
      }
      dirStatus = dir.open("copyDir/");
      if (dirStatus != Os::Directory::Status::OP_OK) {
        dir.close();
        return false;
      }
    }

    Os::File file;
    auto fileStatus = file.open("fileToCopy.txt", Os::File::Mode::OPEN_WRITE);

    constexpr size_t textMaxSize = 80;
    etl::string<textMaxSize> testText = "Text to be copied! ";

    bool isWriteReadSuccess = false;
    if (fileStatus == Os::File::Status::OP_OK) {
      NATIVE_INT_TYPE testTextSize = testText.size();
      fileStatus = file.write(testText.data(), testTextSize, false);
    }
    file.close();

    if (fileStatus != Os::File::Status::OP_OK) {
      return false;
    }

    auto fsStatus = Os::FileSystem::copyFile("fileToCopy.txt", "copyDir/fileToCopy.txt");
    if (fsStatus != Os::FileSystem::Status::OP_OK) {
      return false;
    }

    return true;
  }
}
