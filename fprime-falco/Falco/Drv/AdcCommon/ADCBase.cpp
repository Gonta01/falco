#include "ADCBase.hpp"
#include "IADC.hpp"
#include "event_groups.h"
#include "portmacro.h"
#include "projdefs.h"
#include "samv71q21b.h"
#include "ticks.hpp"
#include "xdmac/plib_xdmac.h"
#include "xdmac/plib_xdmac_common.h"
#include <cstdint>
#include <etl/array_view.h>
#include <etl/expected.h>

etl::expected<float, Interfaces::ADCError> Base::ADC::getVoltageValue() {
    if (!mutex.Lock()) {
        return etl::unexpected(Interfaces::ADCError::Busy);
    }

    const Interfaces::ADCError error = Interfaces::ADCError::IOError;
    uint32_t averageRawValue = 0;
    bool receivedValue = false;

    startConversion();
    if (waitConversionEnd()) {
        SCB_InvalidateDCache_by_Addr(adcBuffer.data(), adcBuffer.size());
        averageRawValue = getAverage(adcBuffer);
        receivedValue = true;
    }
    float voltageValue = static_cast<float>(averageRawValue) * ADC_VREF / maxADCCode;
    mutex.Unlock();

    if (receivedValue) {
        return voltageValue;
    }
    return etl::unexpected(error);
}

etl::expected<uint32_t, Interfaces::ADCError> Base::ADC::getADCCode() {
    if (!mutex.Lock()) {
        return etl::unexpected(Interfaces::ADCError::Busy);
    }

    const Interfaces::ADCError error = Interfaces::ADCError::IOError;
    uint32_t averageRawValue = 0;
    bool receivedValue = false;

    startConversion();
    if (waitConversionEnd()) {
        SCB_InvalidateDCache_by_Addr(adcBuffer.data(), adcBuffer.size());
        averageRawValue = getAverage(adcBuffer);
        receivedValue = true;
    }
    uint32_t ADCCode = averageRawValue;
    mutex.Unlock();

    if (receivedValue) {
        return ADCCode;
    }
    return etl::unexpected(error);
}

float Base::ADC::getVoltageReference() {
    return ADC_VREF;
}

uint32_t Base::ADC::getADCMax() {
    return maxADCCode;
}

bool Base::ADC::waitConversionEnd() {
    const EventBits_t uxBits1 = eventGroup.WaitBits(
            evtDataReady | evtError, true, false, cpp_freertos::Ticks::MsToTicks(maxEventTimeoutMs));
    return (uxBits1 == evtDataReady);
}

uint32_t Base::ADC::getAverage(const etl::array_view<uint16_t> &arrayView) {
    uint32_t averageValue = 0;
    for (uint16_t const adcValue: arrayView) {
        averageValue += adcValue;
    }
    averageValue /= arrayView.size();
    return averageValue;
}

void Base::ADC::DMAEventHandler(XDMAC_TRANSFER_EVENT event) {
    const EventBits_t events = (XDMAC_TRANSFER_COMPLETE == event) ? evtDataReady : evtError;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    eventGroup.SetBitsFromISR(events, &xHigherPriorityTaskWoken);
}
