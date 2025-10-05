#include <FpConfig.hpp>
#include <Os/File.hpp>
#include <etl/array.h>
#include <etl/utility.h>
#include <Os/Directory.hpp>
#include "Fw/Types/Assert.hpp"
#include "ff.h"

namespace Os {
    
    /**
     * Start of File 
     */
    // Correspondence  between statuses:
    // OP_OK           - FR_OK
    // DOESNT_EXIST    - FR_NO_FILE
    // NO_PERMISSION   - FR_WRITE_PROTECTED
    // OTHER_ERROR     - FR_DISK_ERR, FR_INT_ERR, FR_NOT_READY, FR_NO_PATH, FR_INVALID_NAME, FR_DENIED, FR_EXIST, FR_INVALID_OBJECT, FR_INVALID_DRIVE, FR_NOT_ENABLED,
    //                   FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER


    etl::array<FIL, FF_FS_MAX_FILES> fileObjects;
    
    // Configuration of filesDescriptorsPool is for FF_FS_MAX_FILES == 10
    constexpr U32 maxFilesDescriptors = (FF_FS_MAX_FILES / 2);
    constexpr NATIVE_INT_TYPE firstElementOfFdPool   = 1;
    constexpr NATIVE_INT_TYPE secondElementOfFdPool  = 2;
    constexpr NATIVE_INT_TYPE thirdElementOfFdPool   = 3;
    constexpr NATIVE_INT_TYPE fourthElementOfFdPool  = 4;
    constexpr NATIVE_INT_TYPE fifthElementOfFdPool   = 5;

    etl::array<etl::pair<const NATIVE_INT_TYPE, bool>, maxFilesDescriptors> filesDescriptorsPool = {
        etl::pair<const NATIVE_INT_TYPE, bool>{firstElementOfFdPool, false},
        etl::pair<const NATIVE_INT_TYPE, bool>{secondElementOfFdPool, false},
        etl::pair<const NATIVE_INT_TYPE, bool>{thirdElementOfFdPool, false},
        etl::pair<const NATIVE_INT_TYPE, bool>{fourthElementOfFdPool, false},
        etl::pair<const NATIVE_INT_TYPE, bool>{fifthElementOfFdPool, false}
    };

    Os::File::Status translateFileResult(FRESULT result) {
        Os::File::Status status;
        switch (result) {
            case FRESULT::FR_OK:
                status = Os::File::Status::OP_OK;
            break;
            case FRESULT::FR_NO_FILE:
                status = Os::File::Status::DOESNT_EXIST;
            break;
            case FRESULT::FR_WRITE_PROTECTED:
                status = Os::File::Status::NO_PERMISSION;
            break;
            default:
                status = Os::File::Status::OTHER_ERROR;
            break;
        }

        return status;
    }

    FIL* accessFileObject(NATIVE_INT_TYPE fd) {
        if((fd >= firstElementOfFdPool) && (fd <= fifthElementOfFdPool)) {
            return &fileObjects.at(fd-1);
        }
        return NULL;
    }

    bool findFreeSlotInFilesDescriptorsPool(NATIVE_INT_TYPE *fileDescriptor) {
        for (auto& descriptor : filesDescriptorsPool) {
            if (descriptor.second == false) {
                *fileDescriptor = descriptor.first;
                descriptor.second = true;
                return true;
            }
        }
        return false;
    }

    void freePlaceInFilesDescriptorsPool(NATIVE_INT_TYPE *fd) {
        filesDescriptorsPool.at((*fd)-1).second = false;
        *fd = 0;
    }
    /**
     * End of File 
     */

    /**
     * Start of Directory 
     */
    etl::array<DIR, FF_FS_MAX_FILES> dirObjects;
    
    // Configuration of dirDescriptorsPool is for FF_FS_MAX_FILES == 10
    constexpr U32 maxDirsDescriptors = (FF_FS_MAX_FILES / 2);
    constexpr NATIVE_INT_TYPE firstElementOfDirDescriptorsPool   = 1;
    constexpr NATIVE_INT_TYPE secondElementOfDirDescriptorsPool  = 2;
    constexpr NATIVE_INT_TYPE thirdElementOfDirDescriptorsPool   = 3;
    constexpr NATIVE_INT_TYPE fourthElementOfDirDescriptorsPool  = 4;
    constexpr NATIVE_INT_TYPE fifthElementOfDirDescriptorsPool   = 5;

    etl::array<etl::pair<const POINTER_CAST, bool>, maxDirsDescriptors> dirDescriptorsPool = {
        etl::pair<const POINTER_CAST, bool>{firstElementOfDirDescriptorsPool,  false},
        etl::pair<const POINTER_CAST, bool>{secondElementOfDirDescriptorsPool, false},
        etl::pair<const POINTER_CAST, bool>{thirdElementOfDirDescriptorsPool,  false},
        etl::pair<const POINTER_CAST, bool>{fourthElementOfDirDescriptorsPool, false},
        etl::pair<const POINTER_CAST, bool>{fifthElementOfDirDescriptorsPool,  false},
    };

    DIR* accessDirectory(POINTER_CAST dir) {
        if((dir >= firstElementOfDirDescriptorsPool) && (dir <= fifthElementOfDirDescriptorsPool)) {
            return &dirObjects.at(dir-1);
        }
        return NULL;
    }

    bool findFreeSlotInDirDescriptorsPool(POINTER_CAST *dir) {
        for (auto& descriptor : dirDescriptorsPool) {
            if (descriptor.second == false) {
                POINTER_CAST dirDescriptor = descriptor.first;
                *dir = dirDescriptor;
                descriptor.second = true;
                return true;
            }
        }
        return false;
    }

    void freePlaceInDirDescriptorsPool(POINTER_CAST *dir) {
        dirDescriptorsPool.at((*dir)-1).second = false;
        *dir = 0;
    }

    // Correspondence  between statuses:
    // OP_OK           - FR_OK
    // OTHER_ERROR     - FR_DISK_ERR, FR_INT_ERR, FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME, FR_DENIED, FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, 
    // FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER
    Os::Directory::Status translateDirectoryResult(FRESULT result) {
        Os::Directory::Status status;
        switch (result) {
            case FRESULT::FR_OK:
                status = Os::Directory::Status::OP_OK;
            break;
            default:
                status = Os::Directory::Status::OTHER_ERROR;
            break;
        }
        return status;
    }
    /**
     * End of Directory 
     */
}
