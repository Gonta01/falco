// ======================================================================
// \title  FSPerformanceTester.cpp
// \author user
// \brief  cpp file for FSPerformanceTester component implementation class
// ======================================================================

#include "Components/FSPerformanceTester/FSPerformanceTester.hpp"
#include "FpConfig.hpp"
#include "etl/to_string.h"
#include <ctime>
#include "FreeRTOS.h"
#include "task.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FSPerformanceTester ::
    FSPerformanceTester(const char* const compName) :
      FSPerformanceTesterComponentBase(compName)
  {

  }

  FSPerformanceTester ::
    ~FSPerformanceTester()
  {

  }

  void FSPerformanceTester::configure() {
    metrics.open("metrics.txt", Os::File::Mode::OPEN_CREATE);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void FSPerformanceTester ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    createFileName();
    auto timeOfFileOpening = testOpen();
    auto timeOfFileWriting = testWrite();
    auto timeOfFileReading = testRead();

    // FwSizeType originFileSize = 0;
    // auto fsStatus = Os::FileSystem::getFileSize(fileName.data(), originFileSize);

    numOfFiles++;
    
    logMessage.clear();

    etl::to_string(numOfFiles, logMessage, etl::format_spec(), true);
    logMessage.append(",");

    // etl::to_string(originFileSize, logMessage, etl::format_spec(), true);
    // logMessage.append(",");

    etl::to_string(timeOfFileOpening, logMessage, etl::format_spec(), true);
    logMessage.append(",");

    etl::to_string(timeOfFileWriting, logMessage, etl::format_spec(), true);
    logMessage.append(",");
    
    etl::to_string(timeOfFileReading, logMessage, etl::format_spec(), true);
    logMessage.append("\n");
    
    metrics.open("metrics.txt", Os::File::Mode::OPEN_APPEND);
    NATIVE_INT_TYPE logMessageSize = logMessage.size();
    metrics.write(logMessage.data(), logMessageSize, false);
    metrics.close();
  }

  void FSPerformanceTester::createFileName() {
    // createFileName
    Fw::Time currentTime;
    getCalendarTimeInS(currentTime);
    fileName.clear();
    etl::to_string(currentTime.getSeconds(), fileName, etl::format_spec(), false);
    fileName.append(".txt");
  }

  U32 FSPerformanceTester::testOpen() {
    // creation of file test
    U32 timeMark1 = pdTICKS_TO_MS(xTaskGetTickCount());
    testFile.open(fileName.data(), Os::File::Mode::OPEN_APPEND);
    U32 timeMark2 = pdTICKS_TO_MS(xTaskGetTickCount());

    testFile.close();

    U32 relativeTime = timeMark2 - timeMark1;
    return relativeTime;
  }

  U32 FSPerformanceTester::testWrite() {
    // write to file test
    U32 timeMark1 = pdTICKS_TO_MS(xTaskGetTickCount());
    auto result = testFile.open(fileName.data(), Os::File::Mode::OPEN_APPEND);
    U32 timeMark2 = pdTICKS_TO_MS(xTaskGetTickCount());

    etl::string<textMaxSize> testText = "Testing data! Testing data! Testing data! Testing data!";

    NATIVE_INT_TYPE testTextSize = testText.size();
    result = testFile.write(testText.data(), testTextSize, false);

    testFile.close();

    U32 relativeTime = timeMark2 - timeMark1;
    return relativeTime;
  }
  
  U32 FSPerformanceTester::testRead() {
    // read test
    U32 timeMark1 = pdTICKS_TO_MS(xTaskGetTickCount());
    auto result = testFile.open(fileName.data(), Os::File::Mode::OPEN_READ);
    U32 timeMark2 = pdTICKS_TO_MS(xTaskGetTickCount());

    etl::string<textMaxSize> readText;
    readText.initialize_free_space();
    NATIVE_INT_TYPE readTextCapacity = readText.capacity();
    result = testFile.read(readText.data(), readTextCapacity);

    testFile.close();

    U32 relativeTime = timeMark2 - timeMark1;
    return relativeTime;
  }

  void FSPerformanceTester::getCalendarTimeInS(Fw::Time& currentTimeS) {
    Drv::Calendar::Time calendarTime;
    this->getTime_out(0, calendarTime);

    struct tm now;
    now.tm_sec   = calendarTime.getTm_sec();
    now.tm_min   = calendarTime.getTm_min();
    now.tm_hour  = calendarTime.getTm_hour();
    now.tm_mday  = calendarTime.getTm_mday();
    now.tm_mon   = calendarTime.getTm_mon();
    now.tm_year  = calendarTime.getTm_year();
    now.tm_wday  = calendarTime.getTm_wday();
    now.tm_yday  = calendarTime.getTm_yday();
    now.tm_isdst = calendarTime.getTm_isdst();
    time_t sec = mktime(&now);
    Fw::Time Now;
    Now.set(sec, 0);
    currentTimeS = Now;
  }

}
