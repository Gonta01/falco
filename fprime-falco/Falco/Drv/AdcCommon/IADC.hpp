#ifndef IADC_HPP
#define IADC_HPP

#include <cstdint>
#include <etl/expected.h>

/**
 * @brief Interface layer namespace
 */
namespace Interfaces {
    /**
     * ADC Error list
     */
    enum class ADCError : uint8_t {
        NoError,
        IOError,
        Busy
    };

    /**
     * @brief ADC class
     */
    class IADC {
    public:
        /**
         * @brief Class destructor
         */
        virtual ~IADC() {
        }

        /**
         * @brief Get analog input value in Volts
         * @return analog input value in Volts
         */
        virtual etl::expected<float, ADCError> getVoltageValue() = 0;

        /**
         * @brief Get voltage reference of ADC
         * @return ADC voltage reference
         */
        virtual float getVoltageReference() = 0;

        virtual etl::expected<uint32_t, ADCError> getADCCode() = 0;

        virtual uint32_t getADCMax() = 0;

    };

} // namespace Interfaces

#endif //IADC_HPP