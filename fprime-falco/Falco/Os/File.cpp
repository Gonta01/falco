#include <FpConfig.hpp>
#include <Fw/Types/Assert.hpp>
#include <Os/File.hpp>
#include <etl/array.h>
#include <etl/utility.h>
#include "ff.h"

namespace Os {

    extern Os::File::Status translateFileResult(FRESULT result);
    extern FIL* accessFileObject(NATIVE_INT_TYPE fd);
    extern bool findFreeSlotInFilesDescriptorsPool(NATIVE_INT_TYPE *fileDescriptor);
    extern void freePlaceInFilesDescriptorsPool(NATIVE_INT_TYPE *fd);
    
    File::File() : m_fd(0), m_mode(OPEN_NO_MODE),m_lastError(0) {
        
    }

    File::~File() {
        if (this->m_mode != OPEN_NO_MODE) {
            this->close();
        }
    }

    File::Status File::open(const char* fileName, File::Mode mode) {
        return this->open(fileName,mode,false);
    }

    File::Status File::open(const char* fileName, File::Mode mode, __attribute__ ((unused)) bool include_excl) {
        if(isOpen() == true) {
            return File::Status::OTHER_ERROR;
        }

        bool isFreeSlotFound = findFreeSlotInFilesDescriptorsPool(&m_fd);
        
        if (isFreeSlotFound == false) {
            return File::Status::OTHER_ERROR;
        }

        FRESULT resultCode;

        switch (mode) {
            case File::Mode::OPEN_READ:
                resultCode = f_open(accessFileObject(m_fd), fileName,  FA_READ);
                this->m_mode = OPEN_READ;
            break;

            case File::Mode::OPEN_WRITE:
                resultCode = f_open(accessFileObject(m_fd), fileName,  FA_CREATE_ALWAYS | FA_WRITE);
                this->m_mode = OPEN_WRITE;
            break;

            case File::Mode::OPEN_CREATE:
                resultCode = f_open(accessFileObject(m_fd), fileName,  FA_CREATE_ALWAYS | FA_WRITE);
                this->m_mode = OPEN_CREATE;
            break;

            case File::Mode::OPEN_APPEND:
                resultCode = f_open(accessFileObject(m_fd), fileName,  FA_OPEN_APPEND | FA_WRITE);
                this->m_mode = OPEN_APPEND;
            break;

            default:
                this->m_mode = OPEN_NO_MODE;
            break;
        }

        if (resultCode != FRESULT::FR_OK) {
            freePlaceInFilesDescriptorsPool(&m_fd);
            this->m_mode = OPEN_NO_MODE;
            return Status::NOT_OPENED;
        }
        
        return Status::OP_OK;
    }

    bool File::isOpen() {
      return (this->m_fd > 0);
    }

    File::Status File::seek(NATIVE_INT_TYPE offset, bool absolute) {
        if(isOpen() == false) {
            return File::Status::NOT_OPENED;
        }

        FRESULT resultCode = f_lseek(accessFileObject(m_fd), offset);
        auto status = translateFileResult(resultCode);
        return status;
    }

    File::Status File::flush() {
        if(isOpen() == false) {
            return File::Status::NOT_OPENED;
        }
        FRESULT resultCode = f_sync(accessFileObject(m_fd));
        auto status = translateFileResult(resultCode);
        return status;
    }

    File::Status File::read(void * buffer, NATIVE_INT_TYPE &size, bool waitForFull) {
        if(isOpen() == false) {
            return File::Status::NOT_OPENED;
        }

        UINT bytesRead = 0;    
        FRESULT resultCode = f_read(accessFileObject(m_fd), buffer, size, &bytesRead);
        size = bytesRead;
        
        auto status = translateFileResult(resultCode);
        return status;
    }

    File::Status File::write(const void * buffer, NATIVE_INT_TYPE &size, __attribute__ ((unused)) bool waitForDone) {
        if(isOpen() == false) {
            return File::Status::NOT_OPENED;
        }

        UINT bytesWritten = 0;    
        FRESULT resultCode = f_write(accessFileObject(m_fd), buffer, size, &bytesWritten);
        size = bytesWritten;

        auto status = translateFileResult(resultCode);
        return status;
    }

    void File::close() {
        f_close(accessFileObject(m_fd)); 
        freePlaceInFilesDescriptorsPool(&m_fd);
        this->m_mode = OPEN_NO_MODE;
    }

    NATIVE_INT_TYPE File::getLastError() {
        return f_error(accessFileObject(m_fd));
    }

}
