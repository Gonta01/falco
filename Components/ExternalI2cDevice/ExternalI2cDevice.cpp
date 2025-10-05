// ======================================================================
// \title  ExternalI2cDevice.cpp
// \author user
// \brief  cpp file for ExternalI2cDevice component implementation class
// ======================================================================

#include "Components/ExternalI2cDevice/ExternalI2cDevice.hpp"
#include "FpConfig.hpp"
#include "etl/array.h"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  ExternalI2cDevice ::
    ExternalI2cDevice(const char* const compName) :
      ExternalI2cDeviceComponentBase(compName), 
      oneShotWriteEnable(false),
      oneShotReadEnable(false)
  {

  }

  ExternalI2cDevice ::
    ~ExternalI2cDevice()
  {

  }

  void ExternalI2cDevice::parameterUpdated(FwPrmIdType id) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    
    // Check the parameter ID is expected
    if (PARAMID_ADDRESS == id) {
      U32 deviceAddress = this->paramGet_ADDRESS(isValid);
      FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);
      this->tlmWrite_DeviceAddress(deviceAddress);
      this->log_ACTIVITY_HI_DeviceAddressSet(deviceAddress);
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ExternalI2cDevice::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Fw::ParamValid isAddressValid;
    U8 deviceAddress = this->paramGet_ADDRESS(isAddressValid);
    deviceAddress = ((Fw::ParamValid::INVALID == isAddressValid) || (Fw::ParamValid::UNINIT == isAddressValid)) ? 0 : deviceAddress;
    
    if (oneShotWriteEnable) {
      if (isAddressValid) {
        etl::array<uint8_t, 2> arr = {regValue, writeData};
        Fw::Buffer buffer(arr.data(), arr.size());
        bool writeResult = (write_out(0, deviceAddress, buffer, writeTimeout) == Drv::I2C::Status::SUCCESS);

        if (writeResult == true) {
          this->log_ACTIVITY_LO_WriteSuccess();
        }
        else {
          this->log_WARNING_HI_WriteFailure();
        }
      }
      else {
        this->log_WARNING_HI_InvalidAddress();
      }
      this->oneShotWriteEnable = false;
    }

    if (oneShotReadEnable) {
      if (isAddressValid) {
        etl::array<uint8_t, 1> arr = {regValue};
        Fw::Buffer writeBuffer(arr.data(), arr.size());
        bool writeResult = (write_out(0, deviceAddress, writeBuffer, writeTimeout) == Drv::I2C::Status::SUCCESS);

        bool readResult = false;
        if (writeResult == true) {
          Fw::Buffer readBuffer(&readData, 1);
          readResult = (read_out(0, deviceAddress, readBuffer, readTimeout) == Drv::I2C::Status::SUCCESS);
        }

        if (writeResult && readResult) {
          this->tlmWrite_ReadData(readData);
          this->log_ACTIVITY_HI_ReadData(readData);
          this->log_ACTIVITY_LO_ReadSuccess();
        }
        else {
          this->log_WARNING_HI_ReadFailure();
        }
      }
      else {
        this->log_WARNING_HI_InvalidAddress();
      }
      this->oneShotReadEnable = false;
    }
  }
  
  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void ExternalI2cDevice ::
    WRITE_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U8 regValue,
        U8 writeData
    )
  {
    this->regValue = regValue;
    this->tlmWrite_DeviceRegister(regValue);
    this->writeData = writeData;
    oneShotWriteEnable = true;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void ExternalI2cDevice ::
    READ_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U8 regValue
    )
  {
    this->regValue = regValue;
    this->tlmWrite_DeviceRegister(regValue);
    oneShotReadEnable = true;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
