// ======================================================================
// \title  Adc1Driver.cpp
// \author user
// \brief  cpp file for Adc1Driver component implementation class
// ======================================================================

#include "Falco/Drv/Adc1Driver/Adc1Driver.hpp"
#include "FpConfig.hpp"
#include "samv71q21b.h"

namespace Falco {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Adc1Driver ::
    Adc1Driver(const char* const compName) :
      Adc1DriverComponentBase(compName)
  {
    
  }

  Adc1Driver ::
    ~Adc1Driver()
  {
    
  }

  void Adc1Driver::configure() {
    XDMAC_ChannelCallbackRegister(XDMAC_CHANNEL_7, DMACallback, reinterpret_cast<uintptr_t>(this));
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
  Drv::ADC::Status Adc1Driver::getADCCode_handler(NATIVE_INT_TYPE portNum, U32& adc) {
    auto adcCodeResult = getADCCode();
    if (adcCodeResult.has_value()) {
        adc = adcCodeResult.value();
        return Drv::ADC::Status::SUCCESS;
    }
    return Drv::ADC::Status::FAIL;
  }

  U32 Adc1Driver::getADCMax_handler(NATIVE_INT_TYPE portNum) {
    return getADCMax();
  }

  F32 Adc1Driver ::
    getVoltageReference_handler(NATIVE_INT_TYPE portNum)
  {
    return getVoltageReference();
  }

  Drv::ADC::Status Adc1Driver ::
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

  void Adc1Driver::startConversion() {
      XDMAC_ChannelTransfer(
            XDMAC_CHANNEL_7, (const void *) &AFEC1_REGS->AFEC_LCDR, reinterpret_cast<const void *>(adcBuffer.data()),
            adcBuffer.size());
  }

  void Adc1Driver::DMACallback(XDMAC_TRANSFER_EVENT event, uintptr_t xdmacContext) {
      reinterpret_cast<Adc1Driver *>(xdmacContext)->DMAEventHandler(event);
  }

}
