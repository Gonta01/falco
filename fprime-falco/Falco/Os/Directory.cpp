#include <FpConfig.hpp>
#include <Os/Directory.hpp>
#include <Fw/Types/Assert.hpp>
#include <etl/array.h>
#include <etl/algorithm.h>
#include "ff.h"

namespace Os {

    extern Os::Directory::Status translateDirectoryResult(FRESULT result);
    extern DIR* accessDirectory(POINTER_CAST dir);
    extern bool findFreeSlotInDirDescriptorsPool(POINTER_CAST *dir);
    extern void freePlaceInDirDescriptorsPool(POINTER_CAST *dir);
    
    Directory::Directory() :m_dir(0),m_lastError(0) {
    }

    Directory::~Directory() {
        if (this->isOpen()) {
            this->close();
        }
    }

    Directory::Status Directory::open(const char* dirName) {
        bool isFreeSlotFound = findFreeSlotInDirDescriptorsPool(&m_dir);
        
        if (isFreeSlotFound == false) {
            return Directory::Status::OTHER_ERROR;
        }

        FRESULT resultCode = f_opendir(accessDirectory(m_dir), dirName);
        if (resultCode != FRESULT::FR_OK) {
            freePlaceInDirDescriptorsPool(&m_dir);
            return Directory::Status::OTHER_ERROR;
        }
        
        return translateDirectoryResult(resultCode);
    }

    Directory::Status Directory::rewind() {
        if(isOpen() == false) {
            return Directory::Status::NOT_OPENED;
        }

        FRESULT resultCode = f_rewinddir(accessDirectory(m_dir));

        return translateDirectoryResult(resultCode);
    }

    Directory::Status Directory::read(char * fileNameBuffer, U32 bufSize) {
        if(isOpen() == false) {
            return Directory::Status::NOT_OPENED;
        }

        FILINFO fno;
        FRESULT resultCode = f_readdir(accessDirectory(m_dir), &fno);

        if (resultCode == FR_OK) {
            if (fno.fname[0] == 0) {
                fileNameBuffer[0] = '\0';
                return NO_MORE_FILES;
            }
            else {
                auto minOfBuffSizes = etl::min(static_cast<U32>(FF_LFN_BUF), bufSize);
                
                strncpy(fileNameBuffer, fno.fname, minOfBuffSizes - 1);
                fileNameBuffer[minOfBuffSizes - 1] = '\0';
            }
        }

        return translateDirectoryResult(resultCode);
    }

    bool Directory::isOpen() {
        return (this->m_dir > 0);
    }

    void Directory::close() {
        DIR* currentDirectory = accessDirectory(this->m_dir);
        f_closedir(currentDirectory);
        freePlaceInDirDescriptorsPool(&m_dir);
    }
}
