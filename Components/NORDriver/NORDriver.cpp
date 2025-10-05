// ======================================================================
// \title  NORDriver.cpp
// \author user
// \brief  cpp file for NORDriver component implementation class
// ======================================================================

#include "Components/NORDriver/NORDriver.hpp"
#include "FpConfig.hpp"
#include "NORDriver.hpp"
#include <cstddef>
#include <cstdint>
#include <etl/array.h>
#include <etl/expected.h>
#include <etl/private/bitset_new.h>
#include <etl/span.h>

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

using QSPIAddressLength = Drv::QSPI::QSPIAddressLength;
using QSPILaneWidth = Drv::QSPI::QSPILaneWidth;
using QSPIOptionLength = Drv::QSPI::QSPIOptionLength;

using QPSICommandConfig = Drv::QSPI::QPSICommandConfig;
using QSPIRegisterConfig = Drv::QSPI::QSPIRegisterConfig;
using QSPIMemoryConfig = Drv::QSPI::QSPIMemoryConfig;

using ErrorCode = Drv::NOR::ErrorCode;

Components::NORDriver::NORDriver(const char* const compName) : NORDriverComponentBase(compName) {

}

Components::NORDriver::~NORDriver() {

}

void Components::NORDriver::configure(PowerDown powerDownMode) {
    if (powerDownMode == PowerDown::Disable) {
        auto powerDownResult = releasePowerDown();
        if (powerDownResult != ErrorCode::NoError) {
            errorReport(powerDownResult);
            return;
        }
    }

    auto exitQPIModeResult = exitQPIMode();
    if (exitQPIModeResult != ErrorCode::NoError) {
        errorReport(exitQPIModeResult);
        return;
    }

    auto quadEnableResult = quadEnable(QSPILaneWidth::SINGLE_BIT_SPI);
    if (quadEnableResult != ErrorCode::NoError) {
       errorReport(quadEnableResult);
       return;
    }

    auto qpiModeResult = enterQPIMode();
    if (qpiModeResult != ErrorCode::NoError) {
       errorReport(qpiModeResult);
       return;
    }

   auto resetResult = reset();
   if (resetResult != ErrorCode::NoError) {
        errorReport(resetResult);
        return;
   }

    quadEnableResult = quadEnable(QSPILaneWidth::SINGLE_BIT_SPI);
    if (quadEnableResult != ErrorCode::NoError) {
       errorReport(quadEnableResult);
       return;
    }

    qpiModeResult = enterQPIMode();
    if (qpiModeResult != ErrorCode::NoError) {
       errorReport(qpiModeResult);
       return;
    }

    auto jedecID = readID();
    if (!jedecID.has_value()) {
        errorReport(jedecID.error());
        return;
    }
    if (W25Q01JV_ID != jedecID.value()) {
        errorReport(ErrorCode::WrongMemoryId);
        return;
    }

    auto enter4ByteModeResult = enter4ByteAddressMode();
    if (enter4ByteModeResult != ErrorCode::NoError) {
        errorReport(enter4ByteModeResult);
        return;
    }

    auto setReadParamsResult = setReadParameters(NumOfDummyClocks::Clocks8);
    if (setReadParamsResult != ErrorCode::NoError) {
        errorReport(setReadParamsResult);
        return;
    }

    if(powerDownMode == PowerDown::Enable) {
        auto powerDownResult = powerDown();
        if (powerDownResult != ErrorCode::NoError) {
            errorReport(powerDownResult);
            return;
        }
    }

}

Drv::NOR::ErrorCode Components::NORDriver::read_handler(NATIVE_INT_TYPE portNum, U32 startAddress, Fw::Buffer& dataInBuff) {
    size_t const lastAddress{startAddress + dataInBuff.getSize() - 1};
    
    if (dataInBuff.getSize() == 0) {
        errorReport(ErrorCode::InputBufferEmpty);
        return ErrorCode::InputBufferEmpty;
    }

    if ((lastAddress > memorySizeBytes - 1) || (startAddress > memorySizeBytes - 1)) {
        errorReport(ErrorCode::AccessOutOfRange);
        return ErrorCode::AccessOutOfRange;
    }

    QSPIMemoryConfig config{};
    config.setInstruction(static_cast<uint8_t>(commandCode::readFastData));
    config.setOption_en(false);
    config.setContinuous_read_en(false);
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setOption_len(QSPIOptionLength::OPTL_8_BIT);
    config.setOption(0x00U);
    config.setDummy_cycles(qpiModeReadClocks);

    if (auto error = this->memoryRead_out(0, config, startAddress, dataInBuff); error != Drv::QSPI::QSPIError::NoError) {
        errorReport(ErrorCode::IOInterfaceError);
        return Drv::NOR::ErrorCode::IOInterfaceError;
    }

    return Drv::NOR::ErrorCode::NoError;
}

Drv::NOR::ErrorCode Components::NORDriver::write_handler(NATIVE_INT_TYPE portNum, U32 startAddress, Fw::Buffer& dataOutBuff) {
    Drv::NOR::ErrorCode error = ErrorCode::NoError;
    size_t const lastAddress{startAddress + dataOutBuff.getSize() - 1};

    if (dataOutBuff.getSize() == 0) {
        errorReport(ErrorCode::InputBufferEmpty);
        return ErrorCode::InputBufferEmpty;
    }

    if ((lastAddress > memorySizeBytes - 1) || (startAddress > memorySizeBytes - 1)) {
        errorReport(ErrorCode::AccessOutOfRange);
        return ErrorCode::AccessOutOfRange;
    }

    uint32_t bytesLength = dataOutBuff.getSize();
    uint32_t currentAddress = startAddress;
    uint32_t currentPageStart = currentAddress - currentAddress % sizeOfPage;
    const uint32_t currentPageEnd = currentPageStart + sizeOfPage - 1;
    uint32_t nextPageStart = currentPageStart + sizeOfPage;
    auto remainingSpaceOnCurrentPage = static_cast<uint16_t>(currentPageEnd - currentAddress + 1);
    uint32_t bytesStart = 0;

    QSPIMemoryConfig config{};
    config.setInstruction(static_cast<uint8_t>(commandCode::pageProgram));
    config.setOption_en(false);
    config.setContinuous_read_en(false);
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setOption_len(QSPIOptionLength::OPTL_8_BIT);
    config.setOption(0x00U);
    config.setDummy_cycles(0);

    while ((bytesLength > 0) && (error == ErrorCode::NoError)) {
        error = waitUntilFlashReady(WaitProcess::PageProgram);
        if (error == ErrorCode::NoError) {
            error = writeEnable();
        }
        if (error == ErrorCode::NoError) {
            error = waitForWriteEnableLatch();
        }

        if ((bytesLength > remainingSpaceOnCurrentPage) && (error == ErrorCode::NoError)) {
            const etl::span dataTransmit{
                dataOutBuff.getData() + bytesStart, remainingSpaceOnCurrentPage};
            
            Fw::Buffer dataTransmitBuffer(reinterpret_cast<U8 *>(dataTransmit.data()), dataTransmit.size_bytes());

            if (Drv::QSPI::QSPIError::NoError != this->memoryWrite_out(0, config, currentAddress, dataTransmitBuffer)) {
                error = ErrorCode::IOInterfaceError;
            }

            if (error == ErrorCode::NoError) {
                bytesLength -= remainingSpaceOnCurrentPage;
                bytesStart += remainingSpaceOnCurrentPage;
                currentAddress = nextPageStart;
                currentPageStart = nextPageStart;
                remainingSpaceOnCurrentPage = sizeOfPage;
                nextPageStart = currentPageStart + sizeOfPage;
            }
        } else {
            const etl::span dataTransmit{
                dataOutBuff.getData() + bytesStart, bytesLength};

            Fw::Buffer dataTransmitBuffer(reinterpret_cast<U8 *>(dataTransmit.data()), dataTransmit.size_bytes());    
            if (Drv::QSPI::QSPIError::NoError != this->memoryWrite_out(0, config, currentAddress, dataTransmitBuffer)) {
                error = ErrorCode::IOInterfaceError;
            }
            bytesLength = 0;
        }
    }
    if (error != ErrorCode::NoError) {
        errorReport(error);
        return error;
    }
    if (error = waitUntilFlashReady(WaitProcess::PageProgram); error != ErrorCode::NoError) {
        errorReport(error);
        return error;
    }

    return error;
}

Drv::NOR::ErrorCode Components::NORDriver::erase_handler(NATIVE_INT_TYPE portNum, U32 startAddress, U32 length) {
    uint32_t nextAddr        = startAddress;
    uint32_t sizeLeftToErase = length;

    size_t const lastAddress{startAddress + length - 1};
    if ((lastAddress > memorySizeBytes - 1) || (startAddress > memorySizeBytes - 1)) {
        errorReport(ErrorCode::AccessOutOfRange);
        return ErrorCode::AccessOutOfRange;
    }

    if ((startAddress % sizeOfSectorBytes != 0) || (length % sizeOfSectorBytes != 0)) {
        errorReport(ErrorCode::UnalignedAccess);
        return ErrorCode::UnalignedAccess;
    }

    ErrorCode error = ErrorCode::NoError;
    while ((error == ErrorCode::NoError) && (sizeLeftToErase > 0)) {
        if ((sizeLeftToErase >= size_64KB) && ((nextAddr & mask_64KB) == 0U)) {
            error = eraseBlock64kb(nextAddr);
            nextAddr += size_64KB;
            sizeLeftToErase -= size_64KB;
        } else if ((sizeLeftToErase >= size_32KB) && ((nextAddr & mask_32KB) == 0U)) {
            error = eraseBlock32kb(nextAddr);
            nextAddr += size_32KB;
            sizeLeftToErase -= size_32KB;
        } else if ((sizeLeftToErase >= size_4KB) && ((nextAddr & mask_4KB) == 0U)) {
            error = eraseBlock4kb(nextAddr);
            nextAddr += size_4KB;
            sizeLeftToErase -= size_4KB;
        } else {
            error = ErrorCode::IncompleteErase;
        }
    }

    errorReport(error);
    return error;
}

U32 Components::NORDriver::getMemorySize_handler(NATIVE_INT_TYPE portNum) {
    return memorySizeBytes;
}

U32 Components::NORDriver::getSectorSize_handler(NATIVE_INT_TYPE portNum) {
    return sizeOfSectorBytes;
}

Drv::NOR::ErrorCode Components::NORDriver::fullErase() {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::chipErase));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    ErrorCode error = ErrorCode::NoError;

    error = waitUntilFlashReady(WaitProcess::Default);
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = writeEnable();
    if (error != ErrorCode::NoError) {
        return error;
    }

    error = waitForWriteEnableLatch();
    if (error != ErrorCode::NoError) {
        return error;
    }

    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    error = waitUntilFlashReady(WaitProcess::ChipErase);
    if (error != ErrorCode::NoError) {
        return ErrorCode::FlashMemoryBusy;
    }

    return error;
}

ErrorCode Components::NORDriver::reset() {
    mutex.lock();

    // Data corruption may happen if device is reset if there is an on-going or suspended internal Erase or
    // Program operation. So BUSY bit is checked for the worst scenario when full chip erase is on-going
    if (auto error = selectDie(Die::Die1); error != ErrorCode::NoError) {
        mutex.unlock();
        return error;
    }
    if (auto error = waitUntilFlashReady(WaitProcess::ChipErase); error != ErrorCode::NoError) {
        mutex.unlock();
        return error;
    }

    if (auto error = selectDie(Die::Die0); error != ErrorCode::NoError) {
        mutex.unlock();
        return error;
    }
    if (auto error = waitUntilFlashReady(WaitProcess::ChipErase); error != ErrorCode::NoError) {
        mutex.unlock();
        return error;
    }

    QPSICommandConfig config{};
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::resetEnable));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    if (auto error = this->commandWrite_out(0, config, 0); error != Drv::QSPI::QSPIError::NoError) {
        mutex.unlock();
        return ErrorCode::IOInterfaceError;
    }

    config.setInstruction(static_cast<uint8_t>(commandCode::resetDevice));

    if (auto error = this->commandWrite_out(0, config, 0); error != Drv::QSPI::QSPIError::NoError) {
        mutex.unlock();
        return ErrorCode::IOInterfaceError;
    }

    // The device will take approximately 30 uS to reset
    const uint16_t timeToWaitMs = 2;
    Os::Task::delay(timeToWaitMs);
    mutex.unlock();
    return ErrorCode::NoError;
}

etl::expected<uint32_t, ErrorCode> Components::NORDriver::readID() {
    QSPIRegisterConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::readJedecId));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setDummy_cycles(0);

    etl::array<uint8_t, 3> deviceID = {0};
    Fw::Buffer deviceIDBuffer(deviceID.data(), deviceID.size());
    if (auto error = this->registerRead_out(0, config, deviceIDBuffer); error != Drv::QSPI::QSPIError::NoError) {
        return etl::unexpected(ErrorCode::IOInterfaceError);
    }

    constexpr uint8_t coeff32BitSize = 32U;
    constexpr uint8_t mask8BitShift = 8U;
    etl::bitset<coeff32BitSize, uint32_t> result;

    for (size_t i = 0; i < deviceID.size(); i++) {
        result |= deviceID.at(i) << (mask8BitShift * i);
    }

    return result.to_ulong();
}

ErrorCode Components::NORDriver::eraseBlock64kb(uint32_t startAddress) {
    if (auto error = waitUntilFlashReady(WaitProcess::Default); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = writeEnable(); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = waitForWriteEnableLatch(); error != ErrorCode::NoError) {
        return error;
    }

    QPSICommandConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::blockErase64KB));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setAddr_en(true);
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    if (auto error = this->commandWrite_out(0, config, startAddress); error != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    if (auto error = waitUntilFlashReady(WaitProcess::BlockErase64KB); error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::NORDriver::eraseBlock32kb(uint32_t startAddress) {
    if (auto error = waitUntilFlashReady(WaitProcess::Default); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = writeEnable(); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = waitForWriteEnableLatch(); error != ErrorCode::NoError) {
        return error;
    }

    QPSICommandConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::blockErase32KB));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setAddr_en(true);
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    if (auto error = this->commandWrite_out(0, config, startAddress); error != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    if (auto error = waitUntilFlashReady(WaitProcess::BlockErase32KB); error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::NORDriver::eraseBlock4kb(uint32_t startAddress) {
    if (auto error = waitUntilFlashReady(WaitProcess::Default); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = writeEnable(); error != ErrorCode::NoError) {
        return error;
    }

    if (auto error = waitForWriteEnableLatch(); error != ErrorCode::NoError) {
        return error;
    }

    QPSICommandConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::sectorErase));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setAddr_en(true);
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);

    if (auto error = this->commandWrite_out(0, config, startAddress); error != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    if (auto error = waitUntilFlashReady(WaitProcess::SectorErase4KB); error != ErrorCode::NoError) {
        return error;
    }

    return ErrorCode::NoError;
}

ErrorCode Components::NORDriver::waitUntilFlashReady(WaitProcess process) {
    ErrorCode error = ErrorCode::NoError;
    uint32_t timeToWaitMs = 0;
    uint32_t timeToWaitS = 0;
    uint32_t loopCounter = 0;

    switch (process) {
        case WaitProcess::WriteStatusRegister:
            timeToWaitMs = writeStatusWaitMs;
            loopCounter = writeStatusLoopCnt;
            break;
        case WaitProcess::PageProgram:
            timeToWaitMs = pageProgramWaitMs;
            loopCounter = pageProgramLoopCnt;
            break;
        case WaitProcess::SectorErase4KB:
            timeToWaitMs = sectorErase4KBWaitMs;
            loopCounter = sectorErase4KBLoopCnt;
            break;
        case WaitProcess::BlockErase32KB:
            timeToWaitMs = blockErase32KBWaitMs;
            loopCounter = blockErase32KBLoopCnt;
            break;
        case WaitProcess::BlockErase64KB:
            timeToWaitMs = blockErase64KBWaitMs;
            loopCounter = blockErase64KBLoopCnt;
            break;
        case WaitProcess::ChipErase:
            timeToWaitS = chipEraseWaitMs;
            loopCounter = chipEraseLoopCnt;
            break;
        default:
            timeToWaitMs = defaultWaitMs;
            loopCounter = defaultLoopCnt;
            break;
    }
    bool exit = false;

    while ((loopCounter > 0) && !exit) {
        if (auto status = getStatusRegister(StatusRegister::RG1, QSPILaneWidth::QUAD_CMD); status.has_value()) {
            constexpr uint8_t StatusRegister1BUSYBitMask = 1U;
            if ((status.value() & StatusRegister1BUSYBitMask) != 0) {
                loopCounter--;
                if (process == WaitProcess::ChipErase) {
                  Os::Task::delay(timeToWaitS);
                } else {
                  Os::Task::delay(timeToWaitMs);
                }
            } else {
                exit = true;
            }
        } else {
            exit = true;
            error = status.error();
        }
    }

    if (loopCounter == 0) {
        error = ErrorCode::FlashMemoryBusy;
    }

    return error;
}

ErrorCode Components::NORDriver::waitForWriteEnableLatch() {
    ErrorCode error = ErrorCode::NoError;
    const uint16_t timeToWaitMs = 2;
    constexpr uint32_t loopCntForWriteEnable = 20;
    uint32_t loopCounter = loopCntForWriteEnable;
    bool exit = false;

    while ((loopCounter > 0) && !exit) {
        if (auto status = getStatusRegister(StatusRegister::RG1, QSPILaneWidth::QUAD_CMD); status.has_value()) {
            constexpr uint8_t StatusRegister1WELBitMask = 2U;
            if ((status.value() & StatusRegister1WELBitMask) != 0) {
                exit = true;
            } else {
                loopCounter--;
                Os::Task::delay(timeToWaitMs);
            }
        } else {
            exit = true;
            error = status.error();
        }
    }

    if (loopCounter == 0) {
        error = ErrorCode::FlashMemoryBusy;
    }

    return error;
}

ErrorCode Components::NORDriver::writeEnable() {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::writeEnable));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }

    return error;
}

ErrorCode Components::NORDriver::writeEnableVolatile(const QSPILaneWidth &QSPILaneWidth) {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::volatileWriteEnable));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth);

    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }

    return error;
}

etl::expected<uint8_t, ErrorCode> Components::NORDriver::getStatusRegister(StatusRegister statusRegister, const QSPILaneWidth &QSPILaneWidth) {
    etl::array<uint8_t, 1> status{0};

    QSPIRegisterConfig config;
    switch (statusRegister) {
        case StatusRegister::RG1:
            config.setInstruction(static_cast<uint8_t>(commandCode::readStatusRG1));
            break;

        case StatusRegister::RG2:
            config.setInstruction(static_cast<uint8_t>(commandCode::readStatusRG2));
            break;

        case StatusRegister::RG3:
            config.setInstruction(static_cast<uint8_t>(commandCode::readStatusRG3));
            break;

        default:
            break;
    }

    config.setWidth(QSPILaneWidth);
    config.setDummy_cycles(0);
    
    Fw::Buffer statusBuffer(status.data(), status.size());
    if (auto error = this->registerRead_out(0, config, statusBuffer); error != Drv::QSPI::QSPIError::NoError) {
        return etl::unexpected(ErrorCode::IOInterfaceError);
    }

    return status.front();
}

ErrorCode Components::NORDriver::writeRegister(Components::NORDriver::StatusRegister statusRegister, uint8_t value, const QSPILaneWidth &QSPILaneWidth) {
    QSPIRegisterConfig config;
    switch (statusRegister) {
        case StatusRegister::RG1:
            config.setInstruction(static_cast<uint8_t>(commandCode::writeStatusRG1));
            break;

        case StatusRegister::RG2:
            config.setInstruction(static_cast<uint8_t>(commandCode::writeStatusRG2));
            break;

        case StatusRegister::RG3:
            config.setInstruction(static_cast<uint8_t>(commandCode::writeStatusRG3));
            break;

        default:
            break;
    }

    config.setWidth(QSPILaneWidth);
    config.setDummy_cycles(0);

    etl::array<uint8_t, 1> regValue{value};
    Fw::Buffer regValueBuffer(regValue.data(), regValue.size());
    if (auto error = this->registerWrite_out(0, config, regValueBuffer); error != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    // Write Status Register Time max value is 15 ms
    const uint8_t timeToWaitMs = 20;
    Os::Task::delay(timeToWaitMs);

    return ErrorCode::NoError;
}

ErrorCode Components::NORDriver::enter4ByteAddressMode() {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::enter4ByteAddressMode));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }

    return error;
}

ErrorCode Components::NORDriver::quadEnable(const QSPILaneWidth &QSPILaneWidth) {
    auto statusRegister2 = getStatusRegister(StatusRegister::RG2, QSPILaneWidth);
    if (!statusRegister2.has_value()) {
        return statusRegister2.error();
    }

    auto writeEnableVolatileResult = writeEnableVolatile(QSPILaneWidth);
    if (writeEnableVolatileResult != ErrorCode::NoError) {
        return writeEnableVolatileResult;
    }

    uint8_t quadEnable = statusRegister2.value();
    constexpr uint8_t quadEnableMask = 0b1111'1101;
    quadEnable &= quadEnableMask;
    quadEnable |= static_cast<uint8_t>(~quadEnableMask);

    auto writeRegisterResult = writeRegister(StatusRegister::RG2, quadEnable, QSPILaneWidth);
    if (writeRegisterResult != ErrorCode::NoError) {
        return writeRegisterResult;
    }
    
    return ErrorCode::NoError;
}

ErrorCode Components::NORDriver::enterQPIMode() {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::enterQPIMode));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::SINGLE_BIT_SPI);

    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }

    return error;
}

ErrorCode Components::NORDriver::exitQPIMode() {
    QPSICommandConfig config;
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::exitQPIMode));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = this->commandWrite_out(0, config, 0); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }

    return error;
}

ErrorCode Components::NORDriver::selectDie(Components::NORDriver::Die dieID) {
    QSPIRegisterConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::softwareDieSelect));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setDummy_cycles(0);

    etl::array<uint8_t, 1> die{static_cast<uint8_t>(dieID)};
    Fw::Buffer dieBuffer(die.data(), die.size());
    ErrorCode error = ErrorCode::NoError;
    if (auto qspiError = registerWrite_out(0, config, dieBuffer); qspiError != Drv::QSPI::QSPIError::NoError) {
        error = ErrorCode::IOInterfaceError;
    }
    return error;
}

Drv::NOR::ErrorCode Components::NORDriver::setReadParameters(NumOfDummyClocks numOfDummyClocks) {
    QSPIRegisterConfig config;
    config.setInstruction(static_cast<uint8_t>(commandCode::setReadParameters));
    config.setWidth(QSPILaneWidth::QUAD_CMD);
    config.setDummy_cycles(0);

    etl::array<uint8_t, 1> regValue{static_cast<uint8_t>(numOfDummyClocks)};
    Fw::Buffer regValueBuffer(regValue.data(), regValue.size());
    if (auto error = this->registerWrite_out(0, config, regValueBuffer); error != Drv::QSPI::QSPIError::NoError) {
        return ErrorCode::IOInterfaceError;
    }

    switch(numOfDummyClocks) {
        case NumOfDummyClocks::Clocks2:
            qpiModeReadClocks = 2;
        break;

        case NumOfDummyClocks::Clocks4:
            qpiModeReadClocks = 4;
        break;

        case NumOfDummyClocks::Clocks6:
            qpiModeReadClocks = 4;
        break;

        case NumOfDummyClocks::Clocks8:
            qpiModeReadClocks = 8;
        break;

        default:

        break;
    }

    // Write Status Register Time max value is 15 ms
    const uint8_t timeToWaitMs = 20;
    Os::Task::delay(timeToWaitMs);

    return ErrorCode::NoError;
}

Drv::NOR::ErrorCode Components::NORDriver::powerDown() {
    QPSICommandConfig config{};
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::powerDown));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    if (auto error = this->commandWrite_out(0, config, 0); error != Drv::QSPI::QSPIError::NoError) {
        mutex.unlock();
        return ErrorCode::IOInterfaceError;
    }
    return ErrorCode::NoError;
}

Drv::NOR::ErrorCode Components::NORDriver::releasePowerDown() {
    QPSICommandConfig config{};
    config.setAddr_en(false);
    config.setInstruction(static_cast<uint8_t>(commandCode::releasePowerDown));
    config.setAddr_len(QSPIAddressLength::ADDRL_32_BIT);
    config.setWidth(QSPILaneWidth::QUAD_CMD);

    if (auto error = this->commandWrite_out(0, config, 0); error != Drv::QSPI::QSPIError::NoError) {
        mutex.unlock();
        return ErrorCode::IOInterfaceError;
    }
    return ErrorCode::NoError;
}

void Components::NORDriver::errorReport(ErrorCode errorCode) {
  switch (errorCode) {
      case ErrorCode::NoError:

      break;
      case ErrorCode::WrongMemoryId:
          this->log_WARNING_HI_WrongMemoryId();
      break;
      case ErrorCode::InputBufferEmpty:
          this->log_WARNING_HI_InputBufferEmpty();
      break;
      case ErrorCode::UnalignedAccess:
          this->log_WARNING_HI_UnalignedAccess();
      break;  
      case ErrorCode::AccessOutOfRange:
          this->log_WARNING_HI_AccessOutOfRange();
      break;  
      case ErrorCode::IncompleteErase:
          this->log_WARNING_HI_IncompleteErase();
      break;
      case ErrorCode::IOInterfaceError:
          this->log_WARNING_HI_IOInterfaceError();
      break;
      case ErrorCode::FlashMemoryBusy:
          this->log_WARNING_HI_FlashMemoryBusy();
      break;
      default:
          this->log_WARNING_HI_UnknownError();
      break;
  }
}