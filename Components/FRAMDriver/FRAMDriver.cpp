// ======================================================================
// \title  FRAMDriver.cpp
// \author user
// \brief  cpp file for FRAMDriver component implementation class
// ======================================================================

#include "Components/FRAMDriver/FRAMDriver.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FRAMDriver ::
    FRAMDriver(const char* const compName) :
      FRAMDriverComponentBase(compName),
      isInited(false)
  {

  }

  FRAMDriver ::
    ~FRAMDriver()
  {

  }

  void FRAMDriver::configure() {
    ErrorCode initResult = initialize();
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  Drv::FRAM::InStatus FRAMDriver :: read_handler(NATIVE_INT_TYPE portNum, U32 addr, Fw::Buffer& buffer) {
    if (isInited) {
      auto readResult = read(addr, buffer);
      if (readResult.has_value()) {
        return Drv::FRAM::InStatus::SUCCESS;
      }
    } 
    
    return Drv::FRAM::InStatus::FAILED;
  }

  Drv::FRAM::InStatus FRAMDriver :: write_handler(NATIVE_INT_TYPE portNum, U32 addr, Fw::Buffer& buffer) {
    if (isInited) {
      auto writeResult = write(addr, buffer);
      if (writeResult.has_value()) {
        return Drv::FRAM::InStatus::SUCCESS;
      }
    }

    return Drv::FRAM::InStatus::FAILED;
  }

  Drv::FRAM::InStatus FRAMDriver :: sleep_handler(NATIVE_INT_TYPE portNum) {
    if (isInited) {
      auto sleepResult = sleep();
      if (sleepResult == ErrorCode::NoError) {
        return Drv::FRAM::InStatus::SUCCESS;
      }
    }

    return Drv::FRAM::InStatus::FAILED;
  }

  Drv::FRAM::InStatus FRAMDriver :: wakeup_handler(NATIVE_INT_TYPE portNum) {
    if (isInited) {
      auto wakeupResult = wakeup();
      if (wakeupResult == ErrorCode::NoError) {
        return Drv::FRAM::InStatus::SUCCESS;
      }
    }

    return Drv::FRAM::InStatus::FAILED;
  }

  void FRAMDriver::reinit_handler(NATIVE_INT_TYPE portNum) {
    this->driver_reinit_out(0);
    ErrorCode error = initialize();
    if (error != ErrorCode::NoError) {
      this->log_WARNING_HI_Uninited();
    }
  }

  [[nodiscard]] FRAMDriver::ErrorCode FRAMDriver::initialize() {
    isInited = true;

    auto memoryID = readID();
    if (!memoryID.has_value()) {
        isInited = false;
        return memoryID.error();
    }

    ErrorCode errorCode = ErrorCode::NoError;
    if (MB85RS4MT_ID != memoryID.value()) {
        isInited = false;
        errorCode = ErrorCode::WrongMemoryId;
    } else {

    if (auto result = writeEnable(); result.error() != ErrorCode::NoError) {
      return result.error();
    }

    // Block protect is not used
    if (auto result = writeStatusRegister(0); result.error() != ErrorCode::NoError) {
        isInited = false;
        errorCode = result.error();
    }

    if (auto result = writeDisable(); result.error() != ErrorCode::NoError) {
      return result.error();
    }
    }

    return errorCode;
  }

  etl::expected<uint32_t, FRAMDriver::ErrorCode>
  FRAMDriver::read(uint32_t addr, Fw::Buffer &dataIn) {
    if (addr + dataIn.getSize() > memorySize) {
        return etl::unexpected(ErrorCode::AddressOutOfRange);
    }

    if (dataIn.getSize() == 0) {
        return etl::unexpected(ErrorCode::InputBufferEmpty);
    }

    etl::array header = {
        static_cast<uint8_t>(CommandCode::CmdRead), static_cast<uint8_t>(addr >> thirdByteOffset),
        static_cast<uint8_t>(addr >> secondByteOffset), static_cast<uint8_t>(addr >> firstByteOffset)};
    
    chipSelectReset();

    Fw::Buffer headerBuff(header.data(), header.size());

    if (this->driver_write_out(0, headerBuff, writeTimeoutMs) == Drv::SPI::Status::NoError) {
        Drv::SPI::Status err = this->driver_read_out(0, dataIn, readTimeoutMs);

        if (err == Drv::SPI::Status::NoError) {
            chipSelectSet();
            return dataIn.getSize() - headerBuff.getSize();
        }
    }

    chipSelectSet();
    return etl::unexpected(ErrorCode::IOInterfaceError);
  }

  [[nodiscard]] etl::expected<uint32_t, FRAMDriver::ErrorCode>
  FRAMDriver::write(uint32_t addr, Fw::Buffer &dataOut) {
    if (addr + dataOut.getSize() > memorySize) {
        return etl::unexpected(ErrorCode::AddressOutOfRange);
    }

    if (dataOut.getSize() == 0) {
        return etl::unexpected(ErrorCode::InputBufferEmpty);
    }

    if (auto result = writeEnable(); result.error() != ErrorCode::NoError) {
      return etl::unexpected(result.error());
    }
    
    etl::array header = {
        static_cast<uint8_t>(CommandCode::CmdWrite), static_cast<uint8_t>(addr >> thirdByteOffset),
        static_cast<uint8_t>(addr >> secondByteOffset), static_cast<uint8_t>(addr >> firstByteOffset)};

    chipSelectReset();
    
    Fw::Buffer headerBuff(header.data(), header.size());

    if (this->driver_write_out(0, headerBuff, writeTimeoutMs) == Drv::SPI::Status::NoError) {
      if (this->driver_write_out(0, dataOut, writeTimeoutMs) == Drv::SPI::Status::NoError) {
        chipSelectSet();
        if (auto result = writeDisable(); result.error() != ErrorCode::NoError) {
          return etl::unexpected(result.error());
        }
        return dataOut.getSize();
      }
    }

    chipSelectSet();
    if (auto result = writeDisable(); result.error() != ErrorCode::NoError) {
      return etl::unexpected(result.error());
    }

    return etl::unexpected(ErrorCode::IOInterfaceError);
  }

  FRAMDriver::ErrorCode FRAMDriver::wakeup() {
    chipSelectReset();
    Os::Task::delay(wakeupDelayTimeMs);
    chipSelectSet();
    return ErrorCode::NoError;
  }

  FRAMDriver::ErrorCode FRAMDriver::sleep() {
    ErrorCode error = ErrorCode::NoError;
    chipSelectReset();

    etl::array cmdSleepArr = {static_cast<uint8_t>(CommandCode::CmdSleep)};
    Fw::Buffer cmdSleepBuff(cmdSleepArr.data(), cmdSleepArr.size());
    
    if (this->driver_write_out(0, cmdSleepBuff, writeTimeoutMs) != Drv::SPI::Status::NoError) {
        error = ErrorCode::IOInterfaceError;
    }
    chipSelectSet();
    return error;
  }

  [[nodiscard]] etl::expected<uint32_t, FRAMDriver::ErrorCode> FRAMDriver::readID() {
    chipSelectReset();
    etl::array cmdReadIDArr = {static_cast<uint8_t>(CommandCode::CmdReadID)};
    Fw::Buffer cmdReadIDBuff(cmdReadIDArr.data(), cmdReadIDArr.size());

    if (this->driver_write_out(0, cmdReadIDBuff, writeTimeoutMs) == Drv::SPI::Status::NoError) {
        etl::array<uint8_t, 4> readArr{0};
        Fw::Buffer readBuff(readArr.data(), readArr.size());

        if (this->driver_read_out(0, readBuff, readTimeoutMs) == Drv::SPI::Status::NoError) {
            chipSelectSet();
            return etl::rotate_left<uint32_t>(readArr[0], fourthByteOffset) +
                   etl::rotate_left<uint32_t>(readArr[1], thirdByteOffset) +
                   etl::rotate_left<uint32_t>(readArr[2], secondByteOffset) +
                   etl::rotate_left<uint32_t>(readArr[3], firstByteOffset);
        }
    }

    chipSelectSet();
    return etl::unexpected(ErrorCode::IOInterfaceError);
  }

  etl::expected<uint8_t, FRAMDriver::ErrorCode> FRAMDriver::readStatusRegister() {
    chipSelectReset();
    etl::array cmdReadStatusArr = {static_cast<uint8_t>(CommandCode::CmdReadStatus)};
    Fw::Buffer cmdReadStatusBuff(cmdReadStatusArr.data(), cmdReadStatusArr.size());

    if (this->driver_write_out(0, cmdReadStatusBuff, writeTimeoutMs) ==
        Drv::SPI::Status::NoError) {
        etl::array statusArr = {static_cast<uint8_t>(0x00U)};
        Fw::Buffer statusBuff(statusArr.data(), statusArr.size());
        
        if (driver_read_out(0, statusBuff, readTimeoutMs) == Drv::SPI::Status::NoError) {
            chipSelectSet();
            return statusArr[0];
        }
    }

    chipSelectSet();
    return etl::unexpected(ErrorCode::IOInterfaceError);
  }

  etl::expected<void, FRAMDriver::ErrorCode> FRAMDriver::writeStatusRegister(uint8_t status) {
    chipSelectReset();

    etl::array cmdWriteStatusArr = {static_cast<uint8_t>(CommandCode::CmdWriteStatus)};
    Fw::Buffer cmdWriteStatusBuff(cmdWriteStatusArr.data(), cmdWriteStatusArr.size());
    
    if (this->driver_write_out(0, cmdWriteStatusBuff, writeTimeoutMs) == Drv::SPI::Status::NoError) {
      Fw::Buffer statusBuff(&status, 1);
      if (this->driver_write_out(0, statusBuff, writeTimeoutMs) == Drv::SPI::Status::NoError) {
        chipSelectSet();
        return etl::unexpected(ErrorCode::NoError);
      }
    }

    chipSelectSet();
    return etl::unexpected(ErrorCode::IOInterfaceError);
  }

  etl::expected<void, FRAMDriver::ErrorCode> FRAMDriver::writeEnable() {
    ErrorCode error = ErrorCode::NoError;
    chipSelectReset();
    etl::array cmdWriteEnableArr{static_cast<uint8_t>(CommandCode::CmdWriteEnable)};
    Fw::Buffer cmdWriteEnableBuff(cmdWriteEnableArr.data(), cmdWriteEnableArr.size());

    if (this->driver_write_out(0, cmdWriteEnableBuff, writeTimeoutMs) != Drv::SPI::Status::NoError) {
        error = ErrorCode::IOInterfaceError;
    }
    chipSelectSet();
    return etl::unexpected(error);
  }

  etl::expected<void, FRAMDriver::ErrorCode> FRAMDriver::writeDisable() {
    ErrorCode error = ErrorCode::NoError;
    chipSelectReset();
    etl::array cmdWriteDisableArr{static_cast<uint8_t>(CommandCode::CmdWriteDisable)};
    Fw::Buffer cmdWriteDisableBuff(cmdWriteDisableArr.data(), cmdWriteDisableArr.size());

    if (this->driver_write_out(0, cmdWriteDisableBuff, writeTimeoutMs) != Drv::SPI::Status::NoError) {
        error = ErrorCode::IOInterfaceError;
    }
    chipSelectSet();
    return etl::unexpected(error);
  }

  void FRAMDriver::chipSelectSet() {
    this->chipSelect_out(0, Fw::Logic::HIGH);
  }

  void FRAMDriver::chipSelectReset() {
    this->chipSelect_out(0, Fw::Logic::LOW);
  }

}
