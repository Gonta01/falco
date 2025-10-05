#ifndef ADCBASE_HPP
#define ADCBASE_HPP

#include "IADC.hpp"
#include "event_groups.hpp"
#include "mutex.hpp"
#include "xdmac/plib_xdmac.h"
#include "xdmac/plib_xdmac_common.h"
#include <cstdint>
#include <etl/array.h>
#include <etl/array_view.h>
#include <etl/expected.h>

/**
 * @brief Namespace Base
 */
namespace Base {

    class ADC : public Interfaces::IADC {
    protected:
        static constexpr uint16_t numOfSamples{32};
        etl::array<uint16_t, numOfSamples> adcBuffer __attribute__((aligned(32))) {0};
        static constexpr float ADC_VREF{2.5};
        static constexpr uint16_t maxADCCode{4095};

        static constexpr EventBits_t evtNone{1U << 0U};
        static constexpr EventBits_t evtDataReady{1U << 1U};
        static constexpr EventBits_t evtError{1U << 2U};

        static constexpr uint16_t maxEventTimeoutMs{0xffff};

        cpp_freertos::MutexStandard mutex;
        cpp_freertos::EventGroup eventGroup;

        float getVoltageReference() override;

        uint32_t getADCMax() override;

        /**
         * @brief Function to start ADC conversion and sample buffer filling
         */
        virtual void startConversion() = 0;

        /**
         * @brief Function to wait for the end of ADC conversion
         * @return true, if conversion finished and data is ready, otherwise - false
         */
        bool waitConversionEnd();

        /**
         * @brief Function to receive average value of the array
         * @return average value of the array
         */
        uint32_t getAverage(const etl::array_view<uint16_t> &arrayView);

        /**
         * @brief DMA event handler
         */
        void DMAEventHandler(XDMAC_TRANSFER_EVENT event);

    public:
        /**
         * @brief Get average analog input value in volts. If value wasn't received the error code is returned
         * @return average analog input value in volts or error code if value wasn't received
         */
        etl::expected<float, Interfaces::ADCError> getVoltageValue() override;
        
        /**
         * @brief Get average ADC code. If value wasn't received the error code is returned
         * @return average analog input value in volts or error code if value wasn't received
         */
        etl::expected<uint32_t, Interfaces::ADCError> getADCCode() override;

    };

} // namespace Base

#endif // ADCBASE_HPP
