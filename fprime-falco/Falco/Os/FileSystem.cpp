#include <FpConfig.hpp>
#include <Fw/Types/Assert.hpp>
#include <Os/File.hpp>
#include <Os/Directory.hpp>
#include <Os/FileSystem.hpp>
#include <etl/array.h>
#include <etl/algorithm.h>
#include <etl/limits.h>
#include "ff.h"

namespace Os {
extern Os::File::Status translateFileResult(FRESULT result);
extern FIL& accessFileObject(NATIVE_INT_TYPE fd);
extern bool findFreeSlotInFilesDescriptorsPool(NATIVE_INT_TYPE *fileDescriptor);
extern void freePlaceInFilesDescriptorsPool(NATIVE_INT_TYPE *fd);

extern Os::Directory::Status translateDirectoryResult(FRESULT result);
extern DIR& accessDirectory(POINTER_CAST dir);
extern bool findFreeSlotInDirDescriptorsPool(POINTER_CAST *dir);
extern void freePlaceInDirDescriptorsPool(POINTER_CAST *dir);

namespace FileSystem {
    
    FATFS fileSystemObject;
    etl::array<BYTE, FF_MAX_SS> workArea = {0};
    bool isFileSystemInited = false;

    // Correspondence  between statuses:
    // OP_OK           - FR_OK
    // ALREADY_EXISTS  - FR_EXIST
    // NO_PERMISSION   - FR_WRITE_PROTECTED
    // INVALID_PATH    - FR_INVALID_NAME
    // OTHER_ERROR     - FR_DISK_ERR, FR_INT_ERR, FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_OBJECT, FR_INVALID_DRIVE, FR_NOT_ENABLED,
    //                   FR_NO_FILESYSTEM, FR_MKFS_ABORTED, FR_TIMEOUT, FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES, FR_INVALID_PARAMETER
    Os::FileSystem::Status translateFileSystemResult(FRESULT result) {
        Os::FileSystem::Status status;
        switch (result) {
            case FRESULT::FR_OK:
                status = Os::FileSystem::Status::OP_OK;
            break;
            case FRESULT::FR_EXIST:
                status = Os::FileSystem::Status::ALREADY_EXISTS;
            break;
            case FRESULT::FR_WRITE_PROTECTED:
                status = Os::FileSystem::Status::NO_PERMISSION;
            break;
            case FRESULT::FR_INVALID_NAME:
                status = Os::FileSystem::Status::INVALID_PATH;
            break;
            default:
                status = Os::FileSystem::Status::OTHER_ERROR;
            break;
        }
        return status;
    }
    
    Status initFileSystem() {
        if (isFileSystemInited == true) {
            return FileSystem::Status::ALREADY_EXISTS;
        }

        FRESULT resultCode = f_mount(&fileSystemObject, "", 1);
        if (resultCode != FR_OK) {
            etl::fill(workArea.begin(), workArea.end(), 0);
            resultCode = f_mkfs("0:", nullptr, workArea.data(), workArea.size());
            if (resultCode != FR_OK) {
                return FileSystem::Status::OTHER_ERROR;
            }
            
            resultCode = f_mount(&fileSystemObject, "", 1);
            if (resultCode != FR_OK) {
                return FileSystem::Status::OTHER_ERROR;
            }
        }
        isFileSystemInited = true;
        return OP_OK;
    } //!<  initalize file system

    Status createDirectory(const char* path) {
        FRESULT result = f_mkdir(path);
        auto status = translateFileSystemResult(result);
        return status;
    }  // end createDirectory

    Status removeDirectory(const char* path) {
        FRESULT result = f_unlink(path);
        auto status = translateFileSystemResult(result);
        return status;
    }  // end removeDirectory

    Status removeFile(const char* path) {
        FRESULT result = f_unlink(path);
        auto status = translateFileSystemResult(result);
        return status;
    }  // end removeFile

    Status moveFile(const char* originPath, const char* destPath) {
        FRESULT result = f_rename(originPath, destPath);
        auto status = translateFileSystemResult(result);
        return status;
    }  // end moveFile

    Status copyFile(const char* originPath, const char* destPath) {
        Os::File originFile; 
        Os::File::Status resultCode = originFile.open(originPath, Os::File::Mode::OPEN_READ);
        if (resultCode != Os::File::Status::OP_OK) {
            return OTHER_ERROR;
        }

        Os::File destFile; 
        resultCode = destFile.open(destPath, Os::File::Mode::OPEN_WRITE);
        if (resultCode != Os::File::Status::OP_OK) {
            return OTHER_ERROR;
        }
        
        etl::array<U8, 256> buffer{0};
        
        FwSizeType originFileSize = 0;
        auto fsStatus = Os::FileSystem::getFileSize(originPath, originFileSize);
        if (fsStatus != Os::FileSystem::OP_OK) {
            return OTHER_ERROR;
        }

        for (size_t totalBytesWritten = 0; totalBytesWritten <= originFileSize; totalBytesWritten++) {
            NATIVE_INT_TYPE bytesRead = buffer.size();
            resultCode = originFile.read(buffer.data(), bytesRead);
            if (resultCode != Os::File::Status::OP_OK) {
                break; // Error during read
            }

            if (bytesRead == 0) {
                break; // End of file
            }

            NATIVE_INT_TYPE bytesWrite = bytesRead;
            resultCode = destFile.write(buffer.data(), bytesWrite);
            if (resultCode != Os::File::Status::OP_OK) {
                break; // Error during write
            }
            totalBytesWritten += bytesWrite;
        }

        if (resultCode != Os::File::Status::OP_OK) {
            return OTHER_ERROR;
        }
        return OP_OK;
    }  // end copyFile

    Status getFileSize(const char* path, FwSizeType& size) {
        NATIVE_INT_TYPE fd;
        bool isFreeSlotFound = findFreeSlotInFilesDescriptorsPool(&fd);
        if (isFreeSlotFound == false) {
            return FILE_LIMIT;
        }

        FIL& file = accessFileObject(fd);
        FRESULT result = f_open(&file, path, FA_OPEN_EXISTING);
        if (result != FRESULT::FR_OK) {
            f_close(&file); 
            freePlaceInFilesDescriptorsPool(&fd);
            auto status = translateFileSystemResult(result);
            return status;
        }

        size = f_size(&file);
        f_close(&file); 
        freePlaceInFilesDescriptorsPool(&fd);
        return OP_OK;
    }  // end getFileSize

    // Public function to get the file count for a given directory.
    Status getFileCount(const char* directory, U32& fileCount) {
        POINTER_CAST dir;
        bool isFreeSlotFound = findFreeSlotInDirDescriptorsPool(&dir);
        
        if (isFreeSlotFound == false) {
            return OTHER_ERROR;
        }

        FRESULT resultCode = f_opendir(&accessDirectory(dir), directory);
        if (resultCode != FRESULT::FR_OK) {
            freePlaceInDirDescriptorsPool(&dir);
            return OTHER_ERROR;
        }

        FILINFO fno;
        constexpr U32 loopLimit = etl::numeric_limits<U32>::max();
        for (U32 loopCount = 0; loopCount < loopLimit; loopCount++) {
            resultCode = f_readdir(&accessDirectory(dir), &fno); 
            if (resultCode != FR_OK || fno.fname[0] == 0) {
                break;
            }
            if (fno.fattrib & AM_ARC) {                     
                fileCount++;
            }
        }

        DIR& currentDirectory = accessDirectory(dir);
        f_closedir(&currentDirectory);
        freePlaceInDirDescriptorsPool(&dir);

        return OP_OK;
    }  // end getFileCount

    Status getFreeSpace(const char* path, FwSizeType& totalBytes, FwSizeType& freeBytes) {
        FATFS* fs;
        DWORD freeClusters;
        FRESULT result = f_getfree(path, &freeClusters, &fs); // Get free clusters and FATFS object
        
        if (result != FRESULT::FR_OK) {
            auto status = translateFileSystemResult(result);
            return status;
        }

        // Get file system information
        DWORD sectorPerCluster = fs->csize;  // Sectors per cluster
        DWORD bytesPerSector = FF_MAX_SS;  // Typically 512 bytes per sector (can be checked from the disk)
        
        // Calculate total and free space in bytes
        DWORD totalClusters = (fs->n_fatent - 2);  // Total clusters (minus two reserved ones)

        totalBytes = (FwSizeType)totalClusters * sectorPerCluster * bytesPerSector;
        freeBytes = (FwSizeType)freeClusters * sectorPerCluster * bytesPerSector;
        
        return OP_OK;
    }
    
}  // namespace FileSystem
}  // namespace Os
