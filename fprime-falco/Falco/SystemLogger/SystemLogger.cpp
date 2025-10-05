#include "SystemLogger.hpp"

#define LOG_FILE_PATH "dump.txt"
#define MAX_LOG_BUFFER 256
    
namespace Falco {
    
    SystemLogger::SystemLogger() {

    }
    
    SystemLogger::~SystemLogger(){

    }

    void SystemLogger::log(const char* fmt, POINTER_CAST a0, POINTER_CAST a1,
            POINTER_CAST a2, POINTER_CAST a3, POINTER_CAST a4, POINTER_CAST a5,
            POINTER_CAST a6, POINTER_CAST a7, POINTER_CAST a8, POINTER_CAST a9) 
    {
        auto fileStatus = this->dumpFile.open(dumpFileName.data(), Os::File::OPEN_APPEND, false);
        if (fileStatus != Os::File::Status::OP_OK) {
            return;
        } 

        char buffer[MAX_LOG_BUFFER];
        snprintf(buffer, MAX_LOG_BUFFER, fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);

        etl::string<MAX_LOG_BUFFER> stringToWrite = buffer;
        snprintf(stringToWrite.data(), stringToWrite.size(), fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        writeToFile(stringToWrite.data(), stringToWrite.size());
        dumpFile.close();
    }
    
    void SystemLogger::writeToFile(char* msg, NATIVE_INT_TYPE msgSize) {
        NATIVE_INT_TYPE writeSize = msgSize;
        Os::File::Status stat = this->dumpFile.write(msg, writeSize);
    }
} //End namespace Falco
