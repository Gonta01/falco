// ======================================================================
// \title  Adc0Driver.cpp
// \author user
// \brief  cpp file for Adc0Driver component implementation class
// ======================================================================

#include "Falco/Drv/Adc0Driver/Adc0Driver.hpp"
#include "FpConfig.hpp"
#include "samv71q21b.h"

namespace Falco {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Adc0Driver ::
    Adc0Driver(const char* const compName) :
      Adc0DriverComponentBase(compName)
  {
    
  }

  Adc0Driver ::
    ~Adc0Driver()
  {

  }
  
  void Adc0Driver::configure() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_6, DMACallback, reinterpret_cast<uintptr_t>(this));
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Drv::ADC::Status Adc0Driver::getADCCode_handler(NATIVE_INT_TYPE portNum, U32& adc) {
    auto adcCodeResult = getADCCode();
    if (adcCodeResult.has_value()) {
        adc = adcCodeResult.value();
        return Drv::ADC::Status::SUCCESS;
    }
    return Drv::ADC::Status::FAIL;
  }

  U32 Adc0Driver::getADCMax_handler(NATIVE_INT_TYPE portNum) {
    return getADCMax();
  }

  F32 Adc0Driver ::
    getVoltageReference_handler(NATIVE_INT_TYPE portNum)
  {
    return getVoltageReference();
  }

  Drv::ADC::Status Adc0Driver ::
    getVoltageValue_handler(
        NATIVE_INT_TYPE portNum,
        F32& voltage
    )
  {
    auto voltageValueResult = getVoltageValue();
    if (voltageValueResult.has_value()) {
        voltage = voltageValueResult.value();
        return Drv::ADC::Status::SUCCESS;
    }
    return Drv::ADC::Status::FAIL;
  }

  void Adc0Driver::startConversion() {
      XDMAC_ChannelTransfer(
              XDMAC_CHANNEL_6, (const void *) &AFEC0_REGS->AFEC_LCDR, reinterpret_cast<const void *>(adcBuffer.data()),
              adcBuffer.size());
  }

  void Adc0Driver::DMACallback(XDMAC_TRANSFER_EVENT event, uintptr_t xdmacContext) {
      reinterpret_cast<Adc0Driver *>(xdmacContext)->DMAEventHandler(event);
  }
}
