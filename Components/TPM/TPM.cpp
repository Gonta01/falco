// ======================================================================
// \title  TPM.cpp
// \author user
// \brief  cpp file for TPM component implementation class
// ======================================================================

#include "Components/TPM/TPM.hpp"
#include "FpConfig.hpp"

#include <etl/array.h>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TPM::TPM(const char* const compName) :
      TPMComponentBase(compName) {

  }

  TPM::~TPM() {

  }

  void TPM :: configure() {
    
  }  

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void TPM ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    /* Wait for chip startup to complete */
    U8 accesTPMResult = 0;
    if (!readReg(static_cast<U8>(accessTPM), accesTPMResult)) {
      this->log_WARNING_HI_CommunicationError();
    }

    bool isDataCorrect = ((accesTPMResult & accessTPMValid) && (accesTPMResult != 0xFF));
    if (isDataCorrect == false) {
      this->log_WARNING_HI_CommunicationError();
    }

    /* Get device information */
    U8 intfCapsResult = 0;
    if (!readReg(static_cast<U8>(intfCaps), intfCapsResult)) {
      this->log_WARNING_HI_CommunicationError();
    }

    U8 didVidResult = 0;
    if (!readReg(static_cast<U8>(didVid), didVidResult)) {
      this->log_WARNING_HI_CommunicationError();
    }

    U8 ridResult = 0;
    if (!readReg(static_cast<U8>(rid), ridResult)) {
      this->log_WARNING_HI_CommunicationError();
    }
  }

  bool TPM::readReg(uint8_t reg, uint8_t &value) {
    etl::array<uint8_t, 1> arr = {reg};
    Fw::Buffer writeBuffer(arr.data(), arr.size());
    if (write_out(0, SLB9673Addr, writeBuffer, writeTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }

    etl::array<uint8_t, 1> readArr{};
    Fw::Buffer readBuffer(readArr.data(), readArr.size());
    if (read_out(0, SLB9673Addr, readBuffer, readTimeout) != Drv::I2C::Status::SUCCESS) {
        return false;
    }
    value = readArr.front();

    return true;
  }

}
